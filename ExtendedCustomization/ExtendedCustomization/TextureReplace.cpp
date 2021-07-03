#include "TextureReplace.h"
#include "Feature.h"
#include "Config.h"

#pragma region TextureReplace

struct TextureReplaceNode
{
	int TextureHash;
	int ReplaceHash;
	int TexturePointer;
};

void __stdcall HandleTextureReplacements(int* texPtr)
{
	auto rideInfo = (RideInfo*)(*(texPtr + 0xFC));
	auto config = Config::GetGlobal();

	if (config->TiresMod)
	{
		int* attachment15 = Game::GetPart(rideInfo, DBPart::Attachment14);
		if (attachment15) {
			int hash = Game::GetAppliedAttributeIParam1((void*)attachment15, Game::StringHash((char*)"TEXTURE_NAME"), 0);
			if (hash) {
				// Tire texture
				auto tex = (TextureReplaceNode*)(texPtr + 0x171);// BADGING_EU
				tex->TextureHash = Game::StringHash((char*)"TIRE_STYLE01");
				tex->ReplaceHash = hash;
				tex->TexturePointer = -1;

				tex = (TextureReplaceNode*)(texPtr + 0x183);// DRIVER
				tex->TextureHash = Game::StringHash((char*)"TIRE_STYLE02");
				tex->ReplaceHash = hash;
				tex->TexturePointer = -1;
			}

			hash = Game::GetAppliedAttributeIParam1((void*)attachment15, Game::StringHash((char*)"TEXTURE_NAME_N"), 0);
			if (hash) {
				// Bump map
				auto tex = (TextureReplaceNode*)(texPtr + 0x180);// Empty
				tex->TextureHash = Game::StringHash((char*)"TIRE_STYLE01_N");
				tex->ReplaceHash = hash;
				tex->TexturePointer = -1;

				tex = (TextureReplaceNode*)(texPtr + 0x186);// _TIRE
				tex->TextureHash = Game::StringHash((char*)"TIRE_STYLE02_N");
				tex->ReplaceHash = hash;
				tex->TexturePointer = -1;
			}
		}
	}

	if (config->LicensePlateMod)
	{
		int hash = 0;
		auto carType = *Game::CarTypeInfoArray + 0x34 * rideInfo->CarId;
		int* licensePlatePtr = Game::GetPart(rideInfo, DBPart::LicensePlate);
		if (carType && *(carType + 0x25) == 1)
		{
			if (!licensePlatePtr || (licensePlatePtr && Game::IsStock(licensePlatePtr)))
			{
				hash = Game::StringHash("LP_COP");
			}
		}

		if (licensePlatePtr && !hash)
		{
			hash = Game::GetAppliedAttributeIParam1((void*)licensePlatePtr, Game::StringHash((char*)"TEXTURE_NAME"), 0);
		}

		if (hash) {
			auto tex = (TextureReplaceNode*)(texPtr + 0x177);
			tex->TextureHash = Game::StringHash((char*)"LICENSEPLATE");
			tex->ReplaceHash = hash;
			tex->TexturePointer = -1;
		}
	}
}

void __declspec(naked) HandleTextureReplacementsCave()
{
	__asm
	{
		pushad;
		push esi;
		call HandleTextureReplacements;
		popad;
		ret;
	}
}

#pragma endregion TextureReplace

#pragma region HeadlightsOnOff

bool IsGlareOn(int* rideInfo)
{
	return true;
	int** iterator = *Game::PVehicleList;
	int* pVehicle = NULL;
	for (int i = 0; i < *Game::PVehicleCount; i++)
	{
		int* pvh = iterator[i];
		if (pvh)
		{
			int* renderInfo = (int*)*(pvh + 0x572);
			if (renderInfo)
			{
				int ri = *(renderInfo + 0xFC);
				if (ri == (int)rideInfo)
				{
					pVehicle = pvh;
					break;
				}
			}
		}
	}

	if (pVehicle)
	{
		return Game::PVehicle_IsGlareOn(pVehicle, 7);
	}

	return false;
}

int __stdcall HeadLightsOnOff(int* renderInfo)
{
	if (*Game::ForceHeadlightsOff)
	{
		return 1;
	}

	int* rideInfo = (int*)(*(renderInfo + 0xFC));

	if (Game::GetTextureInfo(*(renderInfo + 0xC20), 0, 0))
	{
		if (*Game::GameState == 3)
		{
			return 1;
		}

		if (*Game::GameState == 6)
		{
			return 0;
			return !IsGlareOn(rideInfo);
		}
	}

	return 0;
}

void __declspec(naked) HeadLightsOnOffCave()
{
	static constexpr auto Exit = 0x007ADCAE;

	__asm
	{
		pushad;
		push ecx;
		call HeadLightsOnOff;
		test eax, eax;
		popad;

		je HeadLightsOff;
		mov esi, 1;
		jmp Exit;

	HeadLightsOff:
		mov esi, 0;
		jmp Exit;
	}
}

#pragma endregion HeadlightsOnOff

#pragma region MaterialReplace

void __stdcall LightMaterial(int* rideInfo, int emodel)
{
	auto carConfig = Config::Get(*rideInfo);

	if (carConfig->ReplaceBrakelightShader)
	{

		Material* material = NULL;
		if (*Game::GameState != 3)
		{
			material = Game::elGetLightMaterial(Game::StringHash("BRAKELIGHT_GLOW"), 1);
		}
		else
		{
			material = Game::elGetLightMaterial(Game::StringHash("BRAKELIGHT"), 1);
		}

		Game::eModel_ReplaceLightMaterial(emodel, Game::StringHash("BRAKELIGHTG"), (int)material);
	}

	if (carConfig->ReplaceHeadlightShader)
	{
		Material* material = NULL;
		if (*Game::GameState != 3 && IsGlareOn(rideInfo))
		{
			material = Game::elGetLightMaterial(Game::StringHash("LIGHT_GLOW"), 1);
		}
		else
		{
			material = Game::elGetLightMaterial(Game::StringHash("HEADLIGHTREFLECTOR"), 1);
		}

		Game::eModel_ReplaceLightMaterial(emodel, Game::StringHash("HEADLIGHTG"), (int)material);
	}
}

void __declspec(naked) LightMaterialCave()
{
	static constexpr auto Exit = 0x007DEF1D;

	__asm
	{
		pushad;

		push[edi];
		add esi, 0x3F0;
		push[esi];
		call LightMaterial;

		popad;

		cmp dword ptr[esp + 0x14], 0x2D;
		jmp Exit;
	}
}

#pragma endregion MaterialReplace

#pragma region LoadedCarTextures

void SetTextureHash(int* texPtr, int hash)
{
	for (int i = 0; i < 100; i++)
	{
		if (texPtr[i] == 0)
		{
			texPtr[i] = hash;
			break;
		}
	}

	texPtr[0xD0]++;
}

void __stdcall GetUsedCarTextureInfo(int* texPtr, int* rideInfo)
{
	int* brakelight = Game::GetPart(rideInfo, DBPart::LeftBrakelight);
	if (brakelight)
	{
		int hash = Game::GetAppliedAttributeIParam1(brakelight, Game::StringHash("TEXTURE_NAME"), 0);
		if (hash)
		{
			hash = Game::StringHash1("_ONF", hash);
		}
		else
		{
			char* carName = Game::GetCarTypeName(*rideInfo);
			char buf[64];
			sprintf(buf, "%s_KIT00_BRAKELIGHT_ONF", carName);
			hash = Game::StringHash(buf);
		}

		SetTextureHash(texPtr, hash);
		if (Game::GetTextureInfo(hash, 0, 0))
		{
			*(texPtr + 0xFE) = hash;
		}
	}
}

void __declspec(naked) GetUsedCarTextureInfoCave()
{
	__asm
	{
		pushad;
		push[esp + 0x94];
		push esi;
		call GetUsedCarTextureInfo;
		popad;

		pop esi;
		pop ebp;
		pop ebx;
		add esp, 0x60;
		ret;
	}
}

#pragma endregion LoadedCarTextures

void __declspec(naked) BrakelightOnfCave()
{
	static constexpr auto Exit = 0x007ADE9F;

	__asm
	{
		push eax;
		mov eax, Game::GameState;
		mov eax, [eax];
		cmp eax, 6;
		pop eax;
		jne BrakelightOff;
		cmp[ecx + 0x000030B0], 0;
		je BrakelightOff;
		cmp eax, 0;
		jne BrakelightOff;

		mov edx, [ecx + 0x000030B0];
		mov eax, [ecx + eax * 4 + 0x000030A4];
		mov ebx, [ecx + 0x000030B0];
		mov ebp, [ecx + 0x000030B0];
		jmp Exit;

	BrakelightOff:
		mov edx, [ecx + eax * 4 + 0x00003098];
		mov eax, [ecx + eax * 4 + 0x000030A4];
		mov ebx, [ecx + esi * 4 + 0x00003098];
		mov ebp, [ecx + edi * 4 + 0x00003098];

		jmp Exit;
	}
}

void InitTextureReplace()
{
	injector::MakeJMP(0x007D9E14, HandleTextureReplacementsCave, true);
	injector::MakeJMP(0x007ADCA8, HeadLightsOnOffCave, true);
	//injector::MakeJMP(0x007DEF18, LightMaterialCave, true);
	injector::MakeJMP(0x007ADE83, BrakelightOnfCave, true);

	injector::WriteMemory(0x007CECC7, (DBPart::LeftHeadlight + 0x15) * 4, true);
	injector::WriteMemory(0x007CECCD, (DBPart::LeftBrakelight + 0x15) * 4, true);
	injector::MakeJMP(0x007CF764, GetUsedCarTextureInfoCave, true);
}