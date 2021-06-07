#include "Feature.h"
#include "Config.h"
#include <vector>
#include "GameStructs.h"

float NeonBrightnessBackup = 0;
float NeonDirectionBackup = 0;
void AddRenderInfo(CarRenderInfo* renderInfo)
{
	if (*Game::GameState == 3 && NeonBrightnessBackup)
	{
		renderInfo->NeonBrightness = NeonBrightnessBackup;
		renderInfo->NeonDirection = NeonDirectionBackup;
	}
	else
	{
		renderInfo->NeonBrightness = (Game::Random(101) % 100 + 30) / 100.0f;
		renderInfo->NeonDirection = 1.2f;
	}	
}

void HandleFrontSteerAngle()
{
	if (*Game::GameState == 3 && Game::AnimateFrontSteerAngle)
	{
		if (abs(*Game::FrontSteerAngle - Game::TargetFrontSteerAngle) > 1.0f)
		{
			float dir = Game::TargetFrontSteerAngle - *Game::FrontSteerAngle;
			dir = dir > 0 ? 35.0 : -35.0;
			*Game::FrontSteerAngle += dir * *Game::DeltaTime;
		}
		else
		{
			Game::AnimateFrontSteerAngle = false;
		}
	}
}

int __stdcall SetNeonColor(CarRenderInfo* renderInfo, int originalColor)
{
	HandleFrontSteerAngle();

	auto rideInfo = renderInfo->RideInfo;

	if (!rideInfo)
	{
		return 0;
	}

	int* partPtr = Game::GetPart(rideInfo, DBPart::Attachment15);

	int r, g, b;
	float br = 1.0f;
	if (partPtr)
	{
		r = Game::GetAppliedAttributeIParam1(partPtr, Game::StringHash((char*)"RED"), 0);
		g = Game::GetAppliedAttributeIParam1(partPtr, Game::StringHash((char*)"GREEN"), 0);
		b = Game::GetAppliedAttributeIParam1(partPtr, Game::StringHash((char*)"BLUE"), 0);

		if (Game::GetAppliedAttributeIParam1(partPtr, Game::StringHash((char*)"PULSE"), 0))
		{
			if (!Game::IsPaused())
			{
				renderInfo->NeonBrightness += renderInfo->NeonDirection * (*Game::DeltaTime);
				if (renderInfo->NeonBrightness > 1.0f || renderInfo->NeonBrightness < 0.3f)
				{
					renderInfo->NeonDirection *= -1;
					if (renderInfo->NeonBrightness > 1.0f)
					{
						renderInfo->NeonBrightness = 1.0f;
					}
					else
					{
						renderInfo->NeonBrightness = 0.3f;
					}
				}

				NeonBrightnessBackup = renderInfo->NeonBrightness;
				NeonDirectionBackup = renderInfo->NeonDirection;
			}

			br = renderInfo->NeonBrightness;
		}
	}
	else
	{
		r = 0;
		g = 0;
		b = 0;
	}

	int c = (int)(r * br) + (int)(g * br) * 0x100 + (int)(b * br) * 0x10000;

	int res = originalColor | c;

	return res;
}

void __declspec(naked) ShadowColorCave()
{
	static constexpr auto  Exit = 0x007BEA6D;

	__asm
	{
		SAVE_REGS;

		push eax;
		mov eax, [esp + 0x1c];
		push eax;
		call SetNeonColor;

		RESTORE_REGS;
		jmp Exit;
	}
}

const int _stdcall GetShadowStyle(CarRenderInfo* renderInfo)
{
	AddRenderInfo(renderInfo);
	auto rideInfo = renderInfo->RideInfo;
	int* part = Game::GetPart(rideInfo, DBPart::Attachment15);

	if (part)
	{
		return Game::GetAppliedAttributeIParam1(part, Game::StringHash("TEXTURE_NAME"), Game::StringHash("CARSHADOW"));
	}

	return Game::StringHash("CARSHADOW");
}

void __declspec(naked) CarShadowCave()
{
	static constexpr auto  Exit = 0x007E5A6F;

	__asm
	{
		SAVE_REGS;
		push esi;
		call GetShadowStyle;
		RESTORE_REGS;

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

	injector::MakeJMP(0x007E5A6A, CarShadowCave, true);
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