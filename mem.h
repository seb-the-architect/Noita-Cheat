#pragma once
#include "pch.h"

void PlaceJMP(BYTE* Address, BYTE* jumpTo, unsigned int length);

BYTE* TrampHook(void* src, void* dst, unsigned int len);

MODULEINFO GetModuleInfo(LPCWSTR szModule);

DWORD FindPattern(LPCWSTR module, const char* pattern, const char* mask);

void Patch(BYTE* dst, BYTE* src, unsigned int size);