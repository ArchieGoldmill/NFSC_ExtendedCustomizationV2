#include "Feature.h"

float GetPartValue(int* rideInfo, DBPart::_DBPart part)
{
	float val = 0;

	if (rideInfo) {
		int* partPtr = Game::GetPart(rideInfo, part);
		if (partPtr) {
			float* valPtr = (float*)Game::GetAppliedAttributeIParam(partPtr, Game::StringHash((char*)"VALUE"), 0);

			if (valPtr) {
				val = *(valPtr + 1);
			}
		}
	}

	return val;
}

int __stdcall GetCamber(int* rideInfo)
{
	float res = GetPartValue(rideInfo, DBPart::Attachment12);;

	int ires = 0;
	memcpy(&ires, &res, 4);
	return ires;
}

DWORD Camber1 = 0x007DF771;
void __declspec(naked) CamberCave()
{
	__asm
	{
		pushad;
		push ecx;
		call GetCamber;
		push eax;
		fld[esp];
		pop eax;
		popad;

		jmp Camber1;
	}
}

int __stdcall GetTrackWidth(int* rideInfo, int isRear, int original)
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

	res += GetPartValue(rideInfo, isRear ? DBPart::RearBadging : DBPart::FrontBadging);
	res += GetPartValue(rideInfo, DBPart::Attachment13);
	res += GetPartValue(rideInfo, DBPart::Attachment12) / 30.0f;

	int ires = 0;
	memcpy(&ires, &res, 4);
	return ires;
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

		push eax;
		fld[esp];
		pop eax;

		popad;

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
}