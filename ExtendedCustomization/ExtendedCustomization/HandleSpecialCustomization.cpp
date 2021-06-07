#include "Feature.h"

void UninstallPart(int* rideInfo, int* FECustomizationRecord, DBPart::_DBPart part)
{
	if (FECustomizationRecord)
	{
		Game::UnInstallPart(FECustomizationRecord, *rideInfo, part);
	}

	if (rideInfo)
	{
		Game::SetPart(rideInfo, part, 0, 1);
	}
}

void InstallPart(int* rideInfo, int* FECustomizationRecord, DBPart::_DBPart part, int* partPtr)
{
	if (FECustomizationRecord)
	{
		Game::FECustomizationRecord_SetInstalledPart(FECustomizationRecord, part, partPtr, *rideInfo, 0);
	}

	if (rideInfo)
	{
		Game::SetPart(rideInfo, part, partPtr, 1);
	}
}

void HandleRoof(void* _this, int* rideInfo, int* FECustomizationRecord)
{
	auto carData = Config::Get(*rideInfo);
	if (carData->SunRoof)
	{
		char* roofType = (char*)"%s_ROOF_SUN";
		int* roofScoopPart = (int*)Game::GetPart(rideInfo, DBPart::RoofScoop);
		if (roofScoopPart)
		{
			if (Game::GetPartNameHash(roofScoopPart) != Game::StringHash((char*)"ROOF_STYLE00"))
			{
				roofType = (char*)"%s_ROOF";
			}
		}

		Game::InstallPart(_this, rideInfo, FECustomizationRecord, DBPart::Roof, 1, roofType, Game::GetCarTypeName(*rideInfo));
	}
}

void HandleBadging(void* _this, int* rideInfo, int* FECustomizationRecord, const char* position, DBPart::_DBPart partId)
{
	int* body = Game::GetPart(rideInfo, DBPart::Body);
	if (body && !Game::IsStock(body))
	{
		UninstallPart(rideInfo, FECustomizationRecord, DBPart::FrontBadging);
		UninstallPart(rideInfo, FECustomizationRecord, DBPart::RearBadging);
		return;
	}

	if (Config::GetPartState(*rideInfo, partId) == Config::EnabledState)
	{
		int* badgingPart = (int*)Game::GetPart(rideInfo, partId);
		if (!badgingPart)
		{
			int* bodyPart = (int*)Game::GetPart(rideInfo, DBPart::Body);

			if (!bodyPart || Game::IsStock(bodyPart))
			{
				Game::InstallPart(_this, rideInfo, FECustomizationRecord, partId, 1, "%s_KIT00_%s_BUMPER_BADGING_SET", Game::GetCarTypeName(*rideInfo), position);
			}
		}
	}
	else
	{
		int* bumperPart = (int*)Game::GetPart(rideInfo, (DBPart::_DBPart)(partId - 1));
		if (bumperPart)
		{
			int kit = Game::GetAppliedAttributeIParam1(bumperPart, Game::StringHash((char*)"KITNUMBER"), 0);
			Game::InstallPart(_this, rideInfo, FECustomizationRecord, partId, 1, "%s_KIT%02d_%s_BUMPER_BADGING_SET", Game::GetCarTypeName(*rideInfo), kit, position);
		}
	}
}

void SetHeadlightsOff(void* _this, int* rideInfo, int* FECustomizationRecord)
{
	char* carName = Game::GetCarTypeName(*rideInfo);
	Game::InstallPart(_this, rideInfo, FECustomizationRecord, DBPart::LeftHeadlight, 1, "%s_LEFT_HEADLIGHT_OFF", carName);
	Game::InstallPart(_this, rideInfo, FECustomizationRecord, DBPart::RightHeadlight, 1, "%s_RIGHT_HEADLIGHT_OFF", carName);
	Game::InstallPart(_this, rideInfo, FECustomizationRecord, DBPart::RightHeadlightGlass, 1, "%s_RIGHT_HEADLIGHT_GLASS_OFF", carName);
	Game::InstallPart(_this, rideInfo, FECustomizationRecord, DBPart::LeftHeadlightGlass, 1, "%s_LEFT_HEADLIGHT_GLASS_OFF", carName);
}

void SetHeadlightsOn(void* _this, int* rideInfo, int* FECustomizationRecord)
{
	char* carName = Game::GetCarTypeName(*rideInfo);
	Game::InstallPart(_this, rideInfo, FECustomizationRecord, DBPart::LeftHeadlight, 1, "%s_LEFT_HEADLIGHT", carName);
	Game::InstallPart(_this, rideInfo, FECustomizationRecord, DBPart::RightHeadlight, 1, "%s_RIGHT_HEADLIGHT", carName);
	Game::InstallPart(_this, rideInfo, FECustomizationRecord, DBPart::RightHeadlightGlass, 1, "%s_RIGHT_HEADLIGHT_GLASS", carName);
	Game::InstallPart(_this, rideInfo, FECustomizationRecord, DBPart::LeftHeadlightGlass, 1, "%s_LEFT_HEADLIGHT_GLASS", carName);
}

void SetHeadlights(void* _this, int* rideInfo, int* FECustomizationRecord)
{
	char* carName = Game::GetCarTypeName(*rideInfo);
	int* leftHeadlight = Game::GetPart(rideInfo, DBPart::LeftHeadlight);

	if (leftHeadlight)
	{
		int kit = Game::GetAppliedAttributeIParam1(leftHeadlight, Game::StringHash((char*)"KITNUMBER"), 0);
		if (kit)
		{
			Game::InstallPart(_this, rideInfo, FECustomizationRecord, DBPart::RightHeadlight, 1, "%s_KIT%02d_RIGHT_HEADLIGHT", carName, kit);
			Game::InstallPart(_this, rideInfo, FECustomizationRecord, DBPart::RightHeadlightGlass, 1, "%s_KIT%02d_RIGHT_HEADLIGHT_GLASS", carName, kit);
			Game::InstallPart(_this, rideInfo, FECustomizationRecord, DBPart::LeftHeadlightGlass, 1, "%s_KIT%02d_LEFT_HEADLIGHT_GLASS", carName, kit);
		}
		else
		{
			SetHeadlightsOn(_this, rideInfo, FECustomizationRecord);
		}
	}
}

int* GetPartByLodHash(DBPart::_DBPart part, int hash, int* rideInfo)
{
	int* partPtr = 0;

	while (true)
	{
		partPtr = Game::GetCarPart((int)Game::CarPartDB, 0, *rideInfo, part, 0, partPtr, -1);
		if (partPtr)
		{
			if (hash == Game::GetModelNameHash(partPtr, 0, 0, 0))
			{
				return partPtr;
			}
		}
		else
		{
			break;
		}
	}

	return partPtr;
}

void HandleHeadlights(void* _this, int* rideInfo, int* FECustomizationRecord, bool randomCar)
{
	auto carData = Config::Get(*rideInfo);

	if (carData->PopupHeadlights && !randomCar)
	{
		SetHeadlightsOff(_this, rideInfo, FECustomizationRecord);
		return;
	}

	if (Config::GetPartState(*rideInfo, DBPart::LeftHeadlight) == Config::EnabledState)
	{
		SetHeadlights(_this, rideInfo, FECustomizationRecord);
		return;
	}

	int* frontBumper = Game::GetPart(rideInfo, DBPart::FrontBumper);
	if (frontBumper)
	{
		int kit = Game::GetAppliedAttributeIParam1(frontBumper, Game::StringHash((char*)"KITNUMBER"), 0);
		if (kit)
		{
			char* carName = Game::GetCarTypeName(*rideInfo);
			char buffer[100];
			sprintf(buffer, "%s_KIT%02d_LEFT_HEADLIGHT_OFF_A", carName, kit);
			if (Game::PartMeshExists(Game::StringHash(buffer)))
			{
				InstallPart(rideInfo, FECustomizationRecord, DBPart::LeftHeadlight, GetPartByLodHash(DBPart::LeftHeadlight, Game::StringHash(buffer), rideInfo));
				sprintf(buffer, "%s_KIT%02d_RIGHT_HEADLIGHT_OFF_A", carName, kit);
				InstallPart(rideInfo, FECustomizationRecord, DBPart::RightHeadlight, GetPartByLodHash(DBPart::RightHeadlight, Game::StringHash(buffer), rideInfo));
				return;
			}
		}
	}

	SetHeadlightsOn(_this, rideInfo, FECustomizationRecord);
}

void HandleBrakesStock(void* _this, int* rideInfo, int* FECustomizationRecord)
{
	char* carName = Game::GetCarTypeName(*rideInfo);
	int* frontBrake = Game::GetPart(rideInfo, DBPart::FrontBrake);
	int* frontRotor = Game::GetPart(rideInfo, DBPart::FrontRotor);

	if (frontBrake && !frontRotor)
	{
		int kit = Game::GetAppliedAttributeIParam1(frontBrake, Game::StringHash((char*)"KITNUMBER"), 0);
		if (kit)
		{
			Game::InstallPart(_this, rideInfo, FECustomizationRecord, DBPart::FrontRotor, 1, "BRAKES_KIT01_FRONT_ROTOR");
		}
		else
		{
			Game::InstallPart(_this, rideInfo, FECustomizationRecord, DBPart::FrontRotor, 1, "%s_FRONT_ROTOR", carName);
		}
	}

	if (!frontBrake && frontRotor)
	{
		int kit = Game::GetAppliedAttributeIParam1(frontRotor, Game::StringHash((char*)"KITNUMBER"), 0);
		if (kit)
		{
			Game::InstallPart(_this, rideInfo, FECustomizationRecord, DBPart::FrontBrake, 1, "BRAKES_KIT01_FRONT_BRAKE");
		}
		else
		{
			Game::InstallPart(_this, rideInfo, FECustomizationRecord, DBPart::FrontBrake, 1, "%s_FRONT_BRAKE", carName);
		}
	}
}

void HandleBrakes(void* _this, int* rideInfo, int* FECustomizationRecord)
{
	HandleBrakesStock(_this, rideInfo, FECustomizationRecord);
	char* carName = Game::GetCarTypeName(*rideInfo);
	int* frontBrake = Game::GetPart(rideInfo, DBPart::FrontBrake);
	if (frontBrake)
	{
		int kit = Game::GetAppliedAttributeIParam1(frontBrake, Game::StringHash((char*)"KITNUMBER"), 0);
		if (kit)
		{
			Game::InstallPart(_this, rideInfo, FECustomizationRecord, DBPart::RearBrake, 1, "BRAKES_KIT%02d_REAR_BRAKE", kit);
		}
		else
		{
			Game::InstallPart(_this, rideInfo, FECustomizationRecord, DBPart::RearBrake, 1, "%s_REAR_BRAKE", carName);
		}
	}

	int* frontRotor = Game::GetPart(rideInfo, DBPart::FrontRotor);
	if (frontRotor)
	{
		int kit = Game::GetAppliedAttributeIParam1(frontRotor, Game::StringHash((char*)"KITNUMBER"), 0);
		if (kit)
		{
			Game::InstallPart(_this, rideInfo, FECustomizationRecord, DBPart::RearRotor, 1, "BRAKES_KIT%02d_REAR_ROTOR", kit);
		}
		else
		{
			Game::InstallPart(_this, rideInfo, FECustomizationRecord, DBPart::RearRotor, 1, "%s_REAR_ROTOR", carName);
		}
	}
}

void HandleSideMirrors(void* _this, int* rideInfo, int* FECustomizationRecord)
{
	int* leftMirror = Game::GetPart(rideInfo, DBPart::LeftMirror);
	if (leftMirror)
	{
		int kit = Game::GetAppliedAttributeIParam1(leftMirror, Game::StringHash((char*)"KITNUMBER"), 0);
		char* carName = Game::GetCarTypeName(*rideInfo);
		if (kit)
		{
			Game::InstallPart(_this, rideInfo, FECustomizationRecord, DBPart::RightMirror, 1, "%s_KIT%02d_RIGHT_SIDE_MIRROR", carName, kit);
		}
		else
		{
			Game::InstallPart(_this, rideInfo, FECustomizationRecord, DBPart::RightMirror, 1, "%s_RIGHT_SIDE_MIRROR", carName);
		}
	}
}

void HandleBrakelights(void* _this, int* rideInfo, int* FECustomizationRecord)
{
	int* leftBrakelight = Game::GetPart(rideInfo, DBPart::LeftBrakelight);
	if (leftBrakelight)
	{
		int kit = Game::GetAppliedAttributeIParam1(leftBrakelight, Game::StringHash((char*)"KITNUMBER"), 0);
		char* carName = Game::GetCarTypeName(*rideInfo);
		if (kit)
		{
			Game::InstallPart(_this, rideInfo, FECustomizationRecord, DBPart::RightBrakelight, 1, "%s_KIT%02d_RIGHT_BRAKELIGHT", carName, kit);
			Game::InstallPart(_this, rideInfo, FECustomizationRecord, DBPart::LeftBrakelightGlass, 1, "%s_KIT%02d_LEFT_BRAKELIGHT_GLASS", carName, kit);
			Game::InstallPart(_this, rideInfo, FECustomizationRecord, DBPart::RightBrakelightGlass, 1, "%s_KIT%02d_RIGHT_BRAKELIGHT_GLASS", carName, kit);
		}
		else
		{
			Game::InstallPart(_this, rideInfo, FECustomizationRecord, DBPart::RightBrakelight, 1, "%s_RIGHT_BRAKELIGHT", carName);
			Game::InstallPart(_this, rideInfo, FECustomizationRecord, DBPart::LeftBrakelightGlass, 1, "%s_LEFT_BRAKELIGHT_GLASS", carName);
			Game::InstallPart(_this, rideInfo, FECustomizationRecord, DBPart::RightBrakelightGlass, 1, "%s_RIGHT_BRAKELIGHT_GLASS", carName);
		}
	}
}

void HandleExhaust(void* _this, int* rideInfo, int* FECustomizationRecord)
{
	//int* body = Game::GetPart(rideInfo, DBPart::Body);
	//if (body)
	//{
	//	bool useExhaust = Game::GetAppliedAttributeIParam1(body, Game::StringHash("USE_EXHAUST_MARKER"), 0);
	//	if (useExhaust)
	//	{
	//		int* exhaust = Game::GetPart(rideInfo, DBPart::Exhaust);
	//		if (!exhaust)
	//		{
	//			Game::InstallPart(_this, rideInfo, FECustomizationRecord, DBPart::Exhaust, 1, "EXHAUST_STYLE17_LEVEL1");
	//		}
	//	}
	//	else
	//	{
	//		if (!Game::IsStock(body))
	//		{
	//			Game::SetPart(rideInfo, DBPart::Exhaust, 0, 1);
	//		}
	//	}
	//}

	int* rearBumper = Game::GetPart(rideInfo, DBPart::RearBumper);
	if (!rearBumper || !Game::IsStock(rearBumper))
	{
		int* exhaust = Game::GetPart(rideInfo, DBPart::Exhaust);
		if (exhaust && Game::IsStock(exhaust))
		{
			bool em = Game::GetAppliedAttributeIParam1(exhaust, Game::StringHash("EXHAUST_MARKER"), 0);
			if (!em)
			{
				Game::InstallPart(_this, rideInfo, FECustomizationRecord, DBPart::Exhaust, 1, "EXHAUST_STYLE17_LEVEL1");
			}
		}
	}
	else
	{
		bool useExhaust = Game::GetAppliedAttributeIParam1(rearBumper, Game::StringHash("USE_EXHAUST_MARKER"), 0);
		if (!useExhaust)
		{
			char* carName = Game::GetCarTypeName(*rideInfo);
			Game::InstallPart(_this, rideInfo, FECustomizationRecord, DBPart::Exhaust, 1, "%s_KIT00_EXHAUST", carName);
		}
	}

	//Game::InstallExhaustHack(rideInfo, FECustomizationRecord);
}

void InstallDoorLine(void* _this, int* rideInfo, int* FECustomizationRecord,char* carName)
{
	Game::InstallPart(_this, rideInfo, FECustomizationRecord, DBPart::DoorLine, 1, "%s_KIT00_DOORLINE", carName);
}

void HandleBody(void* _this, int* rideInfo, int* FECustomizationRecord)
{
	int* body = Game::GetPart(rideInfo, DBPart::Body);
	if (!body)
	{
		auto carType = *Game::CarTypeInfoArray + 0x34 * *rideInfo;
		char* str;
		if (carType && *(carType + 0x25) == 2)
		{
			str = (char*)"%s_BODY";
		}
		else
		{
			str = (char*)"%s_BODY_KIT00";
		}

		char* carName = Game::GetCarTypeName(*rideInfo);
		Game::InstallPart(_this, rideInfo, FECustomizationRecord, DBPart::Body, 1, str, carName);
		InstallDoorLine(_this, rideInfo, FECustomizationRecord, carName);
	}
	else
	{
		if (Game::IsStock(body))
		{
			char* carName = Game::GetCarTypeName(*rideInfo);
			if (!Game::GetPart(rideInfo, DBPart::FrontBumper))
			{
				Game::InstallPart(_this, rideInfo, FECustomizationRecord, DBPart::FrontBumper, 1, "%s_KIT00_FRONT_BUMPER", carName);
			}

			if (!Game::GetPart(rideInfo, DBPart::RearBumper))
			{
				Game::InstallPart(_this, rideInfo, FECustomizationRecord, DBPart::RearBumper, 1, "%s_KIT00_REAR_BUMPER", carName);
			}

			if (!Game::GetPart(rideInfo, DBPart::Skirt))
			{
				Game::InstallPart(_this, rideInfo, FECustomizationRecord, DBPart::Skirt, 1, "%s_KIT00_SKIRT", carName);
			}

			InstallDoorLine(_this, rideInfo, FECustomizationRecord, carName);
		}
		else
		{
			UninstallPart(rideInfo, FECustomizationRecord, DBPart::DoorLine);
		}
	}
}

void HandleSkirt(void* _this, int* rideInfo, int* FECustomizationRecord)
{
	char* carName = Game::GetCarTypeName(*rideInfo);
	std::string skirtCap = carName;
	skirtCap.append("_KIT00_SKIRT_CAPPED_A");
	if (Game::PartMeshExists(Game::StringHash(skirtCap.c_str())))
	{
		int* rearBumper = Game::GetPart(rideInfo, DBPart::RearBumper);
		int* skirt = Game::GetPart(rideInfo, DBPart::Skirt);
		if (rearBumper && skirt)
		{
			if (Game::IsStock(skirt) && !Game::IsStock(rearBumper))
			{
				Game::InstallPart(_this, rideInfo, FECustomizationRecord, DBPart::Skirt, 1, "%s_KIT00_SKIRT_CAPPED", carName);
			}
			else if (Game::IsStock(skirt) && Game::IsStock(rearBumper))
			{
				Game::InstallPart(_this, rideInfo, FECustomizationRecord, DBPart::Skirt, 1, "%s_KIT00_SKIRT", carName);
				Game::InstallPart(_this, rideInfo, FECustomizationRecord, DBPart::Exhaust, 1, "%s_KIT00_EXHAUST", carName);
			}

			if (!Game::IsStock(skirt) && Game::IsStock(rearBumper))
			{
				UninstallPart(rideInfo, FECustomizationRecord, DBPart::Exhaust);
			}

			if (!Game::IsStock(rearBumper))
			{
				int* exhaust = Game::GetPart(rideInfo, DBPart::Exhaust);
				if (!exhaust)
				{
					Game::InstallPart(_this, rideInfo, FECustomizationRecord, DBPart::Exhaust, 1, "EXHAUST_STYLE17_LEVEL1");
				}
			}
		}
	}
}

void HandleDoors(void* _this, int* rideInfo, int* FECustomizationRecord)
{
	if (Config::GetPartState(*rideInfo, DBPart::LeftDoor) == Config::EnabledState)
	{
		int* leftDoor = Game::GetPart(rideInfo, DBPart::LeftDoor);
		if (leftDoor)
		{
			char* carName = Game::GetCarTypeName(*rideInfo);
			int kit = Game::GetAppliedAttributeIParam1(leftDoor, Game::StringHash((char*)"KITNUMBER"), 0);
			if (kit)
			{
				Game::InstallPart(_this, rideInfo, FECustomizationRecord, DBPart::LeftDoor, 1, "%s_KIT%02d_DOOR_RIGHT", carName, kit);
			}
			else
			{
				Game::InstallPart(_this, rideInfo, FECustomizationRecord, DBPart::RightDoor, 1, "%s_DOOR_RIGHT", carName);
			}
		}
	}
	else
	{
		int* body = Game::GetPart(rideInfo, DBPart::Body);
		if (body)
		{
			int kit = Game::GetAppliedAttributeIParam1(body, Game::StringHash((char*)"KITNUMBER"), 0);
			char* carName = Game::GetCarTypeName(*rideInfo);
			if (kit)
			{
				Game::InstallPart(_this, rideInfo, FECustomizationRecord, DBPart::LeftDoor, 1, "%s_KITW%02d_DOOR_LEFT", carName, kit);
				Game::InstallPart(_this, rideInfo, FECustomizationRecord, DBPart::RightDoor, 1, "%s_KITW%02d_DOOR_RIGHT", carName, kit);
			}
			else
			{
				Game::InstallPart(_this, rideInfo, FECustomizationRecord, DBPart::LeftDoor, 1, "%s_DOOR_LEFT", carName);
				Game::InstallPart(_this, rideInfo, FECustomizationRecord, DBPart::RightDoor, 1, "%s_DOOR_RIGHT", carName);
			}
		}
	}
}

void HandleGenericVinyls(void* _this, int* rideInfo, int* FECustomizationRecord)
{
	int* vinyl = Game::GetPart(rideInfo, DBPart::GenericVinyls);
	if (vinyl && Game::IsStock(vinyl))
	{
		UninstallPart(rideInfo, FECustomizationRecord, DBPart::GenericVinyls);
	}
}

void HandleSpecialCustomization(void* _this, int* rideInfo, int* FECustomizationRecord, bool randomCar = false)
{
	if (!rideInfo)
	{
		return;
	}

	HandleBody(_this, rideInfo, FECustomizationRecord);
	HandleRoof(_this, rideInfo, FECustomizationRecord);
	HandleHeadlights(_this, rideInfo, FECustomizationRecord, randomCar);
	HandleSideMirrors(_this, rideInfo, FECustomizationRecord);
	HandleBrakelights(_this, rideInfo, FECustomizationRecord);
	HandleBadging(_this, rideInfo, FECustomizationRecord, "FRONT", DBPart::FrontBadging);
	HandleBadging(_this, rideInfo, FECustomizationRecord, "REAR", DBPart::RearBadging);
	HandleBrakes(_this, rideInfo, FECustomizationRecord);
	HandleExhaust(_this, rideInfo, FECustomizationRecord);
	HandleSkirt(_this, rideInfo, FECustomizationRecord);
	HandleDoors(_this, rideInfo, FECustomizationRecord);
	HandleGenericVinyls(_this, rideInfo, FECustomizationRecord);
}

void __fastcall HandleSpecialCustomizationCave(void* _this, int param, int* rideInfo, int* FECustomizationRecord)
{
	HandleSpecialCustomization(_this, rideInfo, FECustomizationRecord);
}

int __fastcall IsSetHeadlightOn(int* _this, int param)
{
	int carId = Game::CarRecordGetType(_this);

	auto carData = Config::Get(carId);
	if (carData->PopupHeadlights)
	{
		return 5;
	}

	return 0;
}

char __fastcall StandardSelectablePart_Install(int* _this, int param, int* a2, char a3)
{
	if (!a2)
	{
		a2 = (int*)0x00BBABC0;
	}

	auto part = (DBPart::_DBPart)_this[6];
	int* partPtr = (int*)*(_this + 4);
	int slot = *(a2 + 0x2F);

	if (part == DBPart::Body)
	{
		if (!Game::IsStock(partPtr))
		{
			Game::UnInstallPart(a2, slot, DBPart::FrontBumper);
			Game::UnInstallPart(a2, slot, DBPart::RearBumper);
			Game::UnInstallPart(a2, slot, DBPart::Skirt);
		}
	}

	bool newStock = Game::GetAppliedAttributeIParam1(partPtr, Game::StringHash("KITNUMBER"), 0) == 0;
	if (part == DBPart::FrontBrake)
	{
		int* frontRotor = Game::FECustomizationRecord_GetInstalledPart(a2, slot, DBPart::FrontRotor);
		if (frontRotor)
		{
			bool rotorStock = Game::GetAppliedAttributeIParam1(frontRotor, Game::StringHash("KITNUMBER"), 0) == 0;
			if (rotorStock != newStock)
			{
				Game::UnInstallPart(a2, slot, DBPart::FrontRotor);
			}
		}
	}

	if (part == DBPart::FrontRotor)
	{
		int* frontBrake = Game::FECustomizationRecord_GetInstalledPart(a2, slot, DBPart::FrontBrake);
		if (frontBrake)
		{
			bool brakeStock = Game::GetAppliedAttributeIParam1(frontBrake, Game::StringHash("KITNUMBER"), 0) == 0;
			if (brakeStock != newStock)
			{
				Game::UnInstallPart(a2, slot, DBPart::FrontBrake);
			}
		}
	}

	return Game::FECustomizationRecord_SetInstalledPart(a2, part, partPtr, slot, a3);
}

void InitHandleSpecialCustomization()
{
	injector::MakeCALL(0x0085EAEC, HandleSpecialCustomizationCave, true);

	injector::MakeJMP(0x008420F0, StandardSelectablePart_Install, true);

	injector::MakeCALL(0x00865278, IsSetHeadlightOn, true);
	injector::MakeNOP(0x00865280, 60, true);
}