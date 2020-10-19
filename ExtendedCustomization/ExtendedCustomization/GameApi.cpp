#include "GameApi.h"

namespace GameApi
{
	int* GameState = (int*)0xA99BBC;
	int* ForceHeadlightsOff = (int*)0x00B74C18;
	float* FrontSteerAngle = (float*)0x00A7B668;
	float* nosTrailCount = (float*)0x00A732A8;

	int(__cdecl* StringHash)(char*) = (int(__cdecl*)(char*))0x471050;
	void* (__thiscall* GetAppliedAttributeIParam)(void*, int, int) = (void* (__thiscall*)(void*, int, int))0x007B00E0;
	int (__thiscall* GetAppliedAttributeIParam1)(void*, int, int) = (int (__thiscall*)(void*, int, int))0x007C27C0;
	char* (__cdecl* GetCarTypeName)(int) = (char* (__cdecl*)(int))0x007B0290;
	int(__thiscall* AddMenuOption)(void*, int, int, int, int) = (int(__thiscall*)(void*, int, int, int, int))0x85FE30;
	int(__cdecl* StringHashModel)(char*, unsigned int) = (int(__cdecl*)(char*, unsigned int))0x471080;
	int(__cdecl* bSNPrintf)(char*, int, const char*, ...) = (int(__cdecl*)(char*, int, const char*, ...))0x475C30;
	void* (__thiscall* GetPart)(void*, int) = (void* (__thiscall*)(void*, int))0x7B06F0;
	int(__thiscall* GetPartNameHash)(void*) = (int(__thiscall*)(void*))0x7CD930;
	int(__cdecl* InstallPart)(void*, int*, int*, int, int, const char*, ...) = (int(__cdecl*)(void*, int*, int*, int, int, const char*, ...))0x84F040;
	int(__thiscall* UnInstallPart)(void*, int, int) = (int(__thiscall*)(void*, int, int))0x004AE380;
	int(__thiscall* SetPart)(void*, int, int, int) = (int(__thiscall*)(void*, int, int, int))0x007D67A0;
	bool(__thiscall* IsStock)(void*) = (bool(__thiscall*)(void*))0x7CA040;
	//auto game_sub_7D16A0 = (int(__cdecl*)(int a1, int a2, int a3, int a4, int a5))0x7D16A0;
	//auto game_LoadedWheel = (int(__thiscall*)(void* _this, int a2, char a3))0x007CF880;
	int(__thiscall* HandleSpecialCustomization)(void*, int*, int*) = (int(__thiscall*)(void*, int*, int*))0x008597F0;
	int* (__thiscall* GetAttributePointer)(void* _this, int, int) = (int* (__thiscall*)(void* _this, int, int))0x00463A80;

	int __stdcall GetPartValue(int* carPtr, int part)
	{
		int val = 0;

		if (carPtr) {
			int* partPtr = carPtr + part;
			if (*partPtr) {
				int* valPtr = (int*)GameApi::GetAppliedAttributeIParam((void*)*partPtr, GameApi::StringHash((char*)"SPINNEROFFSET"), 0);

				if (valPtr) {
					val = *(valPtr + 1);
				}
			}
		}

		return val;
	}
}