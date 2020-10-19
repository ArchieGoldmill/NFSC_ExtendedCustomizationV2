#include "Menu.h"
#include "Feature.h"

void __stdcall AddAutosculptMenus(char* _this)
{
	bool isAutosculpt = *(_this + 0x2D4);

	int* racingClassPtr = GameApi::GetAttributePointer((void*)0x00BBABB0, 0x247E0956, 0);
	bool showChopTop = racingClassPtr && *racingClassPtr == 2;
	if (showChopTop)
	{
		GameApi::AddMenuOption(_this, 0x301B0BF9, 0x4C, isAutosculpt, 0);
	}
}

DWORD MenuCaveExit = 0x00866235;
void __declspec(naked) AutosculptMenuCave()
{
	__asm
	{
		pushad;

		push esi;
		call AddAutosculptMenus;
		popad;

		jmp MenuCaveExit;
	}
}

void __stdcall AddAftermarketMenus(char* _this)
{
	bool isAutosculpt = *(_this + 0x2D4);

	// Interior
	GameApi::AddMenuOption(_this, 0x3691263B, 0x1D, isAutosculpt, 0);

	// Front Badging
	GameApi::AddMenuOption(_this, 0x3691263B, 0x49, isAutosculpt, 0);

	// Rear Badging
	GameApi::AddMenuOption(_this, 0x3691263B, 0x4B, isAutosculpt, 0);
}

void __declspec(naked) AftermarketMenuCave()
{
	__asm
	{
		mov ecx, esi;
		call GameApi::AddMenuOption;

		pushad;

		push esi;
		call AddAftermarketMenus;
		popad;

		jmp MenuCaveExit;
	}
}

DWORD FixInteriorPartLoadCaveExit = 0x0085FA25;
void __declspec(naked) FixInteriorPartLoadCave()
{
	__asm
	{
		cmp ebp, 0x1D; // Interoir
		jne defaultPartLoad;
		cmp ebp, eax;
		jmp interoirPartLoad;

	defaultPartLoad:
		cmp al, bl;
	interoirPartLoad:
		lea eax, [esp + 0x24];

		jmp FixInteriorPartLoadCaveExit;
	}
}

void InitMenu()
{
	injector::MakeJMP(0x0085FA1F, FixInteriorPartLoadCave, true);
	injector::MakeJMP(0x0086622E, AftermarketMenuCave, true);
	injector::MakeJMP(0x0086615E, AutosculptMenuCave, true);
}