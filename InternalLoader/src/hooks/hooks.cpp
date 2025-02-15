#include <Windows.h>
#include <iostream>


#include "hooks.hpp"
#include "../../ext/stafyhook/safetyhook.hpp"

namespace hooks {
	PWSTR dllPath;
	PCWSTR dllName;

	SafetyHookMid g_LdrLoadDll{};

	void hookLdrLoadDllWarpper(safetyhook::Context& ctx) {

		//kernel32.dll
		
		//PWSTR PathToFile,
		//ULONG Flags,
		//PUNICODE_STRING ModuleFileName,
		//PHANDLE ModuleHandle
		
		// rcx is the first arg
		// rdx is the 2 arg;
		// r8 is the 3 arg;


		UNICODE_STRING* RequestedDll = (UNICODE_STRING*)ctx.r8;
		if (RequestedDll) {
			//MessageBoxA(NULL, "Intercepted -> Redirecting to Internal.dll!", "Hook Triggered", MB_OK);

			ctx.rcx = (uintptr_t)dllPath;

			static UNICODE_STRING HookedModuleName;
			RtlInitUnicodeString(&HookedModuleName, dllPath);
			ctx.r8 = (uintptr_t)&HookedModuleName;
		}

	}


	void initHooks(PWSTR _dllPath, PCWSTR _dllName) {
	
		dllPath = _wcsdup(_dllPath);
		dllName = _dllName;

		g_LdrLoadDll = safetyhook::create_mid(LdrLoadDll, hookLdrLoadDllWarpper);

	}

	void uninitHook() {
		g_LdrLoadDll = {};
	}
}