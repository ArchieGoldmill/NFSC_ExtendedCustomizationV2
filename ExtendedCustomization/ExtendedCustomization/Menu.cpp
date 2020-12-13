#include "Menu.h"
#include "Feature.h"
#include "Parts.h"
#include "Config.h"

int* CurrentCarPtr = (int*)0x00B74320;

int* GetRideInfo()
{
	return Game::RideInfoTable[0];
}

int* GetRenderInfo()
{
	return *(*Game::FrontEndRenderingCarList + 0x2FE);
}

void AddMainMenuItem(int* _this, int hash, int num)
{
	int* textOption = (int*)malloc(0x98);
	char a = 1, b = 0;
	Game::TextOption_ctor(textOption, hash, num, &a, &b);
	Game::VectorScrollerMenu_AddOption(_this, textOption);
}

void __stdcall AddMainMenuItems(int* _this)
{
	*Game::FrontSteerAngle = 0;
	int* rideInfo = GetRideInfo();

	for (int i = 0; i < 12; i++)
	{
		if (Config::GetPartState(*rideInfo, (DBPart::_DBPart)(DBPart::Attachment0 + i)) == Config::EnabledState)
		{
			AddMainMenuItem(_this, Game::StringHash("CUST_MAINMENU_ATTACHMENTS"), 5);
			break;
		}
	}

	AddMainMenuItem(_this, Game::StringHash("CUST_MAINMENU_SPECIALTIES"), 6);
}

void __declspec(naked) AddMainMenuItemsCave()
{
	static constexpr auto Exit = 0x0083FBF6;
	__asm
	{
		pushad;
		push esi;
		call AddMainMenuItems;
		popad;

		push 0x00000098;
		jmp Exit;
	}
}

int lastMenu = 0;
int __stdcall ShowMenu(int menu)
{
	lastMenu = menu;
	if (menu >= 5)
	{
		return 0;
	}

	return menu;
}

void __declspec(naked) OpenCustomMenuCave()
{
	static constexpr auto Exit = 0x008683CF;
	__asm
	{
		mov eax, [esp + 0x60];
		pushad;
		push eax;
		call ShowMenu;
		test eax, eax;
		popad;
		jmp Exit;
	}
}

void AddMenuItem(char* _this, DBPart::_DBPart dbpart, bool isAutosculpt)
{
	int filter = 0;
	if (dbpart == DBPart::FrontWheels)
	{
		if (isAutosculpt)
		{
			filter = 17;
		}
		else
		{
			filter = *(_this + 0x2D5);
		}
	}

	int* rideInfo = GetRideInfo();
	auto state = Config::GetPartState(*rideInfo, dbpart);
	if (state == Config::DefaultState)
	{
		if (dbpart == DBPart::Roof && isAutosculpt)
		{
			int* racingClassPtr = Game::GetAttributePointer((void*)0x00BBABB0, 0x247E0956, 0);
			if (racingClassPtr && *racingClassPtr == 2)
			{
				state = Config::EnabledState;
			}
		}
	}

	if (state == Config::EnabledState)
	{
		Game::AddMenuOption(_this, Config::GetPartHeader(*rideInfo, dbpart), dbpart, isAutosculpt, filter);
	}
}

void __stdcall AddMenuItems(char* _this)
{
	bool isAutosculpt = *(_this + 0x2D4);

	if (isAutosculpt)
	{
		Game::SetLanguageHash(*(int*)(_this + 0x2C8), 0x663D2A11);

		AddMenuItem(_this, DBPart::FrontWheels, isAutosculpt);
		AddMenuItem(_this, DBPart::Hood, isAutosculpt);
		AddMenuItem(_this, DBPart::FrontBumper, isAutosculpt);
		AddMenuItem(_this, DBPart::RearBumper, isAutosculpt);
		AddMenuItem(_this, DBPart::Skirt, isAutosculpt);
		AddMenuItem(_this, DBPart::RoofScoop, isAutosculpt);
		AddMenuItem(_this, DBPart::Spoiler, isAutosculpt);
		AddMenuItem(_this, DBPart::Exhaust, isAutosculpt);
		AddMenuItem(_this, DBPart::Roof, isAutosculpt);
	}
	else
	{
		Game::SetLanguageHash(*(int*)(_this + 0x2C8), 0xAC258B54);
		if (lastMenu == 0)
		{
			AddMenuItem(_this, DBPart::Body, isAutosculpt);
			AddMenuItem(_this, DBPart::Hood, isAutosculpt);
			AddMenuItem(_this, DBPart::RoofScoop, isAutosculpt);
			AddMenuItem(_this, DBPart::Spoiler, isAutosculpt);
			AddMenuItem(_this, DBPart::FrontWheels, isAutosculpt);
			AddMenuItem(_this, DBPart::Roof, isAutosculpt);
			AddMenuItem(_this, DBPart::Interior, isAutosculpt);
			AddMenuItem(_this, DBPart::FrontBadging, isAutosculpt);
			AddMenuItem(_this, DBPart::RearBadging, isAutosculpt);
			AddMenuItem(_this, DBPart::LeftHeadlight, isAutosculpt);
			AddMenuItem(_this, DBPart::LeftBrakelight, isAutosculpt);
			AddMenuItem(_this, DBPart::LeftMirror, isAutosculpt);
		}

		if (lastMenu == 5)
		{
			for (int i = 0; i < 12; i++)
			{
				AddMenuItem(_this, (DBPart::_DBPart)(DBPart::Attachment0 + i), isAutosculpt);
			}
		}

		if (lastMenu == 6)
		{
			AddMenuItem(_this, DBPart::LicensePlate, isAutosculpt);
			AddMenuItem(_this, DBPart::FrontBrake, isAutosculpt);
			AddMenuItem(_this, DBPart::FrontRotor, isAutosculpt);
			for (int i = 0; i < 4; i++)
			{
				AddMenuItem(_this, (DBPart::_DBPart)(DBPart::Attachment12 + i), isAutosculpt);
			}

			AddMenuItem(_this, DBPart::FrontDecal, isAutosculpt);
			AddMenuItem(_this, DBPart::RearDecal, isAutosculpt);
			AddMenuItem(_this, DBPart::Driver, isAutosculpt);
			AddMenuItem(_this, DBPart::SteeringWheel, isAutosculpt);
			AddMenuItem(_this, DBPart::GenericVinyls, isAutosculpt);
		}
	}
}

void __declspec(naked) AddMenuItemsCave()
{
	static constexpr auto Exit = 0x00866235;
	__asm
	{
		pushad;
		push esi;
		call AddMenuItems;
		popad;
		jmp Exit;
	}
}

int __fastcall StandardSelectablePart_GetCategoryHash(int* _this)
{
	int* partPtr = _this + 6;
	auto part = (DBPart::_DBPart) * partPtr;
	int* rideInfo = GetRideInfo();
	int header = Config::GetPartHeader(*rideInfo, part);
	if (header != -1)
	{
		return header;
	}

	return Game::StandardSelectablePart_GetCategoryHash(_this);
}

void __fastcall GetCameraScreenName(int* _this, int dummy, char* a2, int a3)
{
	if (a3 > 0)
	{
		if (*(_this + 0xB8) && *(_this + 0xB1) < 0x00A99C60)
		{
			*Game::FrontSteerAngle = 0;
			auto part = DBPart::Invalid;

			int* partPtr = (int*)*(_this + 0xD);
			if (partPtr)
			{
				part = (DBPart::_DBPart) * (partPtr + 0xC);
			}

			char* menu = NULL;
			if (part == DBPart::FrontBumper)
			{
				menu = (char*)"FeAutosculptFrontBumper.fng";
			}

			if (part == DBPart::RearBumper)
			{
				menu = (char*)"FeAutosculptRearBumper.fng";
			}

			if (part == DBPart::Skirt)
			{
				menu = (char*)"FeAutosculptSkirt.fng";
			}

			if (part == DBPart::Exhaust)
			{
				menu = (char*)"FeAutosculptExhaust.fng";
			}

			if (part == DBPart::Hood)
			{
				menu = (char*)"FeAutosculptHood.fng";
			}

			if (part == DBPart::RoofScoop)
			{
				menu = (char*)"FeAutosculptRoof.fng";
			}

			if (part == DBPart::Spoiler)
			{
				menu = (char*)"FeAutosculptSpoiler.fng";
			}

			if (part == DBPart::FrontWheels)
			{
				menu = (char*)"FeAutosculptWheel.fng";
			}

			if (part == DBPart::Roof)
			{
				menu = (char*)"FeAutosculptTop.fng";
			}

			if (part == DBPart::Body)
			{
				menu = (char*)"FeAutosculptBodyKit.fng";
			}

			if (part == DBPart::FrontBadging)
			{
				menu = (char*)"FeFrontBadging.fng";
			}

			if (part == DBPart::RearBadging)
			{
				menu = (char*)"FeRearBadging.fng";
			}

			if (part == DBPart::Interior)
			{
				menu = (char*)"FeInterior.fng";
			}

			if (part == DBPart::LeftMirror)
			{
				menu = (char*)"FeLeftMirror.fng";
			}

			if (part == DBPart::LicensePlate)
			{
				menu = (char*)"FeLicensePlate.fng";
			}

			if (part == DBPart::FrontBrake || part == DBPart::FrontRotor)
			{
				menu = (char*)"FeFrontBrake.fng";
			}

			if (part == DBPart::FrontDecal)
			{
				menu = (char*)"FeFrontDecal.fng";
			}

			if (part == DBPart::RearDecal)
			{
				menu = (char*)"FeRearDecal.fng";
			}

			if ((part >= DBPart::Attachment0 && part <= DBPart::Attachment15) || part == DBPart::GenericVinyls)
			{
				menu = (char*)"FeAttachment.fng";
			}

			auto config = Config::GetGlobal();
			if (config->TiresMod)
			{
				if (part == DBPart::Attachment14)
				{
					*Game::FrontSteerAngle = -20.0;
					menu = (char*)"FeTires.fng";
				}
			}

			if (config->NeonMod)
			{
				if (part == DBPart::Attachment15)
				{
					menu = (char*)"FeNeon.fng";
				}
			}

			if (config->CamberMod)
			{
				if (part == DBPart::Attachment12)
				{
					menu = (char*)"FeCamber.fng";
				}
			}

			if (config->TrackWidthMod)
			{
				if (part == DBPart::Attachment13)
				{
					menu = (char*)"FeCamber.fng";
				}
			}

			if (menu)
			{
				strcpy_s(a2, a3, menu);

				return;
			}
		}
	}

	*a2 = 0;
}

bool HasExhaustMountPoint()
{
	auto renderInfo = GetRenderInfo();
	return *(renderInfo + 0x10D) || *(renderInfo + 0x10E) || *(renderInfo + 0x10F);
}

bool IsBodyStock()
{
	int hash = Game::GetPartHashToSelect((void*)0x00BBABA8, DBPart::Body, 1);
	return hash == Game::StringHash("PART_BODY_00") || hash == Game::StringHash("PART_BODYKIT_00");
}

int __stdcall PartWarning(DBPart::_DBPart part, int a2)
{
	bool show = false;
	if (part == DBPart::FrontBadging || part == DBPart::RearBadging || part == DBPart::FrontBumper || part == DBPart::RearBumper || part == DBPart::Skirt)
	{
		show = !IsBodyStock();
	}

	if (part == DBPart::Exhaust)
	{
		auto renderInfo = GetRenderInfo();
		show = !HasExhaustMountPoint();
	}

	if (show)
	{
		return Game::ShowPartWarning(0x22105094, a2);
	}

	return 0;
}

void __declspec(naked) PartWarningCave()
{
	static constexpr auto Exit123 = 0x0085F7BD;
	__asm
	{
		pushad;
		push ebx;
		push eax;
		call PartWarning;
		test eax, eax;
		popad;
		je noWarn;
		pop edi;
		pop esi;
		pop ebx;
		ret 0x0014;

	noWarn:
		jmp Exit123;
	}
}

int __stdcall DisableParts(int header)
{
	int* rideInfo = GetRideInfo();
	if (header == Game::StringHash("CUST_PARTS_EXHAUST"))
	{
		if (!HasExhaustMountPoint())
		{
			return 1;
		}
	}

	if (header == Config::GetPartHeader(*rideInfo, DBPart::FrontBadging) || header == header == Config::GetPartHeader(*rideInfo, DBPart::RearBadging))
	{
		if (!IsBodyStock())
		{
			return 1;
		}
	}

	if (header == Config::GetPartHeader(*rideInfo, DBPart::FrontBumper)
		|| header == Config::GetPartHeader(*rideInfo, DBPart::RearBumper)
		|| header == Config::GetPartHeader(*rideInfo, DBPart::Skirt))
	{
		if (!IsBodyStock())
		{
			return 1;
		}
	}

	return 0;
}

void __declspec(naked) DisablePartsCave()
{
	static constexpr auto cExit = 0x00866638;
	static constexpr auto cExit1 = 0x008665FC;
	__asm
	{
		pushad;
		mov eax, [ecx + 0x2C];
		push eax;
		call DisableParts;
		test eax, eax;
		popad;
		je enableParts;
		jmp cExit1;

	enableParts:
		jmp cExit;
	}
}

void InitMenu()
{
	*Game::FrontSteerAngle = 0;

	injector::MakeJMP(0x0083FBF1, AddMainMenuItemsCave, true);
	injector::MakeJMP(0x008683C9, OpenCustomMenuCave, true);
	injector::MakeJMP(0x00866074, AddMenuItemsCave, true);
	injector::MakeJMP(0x0085F74C, PartWarningCave, true);
	injector::MakeJMP(0x008665DA, DisablePartsCave, true);

	injector::MakeNOP(0x0085F90E, 5, true);
	injector::WriteMemory(0x0085F90E, 0x182474FF, true);

	injector::WriteMemory(0x009F9D14, StandardSelectablePart_GetCategoryHash, true);
	injector::MakeCALL(0x0084EB1F, GetCameraScreenName, true);
}