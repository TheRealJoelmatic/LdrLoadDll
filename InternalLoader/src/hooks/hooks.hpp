#include <Windows.h>
#include <iostream>
#include <winternl.h>

typedef NTSTATUS(NTAPI* LdrLoadDll_t)(
    PWSTR PathToFile,
    ULONG Flags,
    PUNICODE_STRING ModuleFileName,
    PHANDLE ModuleHandle);

inline LdrLoadDll_t LdrLoadDll = nullptr;

namespace hooks {
	void initHooks(PWSTR _dllPath, PCWSTR _dllName);
	void uninitHook();
}