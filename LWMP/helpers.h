#pragma once

const HMODULE MODULE_HANDLE = GetModuleHandle(nullptr);

#define ASLR(address) \
    ((unsigned int)MODULE_HANDLE + (unsigned int)address - 0x400000)

#define FUNCTION_PTR(returnType, callingConvention, function, location, ...) \
    returnType (callingConvention *function)(__VA_ARGS__) = (returnType(callingConvention*)(__VA_ARGS__))(location);

#define PROC_ADDRESS(libraryName, procName) \
    GetProcAddress(LoadLibrary(TEXT(libraryName)), procName)

#define HOOK(returnType, callingConvention, functionName, location, ...) \
    typedef returnType callingConvention functionName(__VA_ARGS__); \
    functionName* original##functionName = (functionName*)(location); \
    returnType callingConvention implOf##functionName(__VA_ARGS__)

#define INSTALL_HOOK(functionName) \
    { \
        DetourTransactionBegin(); \
        DetourUpdateThread(GetCurrentThread()); \
        DetourAttach((void**)&original##functionName, implOf##functionName); \
        DetourTransactionCommit(); \
    }

#define UNINSTALL_HOOK(functionName) \
    { \
        DetourTransactionBegin(); \
        DetourUpdateThread(GetCurrentThread()); \
        DetourDetach((void**)&original##functionName, implOf##functionName); \
        DetourTransactionCommit(); \
    }

#define WRITE_MEMORY(location, ...) \
    { \
        const unsigned char data[] = { __VA_ARGS__ }; \
        DWORD oldProtect; \
        VirtualProtect((void*)location, sizeof(data), PAGE_EXECUTE_READWRITE, &oldProtect); \
        memcpy((void*)location, data, sizeof(data)); \
        VirtualProtect((void*)location, sizeof(data), oldProtect, NULL); \
    }
