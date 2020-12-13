#pragma once
#include "Parts.h"

namespace Game
{
	extern int* GameState;
	extern float* FrontSteerAngle;
	extern float* nosTrailCount;
	extern int* CarPartSlotMap;
	extern int** CarTypeInfoArray;
	extern int* NumRideInfos;
	extern int** RideInfoTable;
	extern int*** FrontEndRenderingCarList;
	extern bool* ForceHeadlightsOn;
	extern bool* ForceHeadlightsOff;
	extern int** PlayerPVehicle;
	extern int*** PVehicleList;
	extern int* PVehicleCount;

	extern int(__cdecl* StringHash)(const char* str);
	extern int(__cdecl* StringHash1)(const char*, int hash);
	extern int(__cdecl* Random)(int);
	extern int(__cdecl* GetModelNameHash)(int* carPart, int, int, int);
	extern void* (__thiscall* GetAppliedAttributeIParam)(void* _this, int hash, int v1);
	extern int(__thiscall* GetAppliedAttributeIParam1)(void* _this, int hash, int v1);
	extern char* (__cdecl* GetCarTypeName)(int);
	extern void* (__cdecl* bOMalloc)(int);
	extern int(__thiscall* AddMenuOption)(void* _this, int name, int part, int, int);
	extern int(__cdecl* StringHashModel)(char* a1, unsigned int a2);
	extern int(__cdecl* bSNPrintf)(char* buffer, int size, const char* str, ...);
	extern int* (__thiscall* GetPart)(void* rideInfo, DBPart::_DBPart partNum);
	extern int(__thiscall* GetPartNameHash)(void* _this);
	extern int(__cdecl* InstallPart)(void* _this, int* RideInfo, int* FECustomizationRecord, int partNum, int a5, const char* str, ...);
	extern int(__thiscall* UnInstallPart)(void* FECustomizationRecord, int carId, int partId);
	extern int(__thiscall* SetPart)(int* RideInfo, DBPart::_DBPart partId, int* part, int);
	extern int(__thiscall* FECustomizationRecord_SetInstalledPart)(void*, DBPart::_DBPart, int*, int, char);
	extern bool(__thiscall* IsStock)(void* _this);
	extern int(__thiscall* StandardSelectablePart_GetCategoryHash)(int* _this);
	//auto game_sub_7D16A0 = (int(__cdecl*)(int a1, int a2, int a3, int a4, int a5))0x7D16A0;
	//auto game_LoadedWheel = (int(__thiscall*)(void* _this, int a2, char a3))0x007CF880;
	extern int(__thiscall* HandleSpecialCustomization)(void* _this, int* RideInfo, int* FECustomizationRecord);
	extern int(__thiscall* GetEmitterPositions)(void*, int*, int*, int*);
	extern int* (__thiscall* GetAttributePointer)(void* _this, int, int);
	extern int(__thiscall* CarRecordGetType)(void* _this);
	extern int(__thiscall* GetNumCarParts)(void* _this, int CarType, int, int, int);
	extern int(__thiscall* PartModelExist)(int, int hash);
	extern int(__thiscall* VectorScrollerMenu_AddOption)(int* _this, int*);
	extern int(__thiscall* TextOption_ctor)(int*, int, int, char*, char*);
	extern int* (__thiscall* GetCarPart)(int _this, int, int, int, int, int* prev, int);
	extern void* (__thiscall* GetCameraScreenName)(int*, char*, char*);
	extern void(__cdecl* bSafeStrCpy)(char*, char*, int);
	extern int(__stdcall* ShowPartWarning)(int, int);
	extern int(__thiscall* GetPartHashToSelect)(void*, int, char);
	extern int(__stdcall* InstallExhaustHack)(int*, int*);
	extern void(__cdecl* SetLanguageHash)(int, int);
	extern int* (__thiscall* FECustomizationRecord_GetInstalledPart)(int* _this, int, int);
	extern bool(__thiscall* PVehicle_IsGlareOn)(int* PVehicle, int VehicleFXID);
	extern int(__thiscall* eViewPlatInterface_Render)(void* _this, int a2, int a3, int a4, int a5, int a6, int a7);
	extern void(__thiscall* eModel_ReplaceLightMaterial)(int, int, int);
	extern int* (__cdecl* elGetLightMaterial)(int, int);
	extern int* (__cdecl* GetTextureInfo)(int, int, int);

	int __stdcall GetPartValue(int* carPtr, int part);
}