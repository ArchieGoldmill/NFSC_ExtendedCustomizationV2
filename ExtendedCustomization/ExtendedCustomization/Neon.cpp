#include "Feature.h"
#include "Config.h"
#include <vector>
#ifdef _DEBUG
#include <iostream>
#endif

struct RenderExt
{
	int* CarRenderInfo;
	float neonState;
	float dir;
};

RenderExt RenderExtBackup;
std::vector<RenderExt> RenderExts;

void AddRenderInfo(int* renderInfo)
{
	RenderExt ext;

	if (*Game::GameState == 3 && RenderExtBackup.dir != 0)
	{
		ext = RenderExtBackup;
	}
	else
	{
		ext.neonState = (Game::Random(101) % 100 + 30) / 100.0f;
		ext.dir = 1.2f;
	}

	ext.CarRenderInfo = renderInfo;

	RenderExts.push_back(ext);
}

RenderExt* GetState(int* renderInfo)
{
	for (auto& i : RenderExts)
	{
		if (i.CarRenderInfo == renderInfo)
		{
			return &i;
		}
	}

	return NULL;
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

int __stdcall SetNeonColor(int* renderInfo, int originalColor)
{
	HandleFrontSteerAngle();

	int* rideInfo = (int*)*(renderInfo + 0xFC);

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

		if (Game::GetAppliedAttributeIParam1(partPtr, Game::StringHash((char*)"PULSE"), 0)) {
			auto state = GetState(renderInfo);
			if (state)
			{
				if (!Game::IsPaused())
				{
					state->neonState += state->dir * *Game::DeltaTime;
					if (state->neonState > 1 || state->neonState < 0.3f)
					{
						state->dir = state->dir * -1.0f;
					}

					if (state->neonState > 1)
					{
						state->neonState = 1;
					}

					if (state->neonState < 0.3f)
					{
						state->neonState = 0.3f;
					}
				}
				//std::cout << state->neonState << std::endl;

				br = state->neonState;
			}
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

DWORD ShadowColor1 = 0x007BEA6D;
void __declspec(naked) ShadowColorCave()
{
	__asm
	{
		SAVE_REGS;

		push eax;
		mov eax, [esp + 0x1c];
		push eax;
		call SetNeonColor;

		RESTORE_REGS;
		jmp ShadowColor1;
	}
}

const int _stdcall GetShadowStyle(int* renderInfo)
{
	AddRenderInfo(renderInfo);
	int* rideInfo = (int*)*(renderInfo + 0xFC);
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

void __stdcall RemoveRenderInfo(int* renderInfo)
{
	int len = RenderExts.size();
	for (int i = 0; i < len; i++)
	{
		if (RenderExts[i].CarRenderInfo == renderInfo)
		{
			if (*Game::GameState == 3)
			{
				RenderExtBackup = RenderExts[i];
			}

			RenderExts.erase(RenderExts.begin() + i);
			break;
		}
	}
}

void __declspec(naked) CarRenderInfoDtCave()
{
	static constexpr auto  Exit = 0x007D5287;

	__asm
	{
		pushad;
		push ecx;
		call RemoveRenderInfo;
		popad;

		push 0x009AD4A3;
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
	injector::MakeJMP(0x007D5282, CarRenderInfoDtCave, true);
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