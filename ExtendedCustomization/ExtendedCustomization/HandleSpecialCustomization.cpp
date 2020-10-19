#include "Feature.h"

void HandleRoof(CarData* carData, void* _this, int* rideInfo, int* FECustomizationRecord)
{
	char* roofType = (char*)"%s_ROOF";
	if (carData && carData->Roof == 1)
	{
		int* roofScoopPart = (int*)GameApi::GetPart(rideInfo, 0x4D);
		if (roofScoopPart)
		{
			if (GameApi::GetPartNameHash(roofScoopPart) == GameApi::StringHash((char*)"ROOF_STYLE00"))
			{
				roofType = (char*)"%s_ROOF_SUN";
			}
		}
	}

	GameApi::InstallPart(_this, rideInfo, FECustomizationRecord, 0x4C, 1, roofType, GameApi::GetCarTypeName(*rideInfo));
}

void HandleBadging(CarData* carData, void* _this, int* rideInfo, int* FECustomizationRecord, const char* position, int partId, bool custom)
{
	if (custom)
	{
		int* badgingPart = (int*)GameApi::GetPart(rideInfo, partId);
		if (!badgingPart)
		{
			int* bodyPart = (int*)GameApi::GetPart(rideInfo, 0x17);

			if (!bodyPart || GameApi::IsStock(bodyPart))
			{
				GameApi::InstallPart(_this, rideInfo, FECustomizationRecord, partId, 1, "%s_KIT00_%s_BUMPER_BADGING_SET", GameApi::GetCarTypeName(*rideInfo), position);
			}
		}
	}
	else
	{
		int* bumperPart = (int*)GameApi::GetPart(rideInfo, partId - 1);
		if (bumperPart)
		{
			int kit = GameApi::GetAppliedAttributeIParam1(bumperPart, GameApi::StringHash((char*)"KITNUMBER"), 0);
			GameApi::InstallPart(_this, rideInfo, FECustomizationRecord, partId, 1, "%s_KIT%02d_%s_BUMPER_BADGING_SET", GameApi::GetCarTypeName(*rideInfo), kit, position);
		}
	}
}

void __fastcall HandleSpecialCustomization(void* _this, int param, int* rideInfo, int* FECustomizationRecord)
{
	GameApi::HandleSpecialCustomization(_this, rideInfo, FECustomizationRecord);

	if (_this && rideInfo && FECustomizationRecord)
	{
		CarData* carData = GetCarData(*rideInfo);
		HandleRoof(carData, _this, rideInfo, FECustomizationRecord);
		HandleBadging(carData, _this, rideInfo, FECustomizationRecord, "FRONT", 0x49, carData && carData->FrontBadging);
		HandleBadging(carData, _this, rideInfo, FECustomizationRecord, "REAR", 0x4B, carData && carData->RearBadging);
	}
}

void __stdcall SetPartsOnGarageExit(int* _this)
{

}

DWORD GarageDestructorCaveExit = 0x865455;
void __declspec(naked) GarageDestructorCave()
{
	__asm
	{
		pushad;
		push esi;
		call SetPartsOnGarageExit;
		popad;

		jmp GarageDestructorCaveExit;
	}
}

void InitHandleSpecialCustomization()
{
	// Disable set HEADLIGHT_OFF
	injector::MakeJMP(0x00859817, 0x008598ED, true);

	// Disable SUN_ROOF
	injector::MakeJMP(0x008598F0, 0x00859947, true);

	// Disable FRONT_BADGING
	injector::MakeJMP(0x00859C89, 0x00859D02, true);

	// Disable REAR_BADGING
	injector::MakeJMP(0x00859969, 0x008599D5, true);

	injector::MakeCALL(0x0085EAEC, HandleSpecialCustomization, true);

	injector::WriteMemory<char>(0x00865280, 0xEB, true);
	injector::MakeJMP(0x00865278, GarageDestructorCave, true);
}