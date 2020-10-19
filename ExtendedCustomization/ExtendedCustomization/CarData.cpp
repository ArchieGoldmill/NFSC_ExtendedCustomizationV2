#include <stdlib.h>
#include <filesystem>

#include "CarData.h"
#include "includes/IniReader.h"
#include "GameApi.h"

using namespace std;
namespace fs = filesystem;

vector<CarData*> carList;
void AddDefaultCars()
{
	CarData* car_240SX = new CarData();
	car_240SX->Name = (char*)"240SX";
	car_240SX->PopUpHeadLights = 1;

	car_240SX->FrontBadging = 1;

	CarData* car_RX7 = new CarData();
	car_RX7->Name = (char*)"RX7";
	car_RX7->PopUpHeadLights = 1;

	CarData* car_MR2 = new CarData();
	car_MR2->Name = (char*)"MR2";
	car_MR2->PopUpHeadLights = 1;

	CarData* car_CAMARO = new CarData();
	car_CAMARO->Name = (char*)"CAMARO";
	car_CAMARO->PopUpHeadLights = 1;

	CarData* car_COROLLA = new CarData();
	car_COROLLA->Name = (char*)"COROLLA";
	car_COROLLA->PopUpHeadLights = 1;

	CarData* car_CHARGER69 = new CarData();
	car_CHARGER69->Name = (char*)"CHARGER69";
	car_CHARGER69->PopUpHeadLights = 1;

	CarData* car_BRERA = new CarData();
	car_BRERA->Name = (char*)"BRERA";
	car_BRERA->Roof = 1;

	carList.push_back(car_240SX);
	carList.push_back(car_RX7);
	carList.push_back(car_MR2);
	carList.push_back(car_CAMARO);
	carList.push_back(car_COROLLA);
	carList.push_back(car_CHARGER69);
	carList.push_back(car_BRERA);
}

CarData* CheckIfExists(string& name)
{
	for (CarData* i : carList)
	{
		if (name == i->Name)
		{
			return i;
		}
	}

	return NULL;
}

void LoadCarData()
{
	AddDefaultCars();

	std::string path = "ExtendedCustomizationCars";
	fs::directory_iterator iterator;
	try
	{
		iterator = fs::directory_iterator(path);
	}
	catch (...)
	{
		return;
	}

	for (const auto& entry : iterator)
	{
		auto path = entry.path();
		auto carNameStr = path.filename().string();

		CIniReader iniReader(path.string().c_str());

		CarData* carData = new CarData();
		carData->PopUpHeadLights = iniReader.ReadInteger((char*)"GENERAL", (char*)"PopUpHeadLights", 0);
		carData->Roof = iniReader.ReadInteger((char*)"GENERAL", (char*)"Roof", 0);
		carData->ChopTop = iniReader.ReadInteger((char*)"GENERAL", (char*)"ChopTop", 0);
		carData->IneriorHI = iniReader.ReadInteger((char*)"GENERAL", (char*)"IneriorHI", 0);
		carData->ForceLodA = iniReader.ReadInteger((char*)"GENERAL", (char*)"ForceLodA", 0);
		carData->FrontBadging = iniReader.ReadInteger((char*)"GENERAL", (char*)"FrontBadging", 0);
		carData->RearBadging = iniReader.ReadInteger((char*)"GENERAL", (char*)"RearBadging", 0);
		carData->HeadlightsOnOffTexture = iniReader.ReadInteger((char*)"GENERAL", (char*)"HeadlightsOnOffTexture", 0);
		carData->Name = carNameStr;

		CarData* exising = CheckIfExists(carData->Name);

		if (exising != NULL)
		{
			*exising = *carData;
			delete carData;
		}
		else
		{
			carList.push_back(carData);
		}
	}
}

CarData* GetCarData(int carId)
{
	char* carName = GameApi::GetCarTypeName(carId);

	for (CarData* i : carList)
	{
		if (carName == i->Name)
		{
			return i;
		}
	}

	return NULL;
}

//int __stdcall CheckCarData(IniOption type, int carId)
//{
//	char* carName = GameApi::GetCarTypeName(carId);
//
//	for (CarData* i : carList)
//	{
//		if (strcmp(carName, i->Name) != 0)
//		{
//			continue;
//		}
//
//		if (type == IniOption::_PopUpHeadLights)
//		{
//			return i->PopUpHeadLights;
//		}
//
//		if (type == IniOption::_Roof)
//		{
//			return i->Roof;
//		}
//
//		if (type == IniOption::_ChopTop)
//		{
//			return i->ChopTop;
//		}
//
//		if (type == IniOption::_IneriorHI)
//		{
//			return i->IneriorHI;
//		}
//
//		if (type == IniOption::_ForceLodA)
//		{
//			return i->ForceLodA;
//		}
//
//		if (type == IniOption::_FrontBadging)
//		{
//			return i->FrontBadging;
//		}
//
//		if (type == IniOption::_RearBadging)
//		{
//			return i->RearBadging;
//		}
//
//		if (type == IniOption::_HeadlightsOnOffTexture)
//		{
//			return i->HeadlightsOnOffTexture;
//		}
//	}
//
//	return 0;
//}