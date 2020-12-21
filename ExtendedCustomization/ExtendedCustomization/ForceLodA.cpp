#include "ForceLodA.h"
#include "Config.h"

void MakeLod(char* str, char lod)
{
	//if (strstr(str, "DAMAGE") == NULL)
	{
		int len = strlen(str);
		str[len - 1] = 'A';
	}
}

int __cdecl ChangePartString(char* str, int modelHash)
{
	auto config = Config::GetByHash(modelHash);
	if (config->ForceLodA)
	{
		MakeLod(str, 'A');
	}

	return Game::StringHashModel(str, modelHash);
}

void __declspec(naked) HookPartLoadCave()
{
	static constexpr auto Exit = 0x007CDC8D;

	__asm
	{
		call ChangePartString;
		add esp, 0x8;
		jmp Exit;
	}
}

void InitForceLodA()
{
	injector::MakeJMP(0x007CDC85, HookPartLoadCave, true);
}