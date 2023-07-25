#pragma once
#ifndef _DECOMPFUNCTIONS_H_
#define _DECOMPFUNCTIONS_H_

#include <SADXModLoader.h>

FunctionPointer(void, AlMsgFontInit, (), 0x724E60);
VoidFunc(AL_MinimalInit, 0x722520);
VoidFunc(AL_CreateMinimalCreateManager, 0x722500);
VoidFunc(AL_SetChaoOnTheGarden, 0x715F20);
VoidFunc(AL_SetObjectOnTheGarden, 0x717BA0);
VoidFunc(AL_CreateHoldingChao, 0x716200);
VoidFunc(AL_CreateHoldingItem, 0x717C80);
VoidFunc(ALO_XYButtonCreate, 0x71B3B0);
VoidFunc(AL_SeqArrive, 0x7290B0);

TaskFunc(AL_Garden01MasterExecutor, 0x718FD0);
TaskFunc(AL_Garden01MasterDestructor, 0x719010);
TaskFunc(SetObject, 0x46C3D0);
TaskFunc(ObjectFinalEggHasigo, 0x5B4D00);
TaskFunc(PathworkLaddering, 0x4BB830);
FunctionPointer(signed int, ALBHV_Swim, (task* tp), 0x73C110);
FunctionPointer(task*, ALO_CreateGrowTree, (NJS_VECTOR* pos, TREE_SAVE_INFO* pInfo), 0x721800);

DataArray(Sint32[40], LandAttrEC, 0x33A2CD0, 40);

struct POS_XZ
{
	int x;
	int z;
};

struct LAND_ATTR_INDEX
{
	int nbIndex;
	POS_XZ pos[1024];
};


#endif