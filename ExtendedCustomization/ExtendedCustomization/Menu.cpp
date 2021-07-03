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

	AddMainMenuItem(_this, Game::StringHash("CUST_PARTS_WHEELS"), 7);

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

void AddMenuItem(char* _this, DBPart::_DBPart dbpart, bool isAutosculpt, int hash = 0)
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
		if (!hash)
		{
			hash = Config::GetPartHeader(carId, dbpart, isAutosculpt);
		}

		Game::AddMenuOption(_this, hash, dbpart, isAutosculpt, filter);
	}
}

struct RimBrand
{
	int hash;
	int header;
};
bool operator==(const RimBrand& a, const RimBrand& b)
{
	return a.hash == b.hash;
}

std::vector<RimBrand> RimBrands;
std::vector<RimBrand>& GetRimBrands()
{
	if (RimBrands.size() == 0)
	{
		RimBrand rimBrand;
		rimBrand.hash = Game::StringHash("STOCK");
		rimBrand.header = Game::StringHash("COMMON_STOCK");

		RimBrands.push_back(rimBrand);

		int* partPtr = 0;
		while (true)
		{
			partPtr = Game::GetCarPart((int)Game::CarPartDB, 0, GetCarId(), DBPart::FrontWheels, 0, partPtr, -1);
			if (partPtr)
			{
				if (Game::GetAppliedAttributeIParam1(partPtr, Game::StringHash((char*)"MORPHTARGET_NUM"), 0) == 0)
				{
					int brand = Game::GetAppliedAttributeIParam1(partPtr, Game::StringHash((char*)"BRAND_NAME"), 0);

					rimBrand.hash = brand;
					rimBrand.header = Game::StringHash1("_RIM_BRAND", brand);
					if (brand && std::count(RimBrands.begin(), RimBrands.end(), rimBrand) == 0)
					{
						RimBrands.push_back(rimBrand);
					}
				}
			}
			else
			{
				break;
			}
		}
	}

	return RimBrands;
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
			auto brands = GetRimBrands();
			for (auto brand : brands)
			{
				AddMenuItem(_this, DBPart::FrontWheels, isAutosculpt, brand.header);
			}
		}

		if (lastMenu == 8)
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
};

void __cdecl GetPartsListStandart(DBPart::_DBPart partId, PartSlot* ListHead, bool carbon, int brandName, int innerRadius)
{
	if (partId == DBPart::FrontWheels)
	{
		auto brands = GetRimBrands();
		for (auto brand : brands)
		{
			if (brand.header == brandName)
			{
				brandName = brand.hash;
				break;
			}
		}
	}
	else
	{
		brandName = 0;
	}

	int* partPtr = 0;

	int i = 0;
	while (true)
	{
		partPtr = Game::GetCarPart((int)Game::CarPartDB, 0x009D1954, GetCarId(), partId, 0, partPtr, -1);
		if (partPtr)
		{
			if (!Game::IsStock(partPtr))
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

				if (partId == DBPart::FrontWheels)
				{
					if (Game::GetAppliedAttributeIParam1(partPtr, Game::StringHash((char*)"INNER_RADIUS"), 0) != innerRadius)
					{
						continue;
					}

					if (Game::GetAppliedAttributeIParam1(partPtr, Game::StringHash((char*)"BRAND_NAME"), 0) != brandName)
					{
						continue;
					}
				}

				if (Game::GetAppliedAttributeIParam(partPtr, Game::StringHash((char*)"MORPHTARGET_NUM"), 0) != 0)
				{
					continue;
				}

				bool isCarbon = Game::GetAppliedAttributeIParam1(partPtr, Game::StringHash((char*)"CARBONFIBRE"), 0) == 1;
				if ((carbon && !isCarbon) || (!carbon && isCarbon))
				{
					continue;
				}
			}
			else
			{
				if (brandName && Game::StringHash("STOCK") != brandName)
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
			auto vslot = (int*)Game::j_malloc_0(0x1C);
			*vslot = 0x009F9CF0;// vtable;
			auto slot = (PartSlot*)(vslot + 1);
			slot->Hash = 0x881F8EFA;
			slot->PartId = partId;
			slot->PartPtr = partPtr;
			slot->Name = 0x009F9CD8;

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

void __declspec(naked) PartListBrandCave()
{
	static constexpr auto Exit = 0x0085FA1E;

	__asm
	{
		mov ebp, [esp + 0x00000140];
		push ecx;

		cmp lastMenu, 7;
		jne original;
		mov ebx, esp;
		add ebx, 0x140;
		mov ebx, [ebx];
		add ebx, 0x2C;
		mov ebx, [ebx];
		push ebx;

		xor ebx, ebx;
		jmp Exit;

	original:
		xor ebx, ebx;
		push ebx;
		jmp Exit;
	}
}

struct TrackPositionMarker
{
	TrackPositionMarker* Prev;
	TrackPositionMarker* Next;
	int Hash;
	int blank;
	float X;
	float Y;
};

void SetCoords(TrackPositionMarker* item, float x, float y)
{
	item->X = x;
	item->Y = y;
}

void __stdcall SetMainGarageCoords(TrackPositionMarker* coords)
{
	if (coords->Hash == Game::StringHash("CarPosition_Main"))
	{
		switch (Config::GetGlobal()->MainGarage)
		{
		case 1: SetCoords(coords, 0, 0); break;
		case 2: SetCoords(coords, 305.52, 0); break;
		case 3: SetCoords(coords, 458.39, 362.79); break;
		case 4: SetCoords(coords, 203.68, 0.02); break;
		case 5: SetCoords(coords, -725.86, 0); break;
		case 6: SetCoords(coords, -200.30, 0); break;
		case 7: SetCoords(coords, -985, 0); break;
		case 8: SetCoords(coords, -465.30, 0); break;
		case 9: SetCoords(coords, 522.68, 0); break;
		default: break;
		}
	}
}

void __declspec(naked) MainGarageCave()
{
	static constexpr auto Exit = 0x0083F272;

	__asm
	{
		pushad;
		push eax;
		call SetMainGarageCoords;
		popad;

		mov ecx, [esi + 0x44];
		add eax, 0x10;

		jmp Exit;
	}
}

bool __fastcall FEPackage_Startup(FEPackage* package, int param, int a2)
{
	bool result = Game::FEPackage__Startup(package, a2);

	if (strcmp(package->Name, "FeCustomizeParts.fng") == 0)
	{
		auto option12 = (FEGroup*)Game::FEPackage_FindObjectByHash(package, Game::StringHash("OPTION_12"));

		for (int i = 13; i <= 50; i++)
		{
			auto copy = Game::FEGroup_Clone(option12, false);
			char buff[10];
			sprintf(buff, "OPTION_%d", i);
			copy->NameHash = Game::StringHash(buff);
			copy->Prev = option12;
			copy->Next = option12->Next;
			option12->Next = copy;
			package->Objects.NumElements++;
		}

		Game::FEPackage_BuildMouseObjectStateList(package);
	}

	return result;
}

void __declspec(naked) RimSizeCave1()
{
	static constexpr auto Exit = 0x0084020F;

	__asm
	{
		mov ecx, [esi + 0x34];
		mov eax, [ecx + 0x2C];
		cmp lastMenu, 7;
		jne RimSizeCave1_Exit;
		mov eax, 0x0B47AB2E;

	RimSizeCave1_Exit:
		jmp Exit;
	}
}

void __declspec(naked) RimSizeCave2()
{
	static constexpr auto Exit = 0x00840349;

	__asm
	{
		mov eax, [esi + 0x34];
		mov eax, [eax + 0x2C];
		cmp lastMenu, 7;
		jne RimSizeCave2_Exit;
		mov eax, 0x0B47AB2E;

	RimSizeCave2_Exit:
		jmp Exit;
	}
}

void __declspec(naked) RimSizeCave3()
{
	static constexpr auto Exit = 0x008469CC;

	__asm
	{
		mov edx, [esi + 0x34];
		mov eax, [ecx];
		mov edi, [edx + 0x2C];
		cmp lastMenu, 7;
		jne RimSizeCave3_Exit;
		mov edi, 0x0B47AB2E;

	RimSizeCave3_Exit:
		jmp Exit;
	}
}

void __declspec(naked) RimSizeCave4()
{
	static constexpr auto Exit = 0x0085FCE1;

	__asm
	{
		mov eax, [esp + 0x08];
		cmp lastMenu, 7;
		jne RimSizeCave4_Exit;
		mov eax, 0x0B47AB2E;

	RimSizeCave4_Exit:
		cmp eax, 0xA234CD03;
		jmp Exit;
	}
}

void __declspec(naked) RimSizeCave5()
{
	static constexpr auto Exit = 0x008663F3;

	__asm
	{
		cmp lastMenu, 7;
		jne RimSizeCave5_Exit;
		cmp eax, eax;
		jmp Exit;

	RimSizeCave5_Exit:
		cmp [edi + 0x2C], 0x0B47AB2E;
		jmp Exit;
	}
}

void __declspec(naked) RimSizeCave6()
{
	static constexpr auto Exit = 0x0086653C;

	__asm
	{
		mov eax, [esi + 0x34];
		mov edi, [eax + 0x2C];
		cmp lastMenu, 7;
		jne RimSizeCave6_Exit;
		mov edi, 0x0B47AB2E;

	RimSizeCave6_Exit:
		jmp Exit;
	}
}

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

	injector::MakeJMP(0x0085FA13, PartListBrandCave, true);
	injector::MakeJMP(0x00840209, RimSizeCave1, true);
	injector::MakeJMP(0x00840343, RimSizeCave2, true);
	injector::MakeJMP(0x008469C4, RimSizeCave3, true);
	injector::MakeJMP(0x0085FCD8, RimSizeCave4, true);
	injector::MakeJMP(0x008663EC, RimSizeCave5, true);
	injector::MakeJMP(0x00866536, RimSizeCave6, true);

	injector::MakeCALL(0x00600564, FEPackage_Startup, true);

	// show CF toggle
	//injector::MakeNOP(0x00866530, 6, true);
	//injector::MakeNOP(0x00840207, 2, true);

	int garage = Config::GetGlobal()->MainGarage;
	if (garage > 0)
	{
		injector::MakeJMP(0x0083F26C, MainGarageCave, true);
	}
}