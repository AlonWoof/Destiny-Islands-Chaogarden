
#include <SADXModLoader.h>
#include "HelperFunctions.h"
#include "rd_destinyislands.h"
#include "AlonWoofProductionsSave.h"

MusicInfo bgm_DestinyIslands = { "DestinyIslands", 1 };
int MusicID_DestinyIslands = 0;


extern "C"
{
	__declspec(dllexport) ModInfo SADXModInfo = { ModLoaderVer };

	__declspec(dllexport) void __cdecl Init(const char* path, const HelperFunctions& helperFunctions)
	{
		DI_WriteFunctions();
		DI_InitPositions();
		
		MusicID_DestinyIslands = helperFunctions.RegisterMusicFile(bgm_DestinyIslands);

		
	}

	__declspec(dllexport) void OnFrame()
	{
		//njPrint(NJM_LOCATION(10, 10), "Look at this tiny place...");
		//njPrint(NJM_LOCATION(10, 11), "To the heart seeking freedom, this island is a prison, surrounded by water.");
		//njPrint(NJM_LOCATION(10, 12), "And so this girl sought to escape her prison, and opened her heart to darkness...");
	}
}

