#include "Menu.h"
#include "Feature.h"
#include "Parts.h"
#include "Config.h"

DBPart::_DBPart SpecialtiesMenu[12] = { DBPart::LicensePlate, DBPart::FrontBrake, DBPart::FrontRotor, DBPart::Attachment12,
										DBPart::Attachment13, DBPart::Attachment14, DBPart::Attachment15, DBPart::FrontDecal,
										DBPart::RearDecal, DBPart::Driver, DBPart::SteeringWheel, DBPart::GenericVinyls };

DBPart::_DBPart MiscMenu[12] = { DBPart::LeftDoor, DBPart::FrontBrake, DBPart::FrontRotor, DBPart::Attachment12,
								 DBPart::Attachment13, DBPart::Attachment14, DBPart::Attachment15, DBPart::FrontDecal,
								 DBPart::RearDecal, DBPart::Driver, DBPart::SteeringWheel, DBPart::GenericVinyls };

int GetCarId()
{
	return Game::CarRecordGetType((void*)*Game::FECarRecord);
}

int* GetRenderInfo()
{
	return *(*Game::FrontEndRenderingCarList + 0x2FE);
}

void AddMainMenuItem(int* _this, int hash, int num)
{
	int* textOption = (int*)Game::j_malloc_0(0x98);
	char a = 1, b = 0;
	Game::TextOption_ctor(textOption, hash, num, &a, &b);
	Game::VectorScrollerMenu_AddOption(_this, textOption);
}

void __stdcall AddMainMenuItems(int* _this)
{
	Game::SetTargetFrontSteerAngle(Config::GetGlobal()->DefaultFrontSteerAngle);
	int carId = GetCarId();

	for (int i = 0; i < 12; i++)
	{
		if (Config::GetPartState(carId, (DBPart::_DBPart)(DBPart::Attachment0 + i)) == Config::EnabledState)
		{
			AddMainMenuItem(_this, Game::StringHash("CUST_MAINMENU_ATTACHMENTS"), 5);
			break;
		}
	}

	for (int i = 0; i < 12; i++)
	{
		if (Config::GetPartState(carId, SpecialtiesMenu[i]) == Config::EnabledState)
		{
			AddMainMenuItem(_this, Game::StringHash("CUST_MAINMENU_SPECIALTIES"), 6);
			break;
		}
	}

	//for (int i = 0; i < 12; i++)
	//{
	//	if (Config::GetPartState(*rideInfo, MiscMenu[i]) == Config::EnabledState)
	//	{
	//		AddMainMenuItem(_this, Game::StringHash("CUST_MAINMENU_MISC"), 7);
	//		break;
	//	}
	//}
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

	int carId = GetCarId();
	auto state = Config::GetPartState(carId, dbpart);
	if (dbpart == DBPart::Roof && isAutosculpt)
	{
		int* racingClassPtr = Game::GetAttributePointer((void*)0x00BBABB0, 0x247E0956, 0);
		if (racingClassPtr && *racingClassPtr == 2)
		{
			state = Config::EnabledState;
		}
	}

	if (state == Config::EnabledState)
	{
		Game::AddMenuOption(_this, Config::GetPartHeader(carId, dbpart, isAutosculpt), dbpart, isAutosculpt, filter);
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
			for (int i = 0; i < 12; i++)
			{
				AddMenuItem(_this, SpecialtiesMenu[i], isAutosculpt);
			}
		}

		if (lastMenu == 7)
		{
			for (int i = 0; i < 12; i++)
			{
				AddMenuItem(_this, MiscMenu[i], isAutosculpt);
			}
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
	int header = Config::GetPartHeader(GetCarId(), part);
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
			int* partPtr = (int*)*(_this + 0xD);
			if (partPtr)
			{
				Game::SetTargetFrontSteerAngle(Config::GetGlobal()->DefaultFrontSteerAngle);
				auto part = (DBPart::_DBPart) * (partPtr + 0xC);
				auto config = Config::GetGlobal();
				if (config->TiresMod && part == DBPart::Attachment14)
				{
					Game::SetTargetFrontSteerAngle(-20.0);
				}

				if (config->CamberMod && part == DBPart::Attachment12)
				{
					Game::SetTargetFrontSteerAngle(0);
				}

				if (config->TrackWidthMod && part == DBPart::Attachment13)
				{
					Game::SetTargetFrontSteerAngle(0);
				}

				char* menu = Config::GetPartCamera(GetCarId(), part);
				if (menu)
				{
					strcpy_s(a2, a3, menu);
					return;
				}
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
	int carId = GetCarId();
	if (header == Config::GetPartHeader(carId, DBPart::Exhaust))
	{
		if (!HasExhaustMountPoint())
		{
			return 1;
		}
	}

	if (header == Config::GetPartHeader(carId, DBPart::FrontBadging) || header == header == Config::GetPartHeader(carId, DBPart::RearBadging))
	{
		if (!IsBodyStock())
		{
			return 1;
		}
	}

	if (header == Config::GetPartHeader(carId, DBPart::FrontBumper)
		|| header == Config::GetPartHeader(carId, DBPart::RearBumper)
		|| header == Config::GetPartHeader(carId, DBPart::Skirt))
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

struct PartSlot
{
	PartSlot* Prev;
	PartSlot* Next;
	int Hash;
	void* PartPtr;
	int Name;
	DBPart::_DBPart PartId;
	int Region;
	int Val;
};

void GetPartsList(DBPart::_DBPart partId, PartSlot* ListHead, char carbon, int brandName, int innerRadius, bool autosculpt)
{
	int* partPtr = 0;

	int i = 0;
	while (true)
	{
		partPtr = Game::GetCarPart((int)Game::CarPartDB, 0x009D1954, GetCarId(), partId, 0, partPtr, -1);
		if (partPtr)
		{
			if (Game::GetAppliedAttributeIParam1(partPtr, Game::StringHash((char*)"STOCK"), 0) == 0)
			{
				// Check if part has mesh
				DBPart::_DBPart meshParts[] = { DBPart::Hood, DBPart::Body, DBPart::FrontWheels, DBPart::FrontBadging, DBPart::RearBadging,
					DBPart::FrontBumper, DBPart::RearBumper, DBPart::Skirt, DBPart::RoofScoop, DBPart::Spoiler, DBPart::LeftHeadlight, DBPart::LeftBrakelight,
					DBPart::LeftMirror };
				if (std::find(std::begin(meshParts), std::end(meshParts), partId) != std::end(meshParts))
				{
					int nameHash = Game::GetModelNameHash(partPtr, 0, 0, 0);
					if (!Game::PartMeshExists(nameHash))
					{
						continue;
					}
				}

				if ((partId == DBPart::FrontBrake || partId == DBPart::FrontRotor) && !Game::GetAppliedAttributeIParam(partPtr, Game::StringHash((char*)"MAX_LOD"), 0))
				{
					continue;
				}

				if (partId == DBPart::Exhaust && Game::GetAppliedAttributeIParam1(partPtr, Game::StringHash((char*)"CENTER"), 0) == 1)
				{
					continue;
				}

				if (partId == DBPart::FrontWheels && Game::GetAppliedAttributeIParam1(partPtr, Game::StringHash((char*)"INNER_RADIUS"), 0) != innerRadius)
				{
					continue;
				}

				bool isAutosculpt = Game::GetAppliedAttributeIParam(partPtr, Game::StringHash((char*)"MORPHTARGET_NUM"), 0) != NULL;
				if ((autosculpt && !isAutosculpt) || (!autosculpt && isAutosculpt))
				{
					continue;
				}

				bool isCarbon = Game::GetAppliedAttributeIParam1(partPtr, Game::StringHash((char*)"CARBONFIBRE"), 0) == 1;
				if ((carbon && !isCarbon) || (!carbon && isCarbon))
				{
					continue;
				}
			}

			i++;
			if (i > 70)
			{
				break;
			}

			// game stuff to add part to list
			auto vslot = (int*)Game::j_malloc_0(autosculpt ? 0x24 : 0x1C);
			*vslot = 0x009F9CF0;// vtable;
			auto slot = (PartSlot*)(vslot + 1);
			slot->Hash = 0x881F8EFA;
			slot->PartId = partId;
			slot->PartPtr = partPtr;
			slot->Name = 0x009F9CD8;
			if (autosculpt)
			{
				slot->Region = Game::AutosculptSelectablePart_ConvertSlotToRegion(partId);
				slot->Val = 0;
			}

			PartSlot* ListEnd = ListHead->Next;
			ListEnd->Prev = slot;
			ListHead->Next = slot;
			slot->Next = ListEnd;
			slot->Prev = ListHead;
		}
		else
		{
			break;
		}
	}
}

void __cdecl GetPartsListStandart(DBPart::_DBPart partId, PartSlot* ListHead, char carbon, int brandName, int innerRadius)
{
	GetPartsList(partId, ListHead, carbon, brandName, innerRadius, false);
}

//void __cdecl GetPartsListAutosculpt(DBPart::_DBPart partId, PartSlot* ListHead, char carbon, int brandName, int innerRadius)
//{
//	GetPartsList(partId, ListHead, carbon, brandName, innerRadius, true);
//}

void InitMenu()
{
	injector::MakeJMP(0x0083FBF1, AddMainMenuItemsCave, true);
	injector::MakeJMP(0x008683C9, OpenCustomMenuCave, true);
	injector::MakeJMP(0x00866074, AddMenuItemsCave, true);
	injector::MakeJMP(0x0085F74C, PartWarningCave, true);
	injector::MakeJMP(0x008665DA, DisablePartsCave, true);

	injector::MakeNOP(0x0085F90E, 5, true);
	injector::WriteMemory(0x0085F90E, 0x182474FF, true);

	injector::WriteMemory(0x009F9D14, StandardSelectablePart_GetCategoryHash, true);
	injector::MakeCALL(0x0084EB1F, GetCameraScreenName, true);

	injector::MakeCALL(0x0085FA37, GetPartsListStandart, true);
	//injector::MakeCALL(0x0085FA30, GetPartsListAutosculpt, true);
}