#include <windows.h>
#include "manlueModules.hpp"

typedef struct _UNICODE_STRING {
    USHORT Length;
    USHORT MaximumLength;
    PWSTR Buffer;
} UNICODE_STRING, * PUNICODE_STRING;

typedef struct _LDR_DATA_TABLE_ENTRY {
    LIST_ENTRY InLoadOrderLinks;
    LIST_ENTRY InMemoryOrderLinks;
    LIST_ENTRY InInitializationOrderLinks;
    PVOID DllBase;
    PVOID EntryPoint;
    ULONG SizeOfImage;
    UNICODE_STRING FullDllName;
    UNICODE_STRING BaseDllName;
} LDR_DATA_TABLE_ENTRY, * PLDR_DATA_TABLE_ENTRY;

typedef struct _PEB_LDR_DATA {
    ULONG Length;
    UCHAR Initialized;
    PVOID SsHandle;
    LIST_ENTRY InLoadOrderModuleList;
} PEB_LDR_DATA, * PPEB_LDR_DATA;

typedef struct _PEB {
    UCHAR Reserved1[2];
    UCHAR BeingDebugged;
    UCHAR Reserved2[1];
    PVOID Reserved3[2];
    PPEB_LDR_DATA Ldr;
} PEB, * PPEB;


void* GetModuleFunction(const wchar_t* moduleName, const char* functionName) {
    PPEB peb = (PPEB)__readgsqword(0x60); // Get PEB in 64-bit
    PPEB_LDR_DATA ldr = peb->Ldr;

    for (PLIST_ENTRY listEntry = ldr->InLoadOrderModuleList.Flink; listEntry != &ldr->InLoadOrderModuleList; listEntry = listEntry->Flink) {
        PLDR_DATA_TABLE_ENTRY moduleEntry = CONTAINING_RECORD(listEntry, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks);

        if (moduleEntry->BaseDllName.Buffer && _wcsicmp(moduleEntry->BaseDllName.Buffer, moduleName) == 0) {
            PBYTE moduleBase = (PBYTE)moduleEntry->DllBase;
            PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER)moduleBase;
            PIMAGE_NT_HEADERS ntHeaders = (PIMAGE_NT_HEADERS)(moduleBase + dosHeader->e_lfanew);

            DWORD exportRVA = ntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;
            PIMAGE_EXPORT_DIRECTORY exportDir = (PIMAGE_EXPORT_DIRECTORY)(moduleBase + exportRVA);

            PDWORD nameTable = (PDWORD)(moduleBase + exportDir->AddressOfNames);
            PWORD ordinalTable = (PWORD)(moduleBase + exportDir->AddressOfNameOrdinals);
            PDWORD functionTable = (PDWORD)(moduleBase + exportDir->AddressOfFunctions);

            for (DWORD i = 0; i < exportDir->NumberOfNames; i++) {
                const char* functionNameRVA = (const char*)(moduleBase + nameTable[i]);

                if (_stricmp(functionNameRVA, functionName) == 0) {
                    WORD ordinal = ordinalTable[i];
                    return (void*)(moduleBase + functionTable[ordinal]);
                }
            }
        }
    }

    return nullptr;
}