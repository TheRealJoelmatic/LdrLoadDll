#include <Windows.h>

VOID EntryPoint() {
    MessageBoxA(NULL, "Injected!", "Injected!", MB_OK);
    Beep(1000, 1000);
}
BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        EntryPoint();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

