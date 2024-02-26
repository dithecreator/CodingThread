#include "CodingThreadDlg.h"

CodingThreadDlg* CodingThreadDlg::ptr = NULL;
HANDLE hMutex;

CodingThreadDlg::CodingThreadDlg(void)
{
    ptr = this;
    hDialog = NULL;
}

CodingThreadDlg::~CodingThreadDlg(void)
{

}

void CodingThreadDlg::Cls_OnClose(HWND hwnd)
{
    ReleaseMutex(hMutex); // освобождение мьютекса
    EndDialog(hwnd, 0);
}

DWORD WINAPI Coding_Thread(LPVOID lp)
{
    CodingThreadDlg* ptr = (CodingThreadDlg*)lp;
    char buf[100];
    ifstream in("music.txt", ios::in | ios::binary);

    if (!in)
    {
        MessageBoxA(ptr->hDialog, "Ошибка открытия файла!", "Мьютекс", MB_OK | MB_ICONINFORMATION);
        return 1;
    }

    hMutex = OpenMutex(MUTEX_ALL_ACCESS, false, L"{B8A2C367-10FE-494d-A869-841B2AF972E0}");

    DWORD dwAnswer = WaitForSingleObject(hMutex, INFINITE);

    ofstream out("coding.txt", ios::out | ios::trunc | ios::binary);
    const int KEY = 100;
    while (!in.eof())
    {
        in.getline(buf, 100);
        out << buf << endl;
    }

    out.close();
    in.close();
    ReleaseMutex(hMutex); // освобождение мьютекса

    return 0;
}

DWORD WINAPI Thread1(LPVOID lp)
{
    CodingThreadDlg* ptr = (CodingThreadDlg*)lp;
    HWND hEdit = GetDlgItem(ptr->hDialog, IDC_EDIT1);
    HWND hButton = GetDlgItem(ptr->hDialog, IDC_BUTTON1);
    char buf[100];
    ifstream in("music.txt", ios::in | ios::binary);

    if (!in)
    {
        MessageBoxA(ptr->hDialog, "Ошибка открытия файла!", "Мьютекс", MB_OK | MB_ICONINFORMATION);
        return 1;
    }

    hMutex = OpenMutex(MUTEX_ALL_ACCESS, false, L"{0D68E69C-F6D7-456B-ADE5-8277ED004AE3}");

    DWORD dwAnswer = WaitForSingleObject(hMutex, INFINITE);

    ofstream out("coding.txt", ios::out | ios::trunc | ios::binary);
    const int KEY = 100;
    while (!in.eof())
    {
        in.getline(buf, 100);
        out << buf << endl;

        // Добавляем новую строку к содержимому hEdit
        SendMessageA(hEdit, EM_REPLACESEL, 0, (LPARAM)buf);
        SendMessageA(hEdit, EM_REPLACESEL, 0, (LPARAM)"\r\n"); // Добавляем новую строку
    }

    out.close();
    in.close();

    return 0;
}

BOOL CodingThreadDlg::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
    hDialog = hwnd;
    CreateThread(NULL, 0, Coding_Thread, this, 0, NULL);
    CreateThread(NULL, 0, Thread1, this, 0, NULL);
    return TRUE;
}

BOOL CALLBACK CodingThreadDlg::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
        HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);

    case WM_COMMAND:
        if (LOWORD(wParam) == IDC_BUTTON1) {
            ReleaseMutex(hMutex); // освобождение мьютекса
            EndDialog(hwnd, 0);
        }
        break;
    }
    return FALSE;
}
