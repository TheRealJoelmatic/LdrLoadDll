#include <windows.h>
#include <iostream>
#include <string>

#include <winternl.h>
#pragma comment(lib, "ntdll.lib")

#include <psapi.h>
#pragma comment(lib, "Psapi.lib")

#include "../ext/callstack/CallStack-Spoofer.h"
#include "hooks/hooks.hpp"
#include "ultils/manlueModules.hpp"

//
// POC: BY JOELMATIC
// The Idea is to just load another DLL with it looking like it was loaded legit
//
//
// This dll will be manlued mapped to the target process
// Then will load a new dll into the target process
//

// 
// In this dll we are trying to avoid winapi calls as they can be tracked back
//


// 
// Credits
//
// https://github.com/Barracudach/CallStack-Spoofer/tree/main

VOID EntryPoint()
{
    SPOOF_FUNC;

    PWSTR _dllPath;
    _dllPath = SPOOF_CALL(_wcsdup)(L"C:\\Users\\Joelm\\Desktop\\POC-IDEA\\Injector\\x64\\Release\\Internal.dll");

    LPCWSTR _dllName = L"Internal.dll";

    //get the function
    LdrLoadDll = (LdrLoadDll_t)SPOOF_CALL(GetModuleFunction)(L"ntdll.dll", "LdrLoadDll");

    // load teh hooks we need
    SPOOF_CALL(hooks::initHooks)(_dllPath, _dllName);

    SPOOF_CALL(Sleep)(300); //fixes it not injecting idk why tho

    // call the function that will be intercepted
    UNICODE_STRING _ModuleFileName = { 0 };
    HANDLE hModule = NULL;

    SPOOF_CALL(RtlInitUnicodeString)(&_ModuleFileName, L"kernel32.dll");

    NTSTATUS STATUS = LdrLoadDll(NULL, 0, &_ModuleFileName, &hModule);
    if (STATUS != 0x00000000)
	{
        //std::string errorMsg = "LdrLoadDll failed with status: 0x" + std::to_string(STATUS);
        //MessageBoxA(NULL, errorMsg.c_str(), "Error", MB_OK | MB_ICONERROR);
		return;
	}

    //unhook evrything after
    SPOOF_CALL(hooks::uninitHook)();
    SPOOF_CALL(free)(_dllPath);

    return;
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    SPOOF_FUNC;

    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        SPOOF_CALL(EntryPoint)();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}