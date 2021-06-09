#include "GameApi.h"

namespace Game
{
	bool AnimateFrontSteerAngle = false;
	void SetTargetFrontSteerAngle(float angle)
	{
		TargetFrontSteerAngle = angle;
		AnimateFrontSteerAngle = true;
	}

	int* GameState = (int*)0xA99BBC;
	float* FrontSteerAngle = (float*)0x00A7B668;
	float TargetFrontSteerAngle = 0;
	float* nosTrailCount = (float*)0x00A732A8;
	int* CarPartSlotMap = (int*)0x00A73398;
	int** CarTypeInfoArray = (int**)0x00B74CCC;
	int* NumRideInfos = (int*)0x00B74CEC;
	int** RideInfoTable = (int**)0x00B74320;
	int*** FrontEndRenderingCarList = (int***)0x00B77970;
	bool* ForceHeadlightsOn = (bool*)0x00B74C14;
	bool* ForceHeadlightsOff = (bool*)0x00B74C18;
	int** PlayerPVehicle = (int**)0x00A9F168;
	int*** PVehicleList = (int***)0x00A9F15C;
	int* PVehicleCount = (int*)0x00A9F164;
	int* CarPartDB = (int*)0x00B76690;
	float* DeltaTime = (float*)0x00A99A5C;
	float** HeadlightFlare = (float**)0x00A6BF88;
	int* FECarRecord = (int*)0x00BBABAC;

	int(__cdecl* StringHash)(const char*) = (int(__cdecl*)(const char*))0x471050;
	int(__cdecl* StringHash1)(const char*, int) = (int(__cdecl*)(const char*, int))0x00471080;
	int(__cdecl* Random)(int) = (int(__cdecl*)(int))0x0046DB30;
	int(__cdecl* GetModelNameHash)(int*, int, int, int) = (int(__cdecl*)(int*, int, int, int))0x007CDA40;
	void* (__thiscall* GetAppliedAttributeIParam)(void*, int, int) = (void* (__thiscall*)(void*, int, int))0x007B00E0;
	int(__thiscall* GetAppliedAttributeIParam1)(void*, int, int) = (int(__thiscall*)(void*, int, int))0x007C27C0;
	char* (__cdecl* GetCarTypeName)(int) = (char* (__cdecl*)(int))0x007B0290;
	void* (__cdecl* bOMalloc)(int) = (void* (__cdecl*)(int))0x00477BE0;
	void* (__cdecl* j_malloc_0)(int) = (void* (__cdecl*)(int))0x006A1560;
	int(__thiscall* AddMenuOption)(void*, int, int, int, int) = (int(__thiscall*)(void*, int, int, int, int))0x85FE30;
	int(__cdecl* StringHashModel)(char*, unsigned int) = (int(__cdecl*)(char*, unsigned int))0x471080;
	int(__cdecl* bSNPrintf)(char*, int, const char*, ...) = (int(__cdecl*)(char*, int, const char*, ...))0x475C30;
	int* (__thiscall* GetPart)(void*, DBPart::_DBPart) = (int* (__thiscall*)(void*, DBPart::_DBPart))0x7B06F0;
	int(__thiscall* GetPartNameHash)(void*) = (int(__thiscall*)(void*))0x7CD930;
	int(__cdecl* InstallPart)(void*, int*, int*, int, int, const char*, ...) = (int(__cdecl*)(void*, int*, int*, int, int, const char*, ...))0x84F040;
	int(__thiscall* UnInstallPart)(void*, int, int) = (int(__thiscall*)(void*, int, int))0x004AE380;
	int(__thiscall* SetPart)(int*, DBPart::_DBPart, int*, int) = (int(__thiscall*)(int*, DBPart::_DBPart, int*, int))0x007D67A0;
	int(__thiscall* FECustomizationRecord_SetInstalledPart)(void*, DBPart::_DBPart, int*, int, char) = (int(__thiscall*)(void*, DBPart::_DBPart, int*, int, char))0x004BAC50;
	bool(__thiscall* IsStock)(void*) = (bool(__thiscall*)(void*))0x7CA040;
	int(__thiscall* StandardSelectablePart_GetCategoryHash)(int*) = (int(__thiscall*)(int*))0x00842670;
	//auto game_sub_7D16A0 = (int(__cdecl*)(int a1, int a2, int a3, int a4, int a5))0x7D16A0;
	//auto game_LoadedWheel = (int(__thiscall*)(void* _this, int a2, char a3))0x007CF880;
	int(__thiscall* HandleSpecialCustomization)(void*, int*, int*) = (int(__thiscall*)(void*, int*, int*))0x008597F0;
	int(__thiscall* GetEmitterPositions)(void*, int*, int*, int*) = (int(__thiscall*)(void*, int*, int*, int*))0x007BEB90;
	int* (__thiscall* GetAttributePointer)(void* _this, int, int) = (int* (__thiscall*)(void* _this, int, int))0x00463A80;
	int(__thiscall* CarRecordGetType)(void* _this) = (int(__thiscall*)(void* _this))0x004AE150;
	int(__thiscall* GetNumCarParts)(void*, int, int, int, int) = (int(__thiscall*)(void*, int, int, int, int))0x007D6660;
	int(__thiscall* PartModelExist)(int, int) = (int(__thiscall*)(int, int))0x0055B670; // dont forget to add lod to hash string
	int(__thiscall* VectorScrollerMenu_AddOption)(int*, int*) = (int(__thiscall*)(int*, int*))0x005B2320;
	int(__thiscall* TextOption_ctor)(int*, int, int, char*, char*) = (int(__thiscall*)(int*, int, int, char*, char*))0x005869C0;
	int* (__thiscall* GetCarPart)(int, int, int, int, int, int*, int) = (int* (__thiscall*)(int, int, int, int, int, int*, int))0x007CDDF0;
	void* (__thiscall* GetCameraScreenName)(int*, char*, char*) = (void* (__thiscall*)(int*, char*, char*))0x00840040;
	int(__thiscall* GetPartHashToSelect)(void*, int, char) = (int(__thiscall*)(void*, int, char))0x0085EE80;
	void(__cdecl* bSafeStrCpy)(char*, char*, int) = (void(__cdecl*)(char*, char*, int))0x00471230;
	int(__stdcall* ShowPartWarning)(int, int) = (int(__stdcall*)(int, int))0x0085F8F0;
	int(__stdcall* InstallExhaustHack)(int*, int*) = (int(__stdcall*)(int*, int*))0x0084F0D0;
	void(__cdecl* SetLanguageHash)(int, int) = (void(__cdecl*)(int, int))0x00583A90;
	int* (__thiscall* FECustomizationRecord_GetInstalledPart)(int*, int, int) = (int* (__thiscall*)(int*, int, int))0x004AE350;
	bool(__thiscall* PVehicle_IsGlareOn)(int*, int) = (bool(__thiscall*)(int*, int))0x6D8080;
	int(__thiscall* eViewPlatInterface_Render)(void*, int, int, int, int, int, int) = (int(__thiscall*)(void*, int, int, int, int, int, int))0x00729320;
	void(__thiscall* eModel_ReplaceLightMaterial)(int, int, int) = (void(__thiscall*)(int, int, int))0x0055C0B0;
	int* (__cdecl* elGetLightMaterial)(int, int) = (int* (__cdecl*)(int, int))0x007473A0;
	int* (__cdecl* GetTextureInfo)(int, int, int) = (int* (__cdecl*)(int, int, int))0x0055CFD0;
	bool(__cdecl* IsPaused)() = (bool(__cdecl*)())0x004A62E0;
	int(__cdecl* AutosculptSelectablePart_ConvertSlotToRegion)(int) = (int(__cdecl*)(int))0x00843180;
	bool(__cdecl* RenderFlare)(int, int, int, float, int, int, int, float, int, float) = (bool(__cdecl*)(int, int, int, float, int, int, int, float, int, float))0x0074D330;

	bool PartMeshExists(int hash)
	{
		return Game::PartModelExist(0x00A8FFF4, hash);
	}
}