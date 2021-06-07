#include <windows.h>

#include "ExhaustFX.h"
#include "SpecialCustomization.h"
#include "Menu.h"
#include "ForceLodA.h"
#include "TextureReplace.h"
#include "Fixes.h"
#include "Neon.h"
#include "WheelAdjustments.h"
#include "Config.h"
#include "HeadlightsColor.h"

void Init()
{
	if (!Config::Init())
	{
		return;
	}

	InitExhaustFX();
	InitSpecialCustomization();
	InitMenu();
	InitForceLodA();
	InitTextureReplace();
	InitWheelAdjustments();
	//InitHeadlightsColor();
	FixFrontRearDecals();
	InitNeon();
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		uintptr_t base = (uintptr_t)GetModuleHandleA(NULL);
		IMAGE_DOS_HEADER* dos = (IMAGE_DOS_HEADER*)(base);
		IMAGE_NT_HEADERS* nt = (IMAGE_NT_HEADERS*)(base + dos->e_lfanew);

		if ((base + nt->OptionalHeader.AddressOfEntryPoint + (0x400000 - base)) == 0x87E926) // Check if .exe file is compatible - Thanks to thelink2012 and MWisBest
		{
			Init();
		}
		else
		{
			MessageBoxA(NULL, "This .exe is not supported.\nPlease use v1.4 English nfsc.exe (6,88 MB (7.217.152 bytes)).", "NFSC - Extended Customization", MB_ICONERROR);
			return FALSE;
		}
	}
	break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
