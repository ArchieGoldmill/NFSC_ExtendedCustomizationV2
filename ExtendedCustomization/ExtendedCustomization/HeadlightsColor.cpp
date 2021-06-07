#include "Feature.h"

struct Flare
{
	float SizeAngle;
	float Size;
	float R;
	float G;
	float B;
	float A;
	float SizeDecStep;
	float Zindex;
	float A3[2];
	int Texture;
};

void __cdecl HeadlightsFalreColor(int* rideInfo, int a1, int a2, int a3, float a4, int a5, int a6, int a7, float a8, int a9, float a10)
{
	float* ptr = *Game::HeadlightFlare;
	float b1 = ptr[2];
	float b2 = ptr[3];
	float b3 = ptr[4];
	if (rideInfo)
	{
		int* headlight = Game::GetPart(rideInfo, DBPart::Headlight);
		if (headlight)
		{
			ptr[2] = Game::GetAppliedAttributeIParam1(headlight, Game::StringHash((char*)"RED"), 0);
			ptr[3] = Game::GetAppliedAttributeIParam1(headlight, Game::StringHash((char*)"GREEN"), 0);
			ptr[4] = Game::GetAppliedAttributeIParam1(headlight, Game::StringHash((char*)"BLUE"), 0);
		}
	}

	Game::RenderFlare(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);

	ptr[2] = b1;
	ptr[3] = b2;
	ptr[4] = b3;
}

void __declspec(naked) HeadlightsFalreColorCave()
{
	static constexpr auto  Exit = 0x007CC646;

	__asm
	{
		mov eax, [esp + 0x68];
		add eax, 0x3F0;
		push[eax];
		call HeadlightsFalreColor;
		pop eax;
		jmp Exit;
	}
}

void InitHeadlightsColor()
{
	//injector::MakeJMP(0x007CC641, HeadlightsFalreColorCave, true);
}