#pragma once
#include <string>

enum IniOption
{
	_PopUpHeadLights,
	_ForceLodA,
	_IneriorHI,
	_Roof,
	_ChopTop,
	_FrontBadging,
	_RearBadging,
	_HeadlightsOnOffTexture
};

struct CarData
{
	std::string Name;

	int PopUpHeadLights = 0; // disabled, popups, customizible

	bool ForceLodA = false;

	bool IneriorHI = false;

	int Roof = 0; // default, sun roof, roof menu option

	int ChopTop = 0; // default, enabled, disabled

	bool FrontBadging = false;

	bool RearBadging = false;

	bool HeadlightsOnOffTexture = false;
};

void LoadCarData();

CarData* GetCarData(int carId);