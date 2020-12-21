#include "Feature.h"

float GetPartValue(int* rideInfo, DBPart::_DBPart part, const char* attrib)
{
	float val = 0;

	if (rideInfo) {
		int* partPtr = Game::GetPart(rideInfo, part);
		if (partPtr) {
			float* valPtr = (float*)Game::GetAppliedAttributeIParam(partPtr, Game::StringHash(attrib), 0);

			if (valPtr) {
				val = *(valPtr + 1);
			}
		}
	}

	return val;
}

float __stdcall GetCamber(int* rideInfo)
{
	return GetPartValue(rideInfo, DBPart::Attachment12, "ANGLE");;
}

DWORD Camber1 = 0x007DF771;
void __declspec(naked) CamberCave()
{
	__asm
	{
		pushad;
		push ecx;
		call GetCamber;
		popad;

		jmp Camber1;
	}
}

float __stdcall GetTrackWidth(int* rideInfo, int isRear, int original)
{
	float res = (float)original * 0.001f;
	auto bodyPtr = Game::GetPart(rideInfo, DBPart::Body);
	if (bodyPtr)
	{
		float* valPtr = (float*)Game::GetAppliedAttributeIParam(bodyPtr,
			Game::StringHash(isRear ? "REAR_TIRE_OFFSET" : "FRONT_TIRE_OFFSET"), 0);

		if (valPtr) {
			res = *(valPtr + 1);
		}
	}

	res += GetPartValue(rideInfo, isRear ? DBPart::RearBadging : DBPart::FrontBadging, "TIRE_OFFSET");
	res += GetPartValue(rideInfo, DBPart::Attachment13, "TIRE_OFFSET");
	res += GetPartValue(rideInfo, DBPart::Attachment12, "ANGLE") / 30.0f;

	return res;
}

void __declspec(naked) TrackWidthCave()
{
	static constexpr auto Exit = 0x007CB869;

	__asm
	{
		pushad;

		mov ecx, [esp + 0x38];
		push ecx;
		push ebp;
		mov ecx, [esp + 0x30];
		add ecx, 0x3f0;
		push[ecx];
		call GetTrackWidth;

		popad;

		jmp Exit;
	}
}

float __stdcall GetTireWidth(int* rideInfo, float original, int wheel)
{
	float mult = GetPartValue(rideInfo, DBPart::Attachment14, wheel < 2 ? "FRONT_TIRE_WIDTH" : "REAR_TIRE_WIDTH");
	if (mult == 0.0f)
	{
		mult = 1.0f;
	}

	return original * mult;
}

void __declspec(naked) TireWidthCave()
{
	static constexpr auto Exit = 0x007CB8D0;

	__asm
	{
		SAVE_REGS;
		push[esp + 0x28];

		mov eax, [eax];
		push eax;

		add esi, 0x3F0;
		mov esi, [esi];
		push esi;
		call GetTireWidth;
		push eax;
		fstp[esp];
		mov eax, [esp];
		pop ebx;
		RESTORE_REGS;

		mov ebx, [esi + 0x0000324C];
		jmp Exit;
	}
}


void InitWheelAdjustments()
{
	auto config = Config::GetGlobal();
	if (config->CamberMod)
	{
		injector::MakeJMP(0x007DF765, CamberCave, true);
	}

	if (config->TrackWidthMod)
	{
		injector::MakeJMP(0x007CB85F, TrackWidthCave, true);
	}

	injector::MakeJMP(0x007CB8C8, TireWidthCave, true);
}