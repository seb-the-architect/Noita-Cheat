// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "mem.h"
#include "structures.h"

DWORD LevJmpBack = 0x0;
DWORD HelJmpBack = 0x0;
DWORD AhlJmpBack = 0x0;
DWORD ManJmpBack = 0x0;
DWORD OxyJmpBack = 0x0;
DWORD AmmJmpBack = 0x0;
DWORD GldJmpBack = 0x0;

__declspec(naked) void LevitationHook()
{
    __asm
    {
        movss xmm0, [eax + 0x00000110]
        mov values.Levitation, eax
        add values.Levitation, 0x110
        jmp [LevJmpBack]
    }
}

__declspec(naked) void HealthHook()
{
    __asm
    {
        movsd xmm1, [eax+0x48]
        mov values.Health, eax
        mov values.MaxHealth, eax
        add values.Health, 0x4C
        add values.MaxHealth, 0x54
        jmp [HelJmpBack]
    }
}

__declspec(naked) void AllHealthHook()
{
    __asm
    {
        comisd xmm0, [esi+0x48]
        mov values.AllHealth, esi
        add values.AllHealth, 0x4C
    }

    //Making sure we dont alter our own health
    if (values.Health && settings.bInstaKill)
    {
        if (values.AllHealth != values.Health)
        {
            *(values.AllHealth) = 0.01f;
        }
    }

    __asm
    {
        jmp[AhlJmpBack]
    }
}

__declspec(naked) void ManaHook()
{
    __asm
    {
        movss xmm0, [eax + 0x00000088]
        mov values.Mana, eax
        mov values.MaxMana, eax
        add values.Mana, 0x88
        add values.MaxMana, 0x8C
        jmp [ManJmpBack]
    }
}

__declspec(naked) void OxygenHook()
{
    __asm
    {
        movss xmm0, [eax + 0x000000C8]
        mov values.Oxygen, eax
        add values.Oxygen, 0xC4
        jmp [OxyJmpBack]
    }
}

__declspec(naked) void AmmoHook()
{
    __asm
    {
        mov eax, [ecx + 0x68]
        cmp eax, edi
        mov values.Ammo, ecx
        mov ecx, [esp + 0x28]
        cmovl edi, eax
        add values.Ammo, 0x68
        jmp [AmmJmpBack]
    }
}

__declspec(naked) void GoldHook()
{
    __asm
    {
        mov ecx, [eax + 0x48]
        mov values.Gold, eax
        mov eax, [eax + 0x4C]
        add values.Gold, 0x48
        jmp [GldJmpBack]
    }
}

DWORD WINAPI HackThread(HMODULE hModule)
{
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
    std::cout << "Yo whats good" << std::endl;

    //Finding patterns
    uintptr_t levitationHookAddy = FindPattern(L"Noita.exe",
        "\xF3\x0F\x00\x00\x00\x00\x00\x00\xF3\x0F"
        "\x00\x00\x00\x00\x00\x00\xF3\x0F\x00\x00"
        "\x00\x00\x00\x00\xE8\x00\x00\x00\x00\xF3"
        "\x0F\x00\x00\x00\x00\x00\x00\x8D\x8D", 
        "xx??????xx??????xx??????x????xx??????xx");

    uintptr_t healthHookAddy = FindPattern(L"Noita.exe",
        "\xF2\x0F\x00\x00\x00\x0F\x5A\x00\x66\x0F\x00\x00\x0F\x97",
        "xx???xx?xx??xx");

    uintptr_t allHealthHookAddy = FindPattern(L"Noita.exe",
        "\x66\x0F\x00\x00\x00\x0F\x57\x00\x0F\x93",
        "xx???xx?xx");

    uintptr_t manaHookAddy = FindPattern(L"Noita.exe",
        "\xF3\x0F\x00\x00\x00\x00\x00\x00\xF3\x0F\x00\x00\x00\x00"
        "\x00\x00\x8D\x85\x00\x00\x00\x00\xF3\x0F\x00\x00\x00\x00\x50",
        "xx??????xx??????xx????xx????x");

    uintptr_t oxygenHookAddy = FindPattern(L"Noita.exe",
        "\xF3\x0F\x00\x00\x00\x00\x00\x00\x0F\x2F\x00\x00\x00\x00\x00\x0F"
        "\x86\x00\x00\x00\x00\xC7\x85\xF4\xFD\xFF\xFF",
        "xx??????xx?????xx????xxxxxx");

    uintptr_t ammoHookAddy = FindPattern(L"Noita.exe",
        "\x8B\x41\x00\x3B\xC7\x8B\x4C",
        "xx?xxxx");

    uintptr_t goldHookAddy = FindPattern(L"Noita.exe",
        "\x8B\x48\x00\x8B\x40\x00\x85\xC0",
        "xx?xx?xx");

    //Hooking Levitation
    BYTE levitationHookAddyBytes[8]{ 0 };
    memcpy(levitationHookAddyBytes, (void*)levitationHookAddy, 8);
    LevJmpBack = levitationHookAddy + 0x8;
    PlaceJMP((BYTE*)levitationHookAddy, (BYTE*)LevitationHook, 8);

    //Hooking Health
    BYTE healthHookAddyBytes[5]{ 0 };
    memcpy(healthHookAddyBytes, (void*)healthHookAddy, 5);
    HelJmpBack = healthHookAddy + 0x5;
    PlaceJMP((BYTE*)healthHookAddy, (BYTE*)HealthHook, 5);

    //Hooking allHealth
    BYTE allHealthHookAddyBytes[5]{ 0 };
    memcpy(allHealthHookAddyBytes, (void*)allHealthHookAddy, 5);
    AhlJmpBack = allHealthHookAddy + 0x5;
    PlaceJMP((BYTE*)allHealthHookAddy, (BYTE*)AllHealthHook, 5);

    //Hooking mana
    BYTE manaHookAddyBytes[8]{ 0 };
    memcpy(manaHookAddyBytes, (void*)manaHookAddy, 8);
    ManJmpBack = manaHookAddy + 0x8;
    PlaceJMP((BYTE*)manaHookAddy, (BYTE*)ManaHook, 8);

    //Hooking oxygen
    BYTE oxygenHookAddyBytes[8]{ 0 };
    memcpy(oxygenHookAddyBytes, (void*)oxygenHookAddy, 8);
    OxyJmpBack = oxygenHookAddy + 0x8;
    PlaceJMP((BYTE*)oxygenHookAddy, (BYTE*)OxygenHook, 8);

    //Hooking ammo
    BYTE ammoHookAddyBytes[12]{ 0 };
    memcpy(ammoHookAddyBytes, (void*)ammoHookAddy, 12);
    AmmJmpBack = ammoHookAddy + 0xC;
    PlaceJMP((BYTE*)ammoHookAddy, (BYTE*)AmmoHook, 12);

    //Hooking gold
    BYTE goldHookAddyBytes[6]{ 0 };
    memcpy(goldHookAddyBytes, (void*)goldHookAddy, 6);
    GldJmpBack = goldHookAddy + 0x6;
    PlaceJMP((BYTE*)goldHookAddy, (BYTE*)GoldHook, 6);

    while (!GetAsyncKeyState(VK_END))
    {
        if (settings.bAmmo && values.Ammo)
        {
            *(values.Ammo) = 99;
        }
        if (settings.bHealth && values.MaxHealth && values.Health)
        {
            *(values.MaxHealth) = 3.0622f;
            *(values.Health) = 3.0622f;
        }
        if (settings.bLevitation && values.Levitation)
        {
            *(values.Levitation) = 3.0f;
        }
        if (settings.bMana && values.Mana && values.MaxMana)
        {
            *(values.MaxMana) = 999.0f;
            *(values.Mana) = 999.0f;
        }
        if (settings.bOxygen && values.Oxygen)
        {
            *(values.Oxygen) = 8.0f;
        }
        if (settings.bRich && values.Gold)
        {
            *(values.Gold) = 999;
        }

        Sleep(25);
    }

    //Loops

    //Exit (Dont forget to patch)
    Patch((BYTE*)levitationHookAddy, levitationHookAddyBytes, 8);
    Patch((BYTE*)healthHookAddy, healthHookAddyBytes, 5);
    Patch((BYTE*)allHealthHookAddy, allHealthHookAddyBytes, 5);
    Patch((BYTE*)manaHookAddy, manaHookAddyBytes, 8);
    Patch((BYTE*)oxygenHookAddy, oxygenHookAddyBytes, 8);
    Patch((BYTE*)ammoHookAddy, ammoHookAddyBytes, 12);
    Patch((BYTE*)goldHookAddy, goldHookAddyBytes, 6);

    if (f) { fclose(f); }
    FreeConsole();
    FreeLibraryAndExitThread(hModule, 0);
    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, nullptr));

    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

