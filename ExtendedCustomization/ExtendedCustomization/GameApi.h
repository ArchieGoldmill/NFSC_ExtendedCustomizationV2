#pragma once

namespace GameApi
{
	extern int* GameState;
	extern int* ForceHeadlightsOff;
	extern float* FrontSteerAngle;
	extern float* nosTrailCount;

	extern int(__cdecl* StringHash)(char* str);
	extern void* (__thiscall* GetAppliedAttributeIParam)(void* _this, int hash, int v1);
	extern int (__thiscall* GetAppliedAttributeIParam1)(void* _this, int hash, int v1);
	extern char* (__cdecl* GetCarTypeName)(int);
	extern int(__thiscall* AddMenuOption)(void* _this, int name, int part, int, int);
	extern int(__cdecl* StringHashModel)(char* a1, unsigned int a2);
	extern int(__cdecl* bSNPrintf)(char* buffer, int size, const char* str, ...);
	extern void* (__thiscall* GetPart)(void* carNum, int partNum);
	extern int(__thiscall* GetPartNameHash)(void* _this);
	extern int(__cdecl* InstallPart)(void* _this, int* RideInfo, int* FECustomizationRecord, int partNum, int a5, const char* str, ...);
	extern int(__thiscall* UnInstallPart)(void* FECustomizationRecord, int carId, int partId);
	extern int(__thiscall* SetPart)(void* RideInfo, int partId, int, int);
	extern bool(__thiscall* IsStock)(void* _this);
	//auto game_sub_7D16A0 = (int(__cdecl*)(int a1, int a2, int a3, int a4, int a5))0x7D16A0;
	//auto game_LoadedWheel = (int(__thiscall*)(void* _this, int a2, char a3))0x007CF880;
	extern int(__thiscall* HandleSpecialCustomization)(void* _this, int* RideInfo, int* FECustomizationRecord);
	extern int* (__thiscall* GetAttributePointer)(void* _this, int, int);

	int __stdcall GetPartValue(int* carPtr, int part);
}