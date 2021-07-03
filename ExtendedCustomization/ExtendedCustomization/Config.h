#pragma once
#include <string>
#include <vector>
#include "Parts.h"

namespace Settings
{
	enum _Settings : int
	{
		RandomBody = 0,
		RandomBumpers,
		RandomSpoiler,
		RandomSpoilerAS,
		RandomRoofScoop,
		RandomRoofScoopAS,
		RandomHood,
		RandomHoodAS,
		RandomFrontDecals,
		RandomRearDecals,
		RandomGenericVinyls,
		RandomVectorVinyls,
		RandomLicensePlate,
		RandomBrakes,
		RandomNeon,
		RandomTires,
		RandomWheels,
		RandomWheelsAS,
		CheckVanillaMissingParts,
		num_values
	};
}

namespace Config
{
	enum _State :int
	{
		EnabledState = 1,
		DisabledState = 0,
		DefaultState = -1,
	};

	class Part
	{
	public:
		_State State;
		int Header;
		int HeaderAS;
		DBPart::_DBPart DBPart;
		char* Camera;

		Part()
		{
			this->State = _State::DefaultState;
			this->Header = -1;
		}
	};

	class Car
	{
	public:
		std::string _Name;
		std::vector<Part> Parts;
		bool ForceLodA;
		bool ReplaceHeadlightShader;
		bool ReplaceBrakelightShader;
		bool SunRoof;
		bool PopupHeadlights;

		_State Settings[Settings::num_values];

		Part* GetPart(DBPart::_DBPart dbpart);
	};

	class Global : public Car
	{
	public:
		// general
		bool ExhaustFX;
		bool FixWheelMirror;
		bool AllowStockRimsPaint;
		bool FixFrontRearDecals;
		bool AllVinylsMirrorable;
		float DefaultFrontSteerAngle;
		int MainGarage;

		// Mods
		bool TiresMod;
		bool NeonMod;
		bool LicensePlateMod;
		bool CamberMod;
		bool TrackWidthMod;
		bool TireWidthMod;

		// Random parts
		bool RandomEnabled;
	};

	bool Init();
	Car* Get(int carId);
	Car* GetByHash(int hash);
	Global* GetGlobal();
	int GetPartHeader(int carId, DBPart::_DBPart part, bool isAS = false);
	_State GetPartState(int carId, DBPart::_DBPart part);
	char* GetPartCamera(int carId, DBPart::_DBPart dbpart);
	_State GetSetting(int carId, Settings::_Settings setting);
}