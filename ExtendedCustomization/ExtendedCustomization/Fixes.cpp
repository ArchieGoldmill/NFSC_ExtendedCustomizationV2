#include "includes/injector/injector.hpp"
#include "Config.h"
#include "GameApi.h"
#include <thread>

void FixDecals()
{
	int* partPtr = 0;
	char* front = 0;
	char* rear = 0;
	while (true)
	{
		partPtr = Game::GetCarPart((int)Game::CarPartDB, 0, 0, DBPart::FrontDecal, 0, partPtr, -1);
		if (!partPtr)
		{
			break;
		}

		char* id = (char*)Game::GetAppliedAttributeIParam(partPtr, Game::StringHash("PARTID_UPGRADE_GROUP"), 0);
		if (*(id + 5) == 0x60)
		{
			if (*(id + 4) == 0x20)
			{
				front = id;
			}

			if (*(id + 4) == 0x21)
			{
				rear = id;
			}

			if (front != 0 && rear != 0)
			{
				break;
			}
		}
	}

	if (front)
	{
		*(front + 4) = 0;
		*(front + 5) = 0x68;
	}

	if (rear)
	{
		*(rear + 4) = 0;
		*(rear + 5) = 0x69;
	}
}

void FixDecalsThread()
{
	while (!(*Game::CarPartDB && *Game::CarPartDB != (int)Game::CarPartDB));

	FixDecals();
	*(Game::CarPartSlotMap + DBPart::FrontDecal) = 0x68;
	*(Game::CarPartSlotMap + DBPart::RearDecal) = 0x69;
}

void FixFrontRearDecals()
{
	auto config = Config::GetGlobal();
	if (config->FixFrontRearDecals)
	{
		std::thread(FixDecalsThread).detach();
	}

	if (config->AllowStockRimsPaint)
	{
		injector::MakeNOP(0x0083F749, 6, true);
		char disableStockRimCheck[3] = { 0x32, 0xC0, 0xC3 };
		injector::WriteMemoryRaw(0x0083F749, disableStockRimCheck, 3, true);
		injector::MakeNOP(0x007E5FB2, 2, true);
	}

	//rear rims
	//injector::WriteMemory(0x007D6C76, 5, true);
}