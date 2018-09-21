// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"

#include "HookDll.h"


HINSTANCE g_hInstance = NULL;
HHOOK g_hhk;
HWND g_hWnd;


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
	g_hInstance = hModule;
	g_hWnd = FindWindow(NULL, _T("HookTest35791"));

    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		HookOn();
		break;
    case DLL_THREAD_ATTACH:
		break;
    case DLL_THREAD_DETACH:
		break;
    case DLL_PROCESS_DETACH:
		HookOff();
        break;
    }
    return TRUE;
}



