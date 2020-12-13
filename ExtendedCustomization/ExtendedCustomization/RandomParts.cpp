#include "RandomParts.h"
#include "Feature.h"
#include "HandleSpecialCustomization.h"

struct MissingPart
{
	short CarId;
	short Part;
	int Hash;
};

bool IsPartMissing(DBPart::_DBPart part, int* rideInfo, int* partPtr)
{
	auto missingPart = (MissingPart*)0x00A73A30;

	while ((int)missingPart < 0xA7741C)
	{
		if (missingPart->CarId == *rideInfo && missingPart->Part == part && missingPart->Hash == Game::GetPartNameHash(partPtr))
		{
			return true;
		}

		missingPart++;
	}

	return false;
}

bool CheckGroupVinyl(DBPart::_DBPart part, int* rideInfo, int* partPtr)
{
	int hash = Game::GetAppliedAttributeIParam1(partPtr, Game::StringHash((char*)"GROUPLANGUAGEHASH"), 0);
	if (hash == Game::StringHash((char*)"CUST_VINYL_GROUP_BONUS"))
	{
		return false;
	}

	if (hash == Game::StringHash((char*)"CUST_VINYL_GROUP_SPECIAL"))
	{
		return false;
	}

	if (hash == Game::StringHash((char*)"CUST_VINYL_GROUP_CAMOUFLAGE"))
	{
		return false;
	}

	return true;
}

bool IsAustosculpt(DBPart::_DBPart part, int* rideInfo, int* partPtr)
{
	return Game::GetAppliedAttributeIParam1(partPtr, Game::StringHash((char*)"MORPHTARGET_NUM"), 0) > 0;
}

typedef bool(BoolValueCallback)(DBPart::_DBPart part, int* rideInfo, int* partPtr);

int GetNumCarParts(DBPart::_DBPart part, int* rideInfo, bool allowSculpt, bool checkVinyl)
{
	int count = 0;
	int* partPtr = 0;

	while (true)
	{
		partPtr = Game::GetCarPart(0x00B76690, 0, *rideInfo, part, 0, partPtr, -1);
		if (partPtr)
		{
			if (IsPartMissing(part, rideInfo, partPtr))
			{
				continue;
			}

			if (!allowSculpt && IsAustosculpt(part, rideInfo, partPtr))
			{
				continue;
			}

			if (checkVinyl && CheckGroupVinyl(part, rideInfo, partPtr))
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

int* SetRandomPart(DBPart::_DBPart part, int* rideInfo, bool allowSculpt = false, bool checkVinyl = false)
{
	int n = GetNumCarParts(part, rideInfo, allowSculpt, checkVinyl);
	if (n == 0)
	{
		return 0;
	}

	int rnd = Game::Random(n) + 1;

	int* partPtr = 0;
	while (true)
	{
		partPtr = Game::GetCarPart(0x00B76690, 0, *rideInfo, part, 0, partPtr, -1);

		if (IsPartMissing(part, rideInfo, partPtr))
		{
			continue;
		}

		if (!allowSculpt && IsAustosculpt(part, rideInfo, partPtr))
		{
			continue;
		}

		if (checkVinyl && CheckGroupVinyl(part, rideInfo, partPtr))
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
		Game::SetPart(rideInfo, part, partPtr, 1);
	}

	return partPtr;
}

void __fastcall SetRandomParts(int* rideInfo, int param, int hash)
{
	auto config = Config::GetGlobal();
	auto carType = *Game::CarTypeInfoArray + 0x34 * *rideInfo;
	char* str;
	if (carType && *(carType + 0x25) == 2)
	{
		return;
	}

	if (config->RandomBody)
	{
		//if (Game::Random(2))
		if (true)
		{
			int* boydPart = SetRandomPart(DBPart::Body, rideInfo, true);
			if (boydPart && !Game::IsStock(boydPart))
			{
				Game::SetPart(rideInfo, DBPart::FrontBumper, 0, 1);
				Game::SetPart(rideInfo, DBPart::RearBumper, 0, 1);
				Game::SetPart(rideInfo, DBPart::Skirt, 0, 1);
			}
		}
		else
		{
			int* frontBumper = SetRandomPart(DBPart::FrontBumper, rideInfo, true);
			if (frontBumper)
			{
				int kit = Game::GetAppliedAttributeIParam1(frontBumper, Game::StringHash((char*)"KITNUMBER"), 0);
				Game::InstallPart(NULL, rideInfo, NULL, DBPart::RearBumper, 1, "%s_KIT%02d_REAR_BUMPER", Game::GetCarTypeName(*rideInfo), kit);
				Game::InstallPart(NULL, rideInfo, NULL, DBPart::Skirt, 1, "%s_KIT%02d_SKIRT", Game::GetCarTypeName(*rideInfo), kit);
			}
		}
	}

	if (!Game::GetPart(rideInfo, DBPart::GenericVinyls))
	{
		if (config->RandomGenericVinyls)
		{			
			if (Game::Random(2))
			{
				SetRandomPart(DBPart::GenericVinyls, rideInfo);
			}
		}

		if (config->RandomVectorVinyls && !Game::GetPart(rideInfo, DBPart::GenericVinyls))
		{
			if (Game::Random(2))
			{
				SetRandomPart(DBPart::VectorVinyls, rideInfo, false, true);
			}
		}
	}

	if (config->RandomFrontDecals)
	{
		if (Game::Random(2))
		{
			SetRandomPart(DBPart::FrontDecal, rideInfo, true);
			if (config->RandomRearDecals)
			{
				if (Game::Random(2))
				{
					SetRandomPart(DBPart::RearDecal, rideInfo, true);
				}
			}
		}
	}

	if (config->RandomWheels)
	{
		SetRandomPart(DBPart::FrontWheels, rideInfo, true);
	}

	if (config->RandomHood)
	{
		SetRandomPart(DBPart::Hood, rideInfo, true);
	}

	if (config->RandomSpoiler)
	{
		SetRandomPart(DBPart::Spoiler, rideInfo);
	}

	if (config->RandomBrakes)
	{
		SetRandomPart(DBPart::FrontBrake, rideInfo);
		SetRandomPart(DBPart::FrontRotor, rideInfo);
	}

	if (config->RandomTires)
	{
		SetRandomPart(DBPart::Attachment14, rideInfo);
	}

	if (config->RandomNeon)
	{
		SetRandomPart(DBPart::Attachment15, rideInfo);
	}

	if (config->RandomLicensePlate)
	{
		SetRandomPart(DBPart::LicensePlate, rideInfo);
	}

	if (config->RandomRoofScoop)
	{
		if (Game::Random(2))
		{
			SetRandomPart(DBPart::RoofScoop, rideInfo);
		}
	}

	HandleSpecialCustomization(NULL, 0, rideInfo, NULL);
}

void InitRandomParts()
{
	auto config = Config::GetGlobal();
	if (config->RandomEnabled)
	{
		injector::MakeCALL(0x00629D69, SetRandomParts, true);
	}
}