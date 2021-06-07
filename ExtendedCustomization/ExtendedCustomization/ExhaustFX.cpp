#include <vector>
#include "Feature.h"
#include "Config.h"
#include "GameStructs.h"

using namespace std;

struct RotationMatrix
{
	VectorW x;
	VectorW y;
	VectorW z;
};

struct MountPoint
{
	int hash;
	int blank[3];
	RotationMatrix rotationMatrix;
	VectorW position;
};

struct MPListItem
{
	MPListItem* prev;
	Vector v;
	MountPoint* mp;
	MPListItem* next;
};

bool operator==(const Vector& a, const Vector& b)
{
	return a.x == b.x && a.y == b.y && a.z == b.z;
}

bool operator==(const VectorW& a, const VectorW& b)
{
	return a.v == b.v && a.w == b.w;
}

bool operator==(const RotationMatrix& a, const RotationMatrix& b)
{
	return a.x == b.x && a.y == b.y && a.z == b.z;
}

bool operator==(const MountPoint& a, const MountPoint& b)
{
	return a.hash == b.hash && a.position == b.position && a.rotationMatrix == b.rotationMatrix;
}

Vector operator+(const Vector& a, const Vector& b)
{
	Vector w;

	w.x = a.x + b.x;
	w.y = a.y + b.y;
	w.z = a.z + b.z;

	return w;
}

VectorW operator+(const VectorW& a, const VectorW& b)
{
	VectorW w;

	w.v = a.v + b.v;
	w.w = a.w;

	return w;
}

vector<MountPoint*> exhaustFX;
MountPoint* GetExhaustFX(MountPoint* exhaust, MountPoint* fx, bool invert)
{
	MountPoint mp;
	mp = *fx;
	if (invert)
	{
		mp.position.v.y = -mp.position.v.y;

		mp.rotationMatrix.x.v.y = -mp.rotationMatrix.x.v.y;
		mp.rotationMatrix.y.v.x = -mp.rotationMatrix.y.v.x;
		mp.rotationMatrix.y.v.z = -mp.rotationMatrix.y.v.z;
		mp.rotationMatrix.z.v.y = -mp.rotationMatrix.z.v.y;
	}

	mp.position = exhaust->position + mp.position;

	for (MountPoint* i : exhaustFX)
	{
		if (*i == mp)
		{
			return i;
		}
	}

	auto* exhaustMP = new MountPoint();
	*exhaustMP = mp;
	exhaustFX.push_back(exhaustMP);

	return exhaustMP;
}

void __stdcall CreateMountPointItem(int* eModel, MountPoint* original, int* list, int* counter, MountPoint* offset = NULL, bool invert = false)
{
	(*counter)++;

	int* slotPool = (int*)0x00B74C34;
	auto item = (MPListItem*)Game::bOMalloc(*slotPool);

	if (offset)
	{
		auto mp = GetExhaustFX(offset, original, invert);
		item->mp = mp;
		item->v = mp->position.v;
	}
	else
	{
		item->mp = original;
		item->v = original->position.v;
	}

	item->prev = (MPListItem*)list;

	int* back = (int*)*(list + 1);
	*(list + 1) = (int)item;
	*back = (int)item;
}

char* GetModelName(int* eModel)
{
	char* partName = (char*)*(eModel + 1);
	partName += 24;

	return partName;
}

void __stdcall HandleMountPointItem(int* eModel, MountPoint* original, int* list, int* counter, int* renderInfo)
{
	if (original->hash == Game::StringHash((char*)"EXHAUST_FX"))
	{
		auto modelName = GetModelName(eModel);
		auto centerExhaustMarker = (MountPoint*)*(renderInfo + 0x10D);
		auto rightExhaustMarker = (MountPoint*)*(renderInfo + 0x10E);
		auto leftExhaustMarker = (MountPoint*)*(renderInfo + 0x10F);
		if (centerExhaustMarker || rightExhaustMarker || leftExhaustMarker)
		{
			if (strstr(modelName, "EXHAUST") != NULL)
			{
				if (strstr(modelName, "CENTER") != NULL)
				{
					CreateMountPointItem(eModel, original, list, counter, centerExhaustMarker);
				}
				else
				{
					if (leftExhaustMarker)
					{
						CreateMountPointItem(eModel, original, list, counter, leftExhaustMarker);
					}

					if (rightExhaustMarker)
					{
						CreateMountPointItem(eModel, original, list, counter, rightExhaustMarker, true);
					}
				}
			}

			// If we have exhaust mountpoint then ignore other FX mountpoints
			return;
		}
	}

	CreateMountPointItem(eModel, original, list, counter);
}

void __declspec(naked) FixExhaustCave()
{
	static constexpr auto Exit = 0x007BEC35;
	__asm
	{
		pushad;

		push[esp + 0x38];
		mov eax, esp;
		add eax, 0x34;
		push eax; //counter ptr
		push ebx;
		push esi; //mount point
		push ebp; //model
		call HandleMountPointItem;

		popad;
		jmp Exit;
	}
}

void __declspec(naked) ExhaustMarkerCave()
{
	static constexpr auto Exit = 0x007DA137;

	__asm
	{
		mov eax, [ebx + 0x0000017C]; // rear bumper
		test eax, eax;
		jne ExhaustMarkerExit;
		mov eax, [ebx + 0x000000B0]; // body

	ExhaustMarkerExit:
		jmp Exit;
	}
}

bool __stdcall ExhaustRenderByMarker(int* exhaust)
{
	if (!Game::IsStock(exhaust))
	{
		return 1;
	}

	return Game::GetAppliedAttributeIParam1(exhaust, Game::StringHash("EXHAUST_MARKER"), 0);
}

void __declspec(naked) ExhaustRenderByMarkerCave()
{
	static constexpr auto Exit = 0x007DF2F5;

	__asm
	{
		pushad;
		push ecx;
		call ExhaustRenderByMarker;
		test al, al;
		popad;
		jmp Exit;
	}
}

void __declspec(naked) ExhaustRenderCave()
{
	static constexpr auto Exit = 0x007DEFCF;

	__asm
	{
		pushad;
		push ecx;
		call ExhaustRenderByMarker;
		test al, al;
		popad;
		jmp Exit;
	}
}

void InitExhaustFX()
{
	if (Config::GetGlobal()->ExhaustFX)
	{
		char disableRearBumperCheck[5] = { 0xB8, 0x01, 0x00, 0x00, 0x00 };
		injector::WriteMemoryRaw(0x007CC6BD, disableRearBumperCheck, 5, true);

		injector::MakeJMP(0x007BEBFB, FixExhaustCave, true);
	}

	injector::MakeJMP(0x007DA131, ExhaustMarkerCave, true);
	injector::MakeJMP(0x007DF2EE, ExhaustRenderByMarkerCave, true);
	injector::MakeJMP(0x007DEFC8, ExhaustRenderCave, true);
}