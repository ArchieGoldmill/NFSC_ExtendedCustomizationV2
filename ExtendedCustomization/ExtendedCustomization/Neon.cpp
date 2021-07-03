#include "Feature.h"
#include "Config.h"
#include <vector>
#include "GameStructs.h"
#include <cmath> 

float NeonBrightnessBackup = 0;
float NeonDirectionBackup = 0;
float NeonBrightnessMax = 1.0f;
float NeonBrightnessMin = 0.3f;

float CarDistMax = 2.0f;
float CarDistMult = 0.5f;
float CarDistBright = 70.0f;
float FrontShadowSize = 1.2f;
float RearShadowSize = 1.2f;
float SideShadowSize = 1.05f;

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
		renderInfo->NeonDirection = 1.0f;
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

int GetNeonTextureHash(RideInfo* rideInfo)
{
	int* partPtr = Game::GetPart(rideInfo, DBPart::Attachment15);
	if (partPtr)
	{
		return Game::GetAppliedAttributeIParam1(partPtr, Game::StringHash("TEXTURE_NAME"), 0);
	}

	return 0;
}

int GetShadowCutTextureHash(RideInfo* rideInfo)
{
	int neonHash = GetNeonTextureHash(rideInfo);
	if (neonHash)
	{
		return Game::StringHash1("_CUT", neonHash);
	}

	return 0;
}

void SetMaterialColor(int r, int g, int b, int a)
{
	Material* material = Game::elGetLightMaterial(Game::StringHash("NEONGLOW"), 1);

	float level = *Game::GameState == 3 ? 1 : 10;

	material->DiffuseMinRed = material->DiffuseMaxRed = r / 255.0f * level;
	material->DiffuseMinGreen = material->DiffuseMaxGreen = g / 255.0f * level;
	material->DiffuseMinBlue = material->DiffuseMaxBlue = b / 255.0f * level;
	material->DiffuseMinLevel = material->DiffuseMaxLevel = 1;
	material->DiffuseMinAlpha = material->DiffuseMaxAlpha = a;
}

int __stdcall SetNeonColor(CarRenderInfo* renderInfo, int originalColor)
{
	void* neonTexture = Game::GetTextureInfo(GetNeonTextureHash(renderInfo->RideInfo), 0, 0);
	if (neonTexture != renderInfo->CarShadowTexture)
	{
		return originalColor;
	}

	int* partPtr = Game::GetPart(renderInfo->RideInfo, DBPart::Attachment15);

	int r, g, b, a;
	float br = 1.0f;
	if (partPtr && !Game::IsStock(partPtr))
	{
		r = Game::GetAppliedAttributeIParam1(partPtr, Game::StringHash((char*)"RED"), 0);
		g = Game::GetAppliedAttributeIParam1(partPtr, Game::StringHash((char*)"GREEN"), 0);
		b = Game::GetAppliedAttributeIParam1(partPtr, Game::StringHash((char*)"BLUE"), 0);
		a = 1;

		if (Game::GetAppliedAttributeIParam1(partPtr, Game::StringHash((char*)"PULSE"), 0))
		{
			if (!Game::IsPaused())
			{
				renderInfo->NeonBrightness += renderInfo->NeonDirection * 1.7f * (*Game::DeltaTime);

				if (renderInfo->NeonBrightness > NeonBrightnessMax)
				{
					renderInfo->NeonBrightness = NeonBrightnessMax;
					renderInfo->NeonDirection = -1.0f;
				}

				if (renderInfo->NeonBrightness < NeonBrightnessMin)
				{
					renderInfo->NeonBrightness = NeonBrightnessMin;
					renderInfo->NeonDirection = 1.0f;
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
		a = 0;
	}

	int c = (int)(r * br) + (int)(g * br) * 0x100 + (int)(b * br) * 0x10000;

	int res = originalColor | c;

	//SetMaterialColor(r * br, g * br, b * br, a);

	return res;
}

void __declspec(naked) ShadowColorCave()
{
	static constexpr auto  Exit = 0x007BEA6D;

	__asm
	{
		mov eax, ecx;
		shl eax, 0x18;

		SAVE_REGS;
		push eax; // original color
		push ebx; // render info
		call SetNeonColor;
		RESTORE_REGS;

		mov ebx, [ebx + 0x3F8];
		test ebx, ebx;
		jmp Exit;
	}
}

double __fastcall DrawAmbientShadow(CarRenderInfo* renderInfo, int param, int a2, float* a3, float a4, int a5, int a6, int a7)
{
	HandleFrontSteerAngle();

	double ret = Game::CarRenderInfo_DrawAmbientShadow(renderInfo, a2, a3, a4, a5, a6, a7);

	int neonHash = GetNeonTextureHash(renderInfo->RideInfo);
	if (neonHash)
	{
		SideShadowSize = 1.2f;
		void* neonTexture = Game::GetTextureInfo(neonHash, 0, 0);
		auto shadowTexture = renderInfo->CarShadowTexture;
		renderInfo->CarShadowTexture = neonTexture;
		Game::CarRenderInfo_DrawAmbientShadow(renderInfo, a2, a3, a4, a5, a6, a7);
		renderInfo->CarShadowTexture = shadowTexture;
		SideShadowSize = 1.05f;
	}

	return ret;
}

const int _stdcall GetShadowStyle(CarRenderInfo* renderInfo)
{
	AddRenderInfo(renderInfo);
	int cutHash = GetShadowCutTextureHash(renderInfo->RideInfo);
	return cutHash ? cutHash : Game::StringHash("CARSHADOW");
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

void InitNeon()
{
	if (!Config::GetGlobal()->NeonMod)
	{
		return;
	}

	injector::MakeJMP(0x007E5A6A, CarShadowCave, true);
	injector::MakeCALL(0x007DECCD, DrawAmbientShadow, true);
	injector::MakeJMP(0x007BEA5D, ShadowColorCave, true);

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