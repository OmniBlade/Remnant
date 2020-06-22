/**
 * @file
 *
 * @author OmniBlade
 *
 * @brief Function to hook all required functions.
 *
 * @copyright Remnant is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "setuphooks.h"
#include "alloc.h"
#include "cmdline.h"
#include "hooker.h"
#include "listener.h"
#include <malloc.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void Setup_Hooks()
{
#ifdef __WATCOMC__
    // Disable the DirectDraw based terrain caching system.
    //Make_Global<BOOL>(0x0060BA6C) = false; // IconCacheAllowed
    // Patch out the check for another sole window.
    unsigned in = 0x004C0641;
    unsigned char cmd = 0xEB;
    WriteProcessMemory(GetCurrentProcess(), (LPVOID)in, &cmd, 1, nullptr);
    //replaces watcom's exception handler with ours
    //Hook_Function(0x005DF696, Watcom_Exception_Handler);

    //
    // Hook WinMain
    //
    //Hook_Function(0x00551A70, Main_Func);

    // Hooking memory allocation functions.
    Hook_Function(0x005044C6, &malloc);
    Hook_Function(0x004F5D13, &free);
    Hook_Function(0x0050CFFE, &realloc);
    Hook_Function(0x00509840, &Alloc);
    Hook_Function(0x005098B0, &Free);
    Hook_Function(0x005098E0, &Resize_Alloc);

    // Hook command line parsing to allow modifying globals from command line.
    Hook_Function(0x0047CE94, &Parse_Command_Line);

    
    Hook_Function(0x004F28BC, *ListenerProtocolClass::Connection_Requested);
    Hook_Function(0x0050244B, *ListenerClass::Open_Socket);
    Hook_Function(0x00502537, &ListenerClass::Window_Proc);
#endif
}

#if defined __WATCOMC__ && defined GAME_DLL
// Watcom has no static_assert so we are forced to use this instead
// Watcom throws "dimension cannot be negative" on missmatches
#define ASSERT_SIZEOF(structname, expectedsize) typedef char __ASSERT_SIZEOF__[(sizeof(structname) == expectedsize) ? 1 : -1]

#pragma message("Checking Type Sizes!")
//
#pragma message("Done Checking Type Sizes!")
#endif
