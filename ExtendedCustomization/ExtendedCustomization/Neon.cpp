#include "Feature.h"
#include "Config.h"

int __stdcall SetNeonColor(int* rideInfo, int originalColor)
{
	if (!rideInfo)
	{
		return 0;
	}

	int* partPtr = Game::GetPart(rideInfo, DBPart::Attachment15);

	int r, g, b;
	if (partPtr)
	{
		r = Game::GetAppliedAttributeIParam1(partPtr, Game::StringHash((char*)"RED"), 0);
		g = Game::GetAppliedAttributeIParam1(partPtr, Game::StringHash((char*)"GREEN"), 0);
		b = Game::GetAppliedAttributeIParam1(partPtr, Game::StringHash((char*)"BLUE"), 0);
	}
	else
	{
		r = 0;
		g = 0;
		b = 0;
	}

	float br = 1.0f;
	int c = (int)(r * br) + (int)(g * br) * 0x100 + (int)(b * br) * 0x10000;

	int res = originalColor | c;

	return res;
}

#define SAVE_REGS __asm\
{\
	__asm push ebx\
	__asm push ecx\
	__asm push edx\
	__asm push edi\
	__asm push esi\
}\

#define RESTORE_REGS __asm\
{\
	__asm pop esi\
	__asm pop edi\
	__asm pop edx\
	__asm pop ecx\
	__asm pop ebx\
}\

DWORD ShadowColor1 = 0x007BEA6D;
void __declspec(naked) ShadowColorCave()
{
	__asm
	{
		SAVE_REGS;

		push eax;
		mov eax, [esp + 0x1c];
		add eax, 0x3F0;
		push[eax];
		call SetNeonColor;

		RESTORE_REGS;
		jmp ShadowColor1;
	}
}

const char* Style1 = "CARSHADOW_NEON";
const char* _stdcall GetShadowStyle(int** carPtr)
{
	//if (color == NULL || color->hash == 0x471A1DCA)//stock
	//{
	//	return (char*)0x009F26B4;
	//}

	//if (color->style)
	//{
	//	return Style2;
	//}

	return Style1;
}

void __declspec(naked) CarShadowCave()
{
	static constexpr auto  Exit = 0x007E5A6A;

	__asm
	{
		SAVE_REGS;
		mov eax, [ebp + 8];
		push eax;
		call GetShadowStyle;
		RESTORE_REGS;
	shadowOriginal:
		push eax;
		jmp Exit;
	}
}

float CarDistMax = 2.0f;
float CarDistMult = 0.5f;
float CarDistBright = 70.0f;
float FrontShadowSize = 1.2f;
float RearShadowSize = 1.2f;
float SideShadowSize = 1.05f;
void InitNeon()
{
	if (!Config::GetGlobal()->NeonMod)
	{
		return;
	}

	injector::MakeJMP(0x007E5A65, CarShadowCave, true);
	injector::MakeJMP(0x007BEA68, ShadowColorCave, true);

	injector::WriteMemory<float*>(0x007BE4F4, &CarDistMax, true);
	injector::WriteMemory<float*>(0x007BE50D, &CarDistMax, true);
	injector::WriteMemory<float*>(0x007BE501, &CarDistMult, true);
	injector::WriteMemory<float*>(0x007BEA36, &CarDistBright, true);
	injector::WriteMemory<float*>(0x007BE5B9, &FrontShadowSize, true);
	injector::WriteMemory<float*>(0x007BE5AB, &RearShadowSize, true);
	injector::WriteMemory<float*>(0x007BE59B, &SideShadowSize, true);
	injector::WriteMemory<float*>(0x007BE58B, &SideShadowSize, true);
	injector::MakeNOP(0x007BEA3A, 6, true);
}