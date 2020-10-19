#include <vector>
#include "Feature.h"

using namespace std;

struct Vector
{
	int x;
	int y;
	int z;
	int w;
};

struct RotationMatrix
{
	Vector x;
	Vector y;
	Vector z;
};

struct MountPoint
{
	int hash;
	int blank[3];
	RotationMatrix rotationMatrix;
	Vector position;
};

bool operator==(const Vector& a, const Vector& b)
{
	return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
}

int exhaustFxHash = GameApi::StringHash((char*)"EXHAUST_FX");
int leftExhaustHash = GameApi::StringHash((char*)"LEFT_EXHAUST");
int rightExhaustHash = GameApi::StringHash((char*)"RIGHT_EXHAUST");
int centerExhaustHash = GameApi::StringHash((char*)"CENTER_EXHAUST");
bool __stdcall CheckEmmiter(void* rearBumper, int hash1, int hash2)
{
	if (rearBumper != NULL) {
		if (!GameApi::IsStock(rearBumper))
		{
			if (hash1 == exhaustFxHash)
			{
				if (hash2 == hash1)
				{
					return false; // Disable exhaust fx from body
				}

				if (hash2 == leftExhaustHash || hash2 == rightExhaustHash || hash2 == centerExhaustHash)
				{
					return true;
				}
			}
		}
	}

	return hash1 == hash2;
}

DWORD SkipEmmiter = 0x007BEC35;
DWORD ApplyEmmiter = 0x007BEBFB;
void __declspec(naked) FixExhaustCave1()
{
	__asm
	{
		pushad;
		mov ecx, [esp + 0x38]; // +0x18 originally but shifted due to save regs
		mov ecx, [ecx + 0x000003F0];
		mov ecx, [ecx + 0x0000017C];
		push edx;
		push[eax + edi * 4];
		push ecx;
		call CheckEmmiter;
		test al, al;
		popad;

		je skip;
		jmp ApplyEmmiter;
	skip:
		jmp SkipEmmiter;
	}
}

vector<MountPoint*> exhaustMpList;
MountPoint* GetExhaustMountPoint(Vector* position)
{
	for (MountPoint* i : exhaustMpList)
	{
		if (i->position == *position)
		{
			return i;
		}
	}

	MountPoint* exhaustMP = new MountPoint();
	exhaustMP->rotationMatrix.x.z = 0x3F800000;
	exhaustMP->rotationMatrix.y.y = 0x3F800000;
	exhaustMP->rotationMatrix.z.x = 0xBF800000;
	exhaustMP->rotationMatrix.z.w = 0;

	exhaustMP->position = *position;
	exhaustMpList.push_back(exhaustMP);

	return exhaustMP;
}

void __stdcall CopyMountPointForExhaust(MountPoint* original, int nmem)
{
	Vector* newMem = (Vector*)(nmem + 4);
	*newMem = original->position;

	int hash = original->hash;
	int addr;
	if (hash == leftExhaustHash || hash == rightExhaustHash || hash == centerExhaustHash)
	{
		addr = (int)GetExhaustMountPoint(&original->position);
	}
	else
	{
		addr = (int)original;
	}

	int* ptr = (int*)&(newMem->w);
	*ptr = addr;
}

DWORD FixExhaust2 = 0x007BEC27;
void __declspec(naked) FixExhaustCave2()
{
	__asm
	{
		pushad;
		push eax;
		push esi;
		call CopyMountPointForExhaust;
		popad;
		jmp FixExhaust2;
	}
}

void InitExhaustFX()
{
	char disableRearBumperCheck[5] = { 0xB8, 0x01, 0x00, 0x00, 0x00 };
	injector::WriteMemoryRaw(0x007CC6BD, disableRearBumperCheck, 5, true);

	injector::MakeJMP(0x007BEBF6, FixExhaustCave1, true);
	injector::MakeJMP(0x007BEC12, FixExhaustCave2, true);
}