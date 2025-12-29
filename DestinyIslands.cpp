
#include <SADXModLoader.h>
#include "HelperFunctions.h"
#include "rd_destinyislands.h"
#include "AlonWoofProductionsSave.h"
#include "IniFile.hpp"

MusicInfo bgm_DestinyIslands = { "DestinyIslands", 1 };
MusicInfo bgm_DestinyIslands_orig = { "DestinyIslands_orig", 1 };
int MusicID_DestinyIslands = 0;

bool useOrigMusic = false;

extern "C"
{
	__declspec(dllexport) ModInfo SADXModInfo = { ModLoaderVer };

	__declspec(dllexport) void __cdecl Init(const char* path, const HelperFunctions& helperFunctions)
	{
		const IniFile* config = new IniFile(std::string(path) + "\\config.ini");
		useOrigMusic = config->getBool("Settings", "useOrigMusic", false);

		DI_WriteFunctions();
		DI_InitPositions();
		
		if(!useOrigMusic)
			MusicID_DestinyIslands = helperFunctions.RegisterMusicFile(bgm_DestinyIslands);
		else
			MusicID_DestinyIslands = helperFunctions.RegisterMusicFile(bgm_DestinyIslands_orig);
		
	}

	__declspec(dllexport) void OnFrame()
	{
		//njPrint(NJM_LOCATION(10, 10), "Look at this tiny place...");
		//njPrint(NJM_LOCATION(10, 11), "To the heart seeking freedom, this island is a prison, surrounded by water.");
		//njPrint(NJM_LOCATION(10, 12), "And so this girl sought to escape her prison, and opened her heart to darkness...");
	}
}

