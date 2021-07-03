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

	//int* tex = Game::GetTextureInfo(Game::StringHash("HEADLIGHT_XENON_YEL"), 0, 0);
	//int** texturePtr = (int**)0x00B4303C;
	//*texturePtr = tex;

	//texturePtr = (int**)0x00B43040;
	//*texturePtr = tex;

	//texturePtr = (int**)0x00B43038;
	//*texturePtr = tex;
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

int* DynamicLights;

void __declspec(naked) HeadlightsCave()
{
	static constexpr auto  Exit = 0x0074A560;
	static constexpr auto  Func1 = 0x0074A380;

	__asm
	{
		mov eax, 0x80;
		mul esi;
		add eax, DynamicLights;
		call Func1;
		jmp Exit;
	}
}

void InitHeadlightsColor()
{
	//const int carDynamicLightsCount = 20;
	//const int carDynamicLightSize = 0x80;
	//DynamicLights = (int*)calloc(carDynamicLightsCount, carDynamicLightSize);
	//for (int i = 0; i < carDynamicLightsCount; i++)
	//{
	//	int* ptr = DynamicLights + (carDynamicLightSize / 4) * i;
	//	Game::DynamicLightCtor(ptr);

	//	ptr[0] = 0x00B1B620;
	//	ptr[1] = 0x009EA844;
	//	ptr[2] = 9;
	//}

	//injector::MakeJMP(0x0074A557, HeadlightsCave, true);
	//injector::MakeJMP(0x007CC641, HeadlightsFalreColorCave, true);
}