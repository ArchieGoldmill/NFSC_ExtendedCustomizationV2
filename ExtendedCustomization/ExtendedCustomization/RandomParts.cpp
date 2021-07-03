#include "RandomParts.h"
#include "Feature.h"
#include "HandleSpecialCustomization.h"

struct MissingPart
{
	short CarId;
	short Part;
	int Hash;
};

bool IsPartMissing(DBPart::_DBPart part, int carId, int* partPtr)
{
	auto missingPart = (MissingPart*)0x00A73A30;

	while ((int)missingPart < 0xA7741C)
	{
		if (missingPart->CarId == carId && missingPart->Part == part && missingPart->Hash == Game::GetPartNameHash(partPtr))
		{
			return true;
		}

		missingPart++;
	}

	return false;
}

bool CheckGroupVinyl(int* partPtr)
{
	int hash = Game::GetAppliedAttributeIParam1(partPtr, Game::StringHash((char*)"GROUPLANGUAGEHASH"), 0);
	if (hash == Game::StringHash((char*)"CUST_VINYL_GROUP_BONUS"))
	{
		return true;
	}

	if (hash == Game::StringHash((char*)"CUST_VINYL_GROUP_SPECIAL"))
	{
		return true;
	}

	if (hash == Game::StringHash((char*)"CUST_VINYL_GROUP_CAMOUFLAGE"))
	{
		return true;
	}

	return false;
}

bool IsAustosculpt(int* partPtr)
{
	return Game::GetAppliedAttributeIParam1(partPtr, Game::StringHash((char*)"MORPHTARGET_NUM"), 0) > 0;
}

bool CheckPart(int carId, DBPart::_DBPart part, int* partPtr, int autosculpt)
{
	if (part == DBPart::VectorVinyls)
	{
		return CheckGroupVinyl(partPtr);
	}

	if (Config::GetSetting(carId, Settings::CheckVanillaMissingParts) == Config::EnabledState)
	{
		if (IsPartMissing(part, carId, partPtr))
		{
			return false;
		}
	}

	if (autosculpt == 0)
	{
		return true;
	}

	bool isAS = IsAustosculpt(partPtr);
	return autosculpt == -1 ? isAS : !isAS;
}

int GetNumCarParts(DBPart::_DBPart part, int carId, int autosculpt)
{
	int count = 0;
	int* partPtr = 0;

	while (true)
	{
		partPtr = Game::GetCarPart((int)Game::CarPartDB, 0, carId, part, 0, partPtr, -1);
		if (partPtr)
		{
			if (!CheckPart(carId, part, partPtr, autosculpt))
			{
				continue;
			}

			count++;
		}
		else
		{
			break;
		}
	}

	return count;
}

int* SetRandomPart1(DBPart::_DBPart part, int* rideInfo, int autosculpt = 0)
{
	int n = GetNumCarParts(part, *rideInfo, autosculpt);
	if (n == 0)
	{
		return 0;
	}

	int rnd = Game::Random(n) + 1;

	int* partPtr = 0;
	while (true)
	{
		partPtr = Game::GetCarPart((int)Game::CarPartDB, 0, *rideInfo, part, 0, partPtr, -1);

		if (!CheckPart(*rideInfo, part, partPtr, autosculpt))
		{
			continue;
		}

		rnd--;
		if (rnd == 0)
		{
			break;
		}
	}

	if (partPtr)
	{
		//Game::SetPart(rideInfo, part, partPtr, 1);
	}

	return partPtr;
}

void __fastcall SetRandomParts1(RideInfo* rideInfo, int param, int hash)
{
	//auto config = Config::GetGlobal();
	//auto carType = *Game::CarTypeInfoArray + 0x34 * *rideInfo;
	//if (carType && *(carType + 0x25) == 2)
	//{
	//	return;
	//}

	//if (config->RandomBody)
	//{
	//	//if (Game::Random(2))
	//	if (true)
	//	{
	//		int* boydPart = SetRandomPart(DBPart::Body, rideInfo, true);
	//		if (boydPart && !Game::IsStock(boydPart))
	//		{
	//			Game::SetPart(rideInfo, DBPart::FrontBumper, 0, 1);
	//			Game::SetPart(rideInfo, DBPart::RearBumper, 0, 1);
	//			Game::SetPart(rideInfo, DBPart::Skirt, 0, 1);
	//		}
	//	}
	//	else
	//	{
	//		int* frontBumper = SetRandomPart(DBPart::FrontBumper, rideInfo, true);
	//		if (frontBumper)
	//		{
	//			int kit = Game::GetAppliedAttributeIParam1(frontBumper, Game::StringHash((char*)"KITNUMBER"), 0);
	//			Game::InstallPart(NULL, rideInfo, NULL, DBPart::RearBumper, 1, "%s_KIT%02d_REAR_BUMPER", Game::GetCarTypeName(*rideInfo), kit);
	//			Game::InstallPart(NULL, rideInfo, NULL, DBPart::Skirt, 1, "%s_KIT%02d_SKIRT", Game::GetCarTypeName(*rideInfo), kit);
	//		}
	//	}
	//}

	//if (!Game::GetPart(rideInfo, DBPart::GenericVinyls))
	//{
	//	if (config->RandomGenericVinyls)
	//	{
	//		if (Game::Random(2))
	//		{
	//			SetRandomPart(DBPart::GenericVinyls, rideInfo);
	//		}
	//	}

	//	if (config->RandomVectorVinyls && !Game::GetPart(rideInfo, DBPart::GenericVinyls))
	//	{
	//		if (Game::Random(2))
	//		{
	//			SetRandomPart(DBPart::VectorVinyls, rideInfo, false, true);
	//		}
	//	}
	//}

	//if (config->RandomFrontDecals)
	//{
	//	if (Game::Random(2))
	//	{
	//		SetRandomPart(DBPart::FrontDecal, rideInfo, true);
	//		if (config->RandomRearDecals)
	//		{
	//			if (Game::Random(2))
	//			{
	//				SetRandomPart(DBPart::RearDecal, rideInfo, true);
	//			}
	//		}
	//	}
	//}

	//if (config->RandomWheels)
	//{
	//	SetRandomPart(DBPart::FrontWheels, rideInfo, true);
	//}

	//if (config->RandomHood)
	//{
	//	SetRandomPart(DBPart::Hood, rideInfo, true);
	//}

	//if (config->RandomSpoiler)
	//{
	//	SetRandomPart(DBPart::Spoiler, rideInfo);
	//}

	//if (config->RandomBrakes)
	//{
	//	SetRandomPart(DBPart::FrontBrake, rideInfo);
	//	SetRandomPart(DBPart::FrontRotor, rideInfo);
	//}

	//if (config->RandomTires)
	//{
	//	SetRandomPart(DBPart::Attachment14, rideInfo);
	//}

	//if (config->RandomNeon)
	//{
	//	SetRandomPart(DBPart::Attachment15, rideInfo);
	//}

	//if (config->RandomLicensePlate)
	//{
	//	SetRandomPart(DBPart::LicensePlate, rideInfo);
	//}

	//if (config->RandomRoofScoop)
	//{
	//	if (Game::Random(2))
	//	{
	//		SetRandomPart(DBPart::RoofScoop, rideInfo);
	//	}
	//}

	//HandleSpecialCustomization(NULL, rideInfo, NULL, true);
}

std::vector<int*> GetPartList(RideInfo* rideInfo, DBPart::_DBPart part)
{
	std::vector<int*> parts;

	int* partPtr = 0;
	while (true)
	{
		partPtr = Game::GetCarPart((int)Game::CarPartDB, 0, rideInfo->CarId, part, 0, partPtr, -1);
		if (partPtr)
		{
			if (!IsPartMissing(part, rideInfo->CarId, partPtr) && !IsAustosculpt(partPtr) && !Game::IsStock(partPtr))
			{
				parts.push_back(partPtr);
			}
		}
		else
		{
			break;
		}
	}

	return parts;
}

int* SetRandomPart(RideInfo* rideInfo, DBPart::_DBPart part)
{
	auto parts = GetPartList(rideInfo, part);
	if (parts.size() == 0)
	{
		return NULL;
	}

	int rnd = Game::Random(parts.size());
	if (rnd > parts.size() - 1)
	{
		rnd = parts.size() - 1;
	}

	if (rnd < 0)
	{
		rnd = 0;
	}

	auto partPtr = parts[rnd];
	Game::SetPart(rideInfo, part, partPtr, 1);
	return partPtr;
}

void __fastcall SetRandomParts(RideInfo* rideInfo, int param, int hash)
{
	SetRandomPart(rideInfo, DBPart::LicensePlate);
	SetRandomPart(rideInfo, DBPart::FrontWheels);
	SetRandomPart(rideInfo, DBPart::Spoiler);
	SetRandomPart(rideInfo, DBPart::FrontDecal);
	SetRandomPart(rideInfo, DBPart::RearDecal);
	SetRandomPart(rideInfo, DBPart::Attachment14);
	SetRandomPart(rideInfo, DBPart::Attachment15);
	SetRandomPart(rideInfo, DBPart::Hood);

	if (!Game::GetPart(rideInfo, DBPart::GenericVinyls))
	{
		if (Game::Random(2))
		{
			SetRandomPart(rideInfo, DBPart::GenericVinyls);
		}
	}

	int* bodyPart = SetRandomPart(rideInfo, DBPart::Body);
	if (bodyPart && !Game::IsStock(bodyPart))
	{
		Game::SetPart(rideInfo, DBPart::FrontBumper, 0, 1);
		Game::SetPart(rideInfo, DBPart::RearBumper, 0, 1);
		Game::SetPart(rideInfo, DBPart::Skirt, 0, 1);
		Game::SetPart(rideInfo, DBPart::Exhaust, 0, 1);
	}

	rideInfo->RideHeight = 2.0f;
}

void InitRandomParts()
{
	auto config = Config::GetGlobal();
	//if (config->RandomEnabled)
	{
		injector::MakeCALL(0x00629D69, SetRandomParts, true);
	}
}