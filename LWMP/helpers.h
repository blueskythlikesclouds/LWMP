﻿#pragma once

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <detours.h>

#define FUNCTION_PTR(returnType, callingConvention, function, location, ...) \
	returnType (callingConvention *function)(__VA_ARGS__) = (returnType(callingConvention*)(__VA_ARGS__))(location)

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

#define VTABLE_HOOK(returnType, callingConvention, className, functionName, ...) \
	typedef returnType callingConvention functionName(className* This, __VA_ARGS__); \
	functionName* original##functionName; \
	returnType callingConvention implOf##functionName(className* This, __VA_ARGS__)

#define INSTALL_VTABLE_HOOK(object, functionName, functionIndex) \
	{ \
		void** addr = &(*(void***)object)[functionIndex]; \
		if (*addr != implOf##functionName) \
		{ \
			original##functionName = (functionName*)*addr; \
			DWORD oldProtect; \
			VirtualProtect(addr, sizeof(void*), PAGE_EXECUTE_READWRITE, &oldProtect); \
			*addr = implOf##functionName; \
			VirtualProtect(addr, sizeof(void*), oldProtect, NULL); \
		} \
	}

#define WRITE_MEMORY(location, ...) \
	{ \
		const char data[] = { __VA_ARGS__ }; \
		DWORD oldProtect; \
		VirtualProtect((void*)location, sizeof(data), PAGE_EXECUTE_READWRITE, &oldProtect); \
		memcpy((void*)location, data, sizeof(data)); \
		VirtualProtect((void*)location, sizeof(data), oldProtect, NULL); \
	}