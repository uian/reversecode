#include <windows.h>

#pragma comment(linker, "/INCLUDE:__tls_used")// 通过#pragma comment(Linker,"INCLUDE:_tls_used")显示的申明crt库中定义的_tls_used变量
　　　　　　　　　　　　　　　　　　　　　　　　　　　 // 以便向_tls_used!AddressOfCallBacks域添加回调函数。　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　
void print_console(char* szMsg)
{
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

    WriteConsoleA(hStdout, szMsg, strlen(szMsg), NULL, NULL);
}

void NTAPI TLS_CALLBACK1(PVOID DllHandle, DWORD Reason, PVOID Reserved)
{
    char szMsg[80] = {0,};
    wsprintfA(szMsg, "TLS_CALLBACK1() : DllHandle = %X, Reason = %d\n", DllHandle, Reason);
    print_console(szMsg);
}

void NTAPI TLS_CALLBACK2(PVOID DllHandle, DWORD Reason, PVOID Reserved)
{
    char szMsg[80] = {0,};
    wsprintfA(szMsg, "TLS_CALLBACK2() : DllHandle = %X, Reason = %d\n", DllHandle, Reason);
    print_console(szMsg);
}

#pragma data_seg(".CRT$XLX")   //CRT 表明使用 C RunTime 机制，X 表示标识名随机，L 表示 TLS callback section， X 为 B-Y 之间的任意一个字母
    PIMAGE_TLS_CALLBACK pTLS_CALLBACKs[] = { TLS_CALLBACK1, TLS_CALLBACK2, 0 }; // 注意，此处将自定义函数放入了 TLS 表中。
#pragma data_seg()

DWORD WINAPI ThreadProc(LPVOID lParam)
{
    print_console("ThreadProc() start\n");

    print_console("ThreadProc() end\n");

    return 0;
}

int main(void)
{
    HANDLE hThread = NULL;

    print_console("main() start\n");

    hThread = CreateThread(NULL, 0, ThreadProc, NULL, 0, NULL);
    WaitForSingleObject(hThread, 60*1000);
    CloseHandle(hThread);

    print_console("main() end\n");

    return 0;
}