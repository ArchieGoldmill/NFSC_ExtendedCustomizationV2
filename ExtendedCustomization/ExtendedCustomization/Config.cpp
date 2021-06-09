#include <stdlib.h>
#include <filesystem>

#include "Config.h"
#include "GameApi.h"
#include "includes/IniReader.h"
#include <string>

namespace fs = std::filesystem;

namespace Config
{
	std::vector<Car*> carList;
	Car* GetCar(int carId)
	{
		char* carName = Game::GetCarTypeName(carId);

		for (Car* i : carList)
		{
			if (carName == i->_Name)
			{
				return i;
			}
		}

		return NULL;
	}

	Car* GetCarByHash(int hash)
	{
		for (Car* i : carList)
		{
			if (hash == Game::StringHash(i->_Name.c_str()))
			{
				return i;
			}
		}

		return NULL;
	}

	Car* GetCarByName(std::string name)
	{
		for (Car* i : carList)
		{
			if (name == i->_Name)
			{
				return i;
			}
		}

		return NULL;
	}

	Global* glc = NULL;
	Car* Get(int carId)
	{
		auto config = GetCar(carId);
		if (config)
		{
			return config;
		}

		return glc;
	}

	Car* GetByHash(int hash)
	{
		auto config = GetCarByHash(hash);
		if (config)
		{
			return config;
		}

		return glc;
	}

	Global* GetGlobal()
	{
		return glc;
	}

	void InitDefaultCars()
	{
		if (!GetCarByName("BRERA"))
		{
			auto car = new Config::Car();
			car->_Name = "BRERA";
			car->SunRoof = true;
			carList.push_back(car);
		}

		if (!GetCarByName("240SX"))
		{
			auto car = new Config::Car();
			car->_Name = "240SX";
			car->PopupHeadlights = true;
			carList.push_back(car);
		}

		if (!GetCarByName("RX7"))
		{
			auto car = new Config::Car();
			car->_Name = "RX7";
			car->PopupHeadlights = true;
			carList.push_back(car);
		}

		if (!GetCarByName("MR2"))
		{
			auto car = new Config::Car();
			car->_Name = "MR2";
			car->PopupHeadlights = true;
			carList.push_back(car);
		}

		if (!GetCarByName("CAMARO"))
		{
			auto car = new Config::Car();
			car->_Name = "CAMARO";
			car->PopupHeadlights = true;
			carList.push_back(car);
		}

		if (!GetCarByName("COROLLA"))
		{
			auto car = new Config::Car();
			car->_Name = "COROLLA";
			car->PopupHeadlights = true;
			carList.push_back(car);
		}

		if (!GetCarByName("CHARGER69"))
		{
			auto car = new Config::Car();
			car->_Name = "CHARGER69";
			car->PopupHeadlights = true;
			carList.push_back(car);
		}
	}

	_State InitState(int value)
	{
		if (value < -1 || value > 1)
		{
			return _State::DefaultState;
		}

		return (_State)value;
	}

	void InitPart(CIniReader& iniReader, Car* cfg, const char* name, DBPart::_DBPart part)
	{
		Part menu;
		menu.State = InitState(iniReader.ReadInteger(name, "Enabled", -1));
		menu.Header = Game::StringHash(iniReader.ReadString(name, "Header", ""));
		menu.HeaderAS = Game::StringHash(iniReader.ReadString(name, "HeaderAS", ""));
		if (menu.HeaderAS == -1)
		{
			menu.HeaderAS = menu.Header;
		}

		menu.DBPart = part;
		menu.Camera = iniReader.ReadString(name, "Camera", "");
		if (strlen(menu.Camera) == 0)
		{
			menu.Camera = NULL;
		}

		cfg->Parts.push_back(menu);
	}

	void InitParts(CIniReader& iniReader, Car* cfg)
	{
		InitPart(iniReader, cfg, "PART_BODY", DBPart::Body);
		InitPart(iniReader, cfg, "PART_HOOD", DBPart::Hood);
		InitPart(iniReader, cfg, "PART_FRONT_WHEEL", DBPart::FrontWheels);
		InitPart(iniReader, cfg, "PART_FRONT_BUMPER", DBPart::FrontBumper);
		InitPart(iniReader, cfg, "PART_REAR_BUMPER", DBPart::RearBumper);
		InitPart(iniReader, cfg, "PART_SKIRT", DBPart::Skirt);
		InitPart(iniReader, cfg, "PART_ROOF_SCOOP", DBPart::RoofScoop);
		InitPart(iniReader, cfg, "PART_SPOILER", DBPart::Spoiler);
		InitPart(iniReader, cfg, "PART_EXHAUST", DBPart::Exhaust);
		InitPart(iniReader, cfg, "PART_ROOF", DBPart::Roof);
		InitPart(iniReader, cfg, "PART_LICENSE_PLATE", DBPart::LicensePlate);
		InitPart(iniReader, cfg, "PART_DECAL_FRONT", DBPart::FrontDecal);
		InitPart(iniReader, cfg, "PART_DECAL_REAR", DBPart::RearDecal);
		InitPart(iniReader, cfg, "PART_FRONT_BUMPER_BADGING", DBPart::FrontBadging);
		InitPart(iniReader, cfg, "PART_REAR_BUMPER_BADGING", DBPart::RearBadging);
		InitPart(iniReader, cfg, "PART_LEFT_SIDE_MIRROR", DBPart::LeftMirror);
		InitPart(iniReader, cfg, "PART_FRONT_BRAKE", DBPart::FrontBrake);
		InitPart(iniReader, cfg, "PART_FRONT_ROTOR", DBPart::FrontRotor);
		InitPart(iniReader, cfg, "PART_INTERIOR", DBPart::Interior);
		InitPart(iniReader, cfg, "PART_GENERIC_VINYLS", DBPart::GenericVinyls);
		InitPart(iniReader, cfg, "PART_LEFT_HEADLIGHT", DBPart::LeftHeadlight);
		InitPart(iniReader, cfg, "PART_LEFT_BRAKELIGHT", DBPart::LeftBrakelight);
		InitPart(iniReader, cfg, "PART_DRIVER", DBPart::Driver);
		InitPart(iniReader, cfg, "PART_STEERING_WHEEL", DBPart::SteeringWheel);
		InitPart(iniReader, cfg, "PART_DOOR_LEFT", DBPart::LeftDoor);

		for (int i = 0; i < 16; i++)
		{
			char buffer[32];
			sprintf(buffer, "PART_ATTACHMENT%02d", i);
			InitPart(iniReader, cfg, buffer, (DBPart::_DBPart)(DBPart::Attachment0 + i));
		}
	}

	void InitCar(CIniReader& iniReader, Car* cfg)
	{
		InitParts(iniReader, cfg);
		cfg->ForceLodA = iniReader.ReadInteger("GENERAL", "ForceLodA", 0) == 1;
		cfg->ReplaceHeadlightShader = iniReader.ReadInteger("GENERAL", "ReplaceHeadlightShader", 0) == 1;
		cfg->ReplaceBrakelightShader = iniReader.ReadInteger("GENERAL", "ReplaceBrakelightShader", 0) == 1;
		cfg->PopupHeadlights = iniReader.ReadInteger("GENERAL", "PopUpHeadLights", 0) == 1;
	}

	void InitCars()
	{
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
			CIniReader iniReader(path.string().c_str());

			auto carData = new Car();
			carData->_Name = path.filename().string();
			InitCar(iniReader, carData);

			carList.push_back(carData);
		}
	}

	void InitGlobal()
	{
		glc = new Global();
		CIniReader iniReader("ExtendedCustomization.ini");
		InitCar(iniReader, glc);

		glc->ExhaustFX = iniReader.ReadInteger("GENERAL", "ExhaustFX", 0) == 1;
		glc->FixWheelMirror = iniReader.ReadInteger("GENERAL", "FixWheelMirror", 0) == 1;
		glc->AllowStockRimsPaint = iniReader.ReadInteger("GENERAL", "AllowStockRimsPaint", 0) == 1;
		glc->FixFrontRearDecals = iniReader.ReadInteger("GENERAL", "FixFrontRearDecals", 0) == 1;
		glc->AllVinylsMirrorable = iniReader.ReadInteger("GENERAL", "AllVinylsMirrorable ", 0) == 1;
		*Game::FrontSteerAngle = glc->DefaultFrontSteerAngle = iniReader.ReadInteger("GENERAL", "FrontSteerAngle", 0);

		glc->NeonMod = iniReader.ReadInteger("MODS", "Neon", 0) == 1;
		glc->TiresMod = iniReader.ReadInteger("MODS", "Tires", 0) == 1;
		glc->LicensePlateMod = iniReader.ReadInteger("MODS", "LicensePlate", 0) == 1;
		glc->CamberMod = iniReader.ReadInteger("MODS", "Camber", 0) == 1;
		glc->TrackWidthMod = iniReader.ReadInteger("MODS", "TrackWidth", 0) == 1;
		glc->TireWidthMod = iniReader.ReadInteger("MODS", "TireWidth", 0) == 1;

		glc->RandomEnabled = iniReader.ReadInteger("RANDOM_PARTS", "Enabled", 0) == 1;
		glc->Settings[Settings::RandomBody] = InitState(iniReader.ReadInteger("RANDOM_PARTS", "Body", -1));
		glc->Settings[Settings::RandomWheels] = InitState(iniReader.ReadInteger("RANDOM_PARTS", "Wheels", -1));
		glc->Settings[Settings::RandomHood] = InitState(iniReader.ReadInteger("RANDOM_PARTS", "Hood", -1));
		glc->Settings[Settings::RandomHoodAS] = InitState(iniReader.ReadInteger("RANDOM_PARTS", "HoodAS", -1));
		glc->Settings[Settings::RandomRoofScoop] = InitState(iniReader.ReadInteger("RANDOM_PARTS", "RoofScoop", -1));
		glc->Settings[Settings::RandomRoofScoopAS] = InitState(iniReader.ReadInteger("RANDOM_PARTS", "RoofScoopAS", -1));
		glc->Settings[Settings::RandomSpoiler] = InitState(iniReader.ReadInteger("RANDOM_PARTS", "Spoiler", -1));
		glc->Settings[Settings::RandomSpoilerAS] = InitState(iniReader.ReadInteger("RANDOM_PARTS", "SpoilerAS", -1));
		glc->Settings[Settings::RandomGenericVinyls] = InitState(iniReader.ReadInteger("RANDOM_PARTS", "GenericVinyls", -1));
		glc->Settings[Settings::RandomVectorVinyls] = InitState(iniReader.ReadInteger("RANDOM_PARTS", "VectorVinyls", -1));
		glc->Settings[Settings::RandomFrontDecals] = InitState(iniReader.ReadInteger("RANDOM_PARTS", "FrontDecals", -1));
		glc->Settings[Settings::RandomRearDecals] = InitState(iniReader.ReadInteger("RANDOM_PARTS", "RearDecals", -1));
		glc->Settings[Settings::RandomLicensePlate] = InitState(iniReader.ReadInteger("RANDOM_PARTS", "LicensePlate", -1));
		glc->Settings[Settings::RandomNeon] = InitState(iniReader.ReadInteger("RANDOM_PARTS", "Neon", -1));
		glc->Settings[Settings::RandomTires] = InitState(iniReader.ReadInteger("RANDOM_PARTS", "Tires", -1));
		glc->Settings[Settings::RandomBrakes] = InitState(iniReader.ReadInteger("RANDOM_PARTS", "Brakes", -1));
		glc->Settings[Settings::CheckVanillaMissingParts] = InitState(iniReader.ReadInteger("RANDOM_PARTS", "CheckVanillaMissingParts", -1));
	}

	bool Init()
	{
		CIniReader iniReader("NFSCarbon.WidescreenFix.ini");
		if (iniReader.ReadInteger("MISC", "CarShadowFix", 0) != 0)
		{
			MessageBoxA(NULL, "Please set CarShadowFix to 0 in NFSCarbon.WidescreenFix.ini", "NFSC - Extended Customization", MB_ICONERROR);
			return false;
		}

		InitGlobal();
		InitCars();
		InitDefaultCars();
		return true;
	}

	Part* Car::GetPart(DBPart::_DBPart dbpart)
	{
		for (auto& part : this->Parts)
		{
			if (part.DBPart == dbpart)
			{
				return &part;
			}
		}

		return NULL;
	}

	int GetPartHeader(int carId, DBPart::_DBPart dbpart, bool isAS)
	{
		auto carConfig = GetCar(carId);
		if (carConfig)
		{
			auto part = carConfig->GetPart(dbpart);
			if (part)
			{
				int header = isAS ? part->HeaderAS : part->Header;
				if (header != -1)
				{
					return header;
				}
			}
		}

		auto part = glc->GetPart(dbpart);
		if (part)
		{
			return isAS ? part->HeaderAS : part->Header;
		}

		return -1;
	}

	_State GetPartState(int carId, DBPart::_DBPart dbpart)
	{
		auto carConfig = GetCar(carId);
		if (carConfig)
		{
			auto part = carConfig->GetPart(dbpart);
			if (part)
			{
				if (part->State != _State::DefaultState)
				{
					return part->State;
				}
			}
		}

		auto part = glc->GetPart(dbpart);
		if (part)
		{
			return part->State;
		}

		return _State::DisabledState;
	}

	_State GetSetting(int carId, Settings::_Settings setting)
	{
		auto carConfig = GetCar(carId);
		if (carConfig)
		{
			auto st = carConfig->Settings[setting];
			if (st != _State::DefaultState)
			{
				return st;
			}
		}

		return glc->Settings[setting];
	}

	char* GetPartCamera(int carId, DBPart::_DBPart dbpart)
	{
		auto carConfig = GetCar(carId);
		if (carConfig)
		{
			auto part = carConfig->GetPart(dbpart);
			if (part)
			{
				if (part->Camera)
				{
					return part->Camera;
				}
			}
		}

		auto part = glc->GetPart(dbpart);
		if (part)
		{
			return part->Camera;
		}

		return NULL;
	}
}