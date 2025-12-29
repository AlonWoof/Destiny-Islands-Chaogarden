
#include <SADXModLoader.h>
#include "rd_destinyislands.h"

#include "di_sky.h"
#include "di_water.h"
#include "di_waterfall.h"
#include "di_waterflow.h"
#include "di_waves.h"
#include "di_foliage.h"
#include "di_spring.h"
#include "sound.h"
#include "destinyIslands_sfx.h"
#include "AlonWoofProductionsSave.h"

enum
{
	MD_DI_LOAD,
	MD_DI_INIT,
	MD_DI_EXEC,
	MD_DI_DESTROY
};

NJS_POINT3 OdekakeMachinePos = { -69.0f, 17.0f, 88.0f };
NJS_POINT3 WarpPos_MR = { 288.0f, 63.87f, -242.62f };
NJS_POINT3 WarpPos_SS = { 279.0f, 63.87f, -268.78f };
NJS_POINT3 WarpPos_Quit = { -87.0f, 25.0f, 260.0f };

struct DI_SPRING_ATR
{
	NJS_POINT3 pos;
	float baseForce;
};

DI_SPRING_ATR SpringPos[] =
{
	{{273.0f, 42.5f, -79.4f}, 3.5f},
	{{-395.5f, 2.4f, -33.12f}, 3.0f},
	{{152.58f, 86.9f, 72.2f}, 3.0f}
};

static int nbSpring = SizeOfArray(SpringPos);

AL_ST_POS AL_StartPosDI_DI = { {-142.4f,25.05f,259.8f}, 44431 };
AL_ST_POS AL_StartPosDI_Warp = { {256.0f, 64.244f, -245.19f}, 29712 };
AL_ST_POS AL_StartPosDI_Odekake = { {-142.4f,25.05f,259.8f}, 44431 };
AL_ST_POS DI_ReturnPos = { -175.0f,185.0f,-141.0f, 0 };
NJS_POINT3 DI_Center = { -97.7786f, 62.1264f, 49.8259f };

Sint32 LandAttrDI[40][40] = {
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 9, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 9, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 9, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 9, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 9, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 9, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 9, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 9, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 9, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 9, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 9, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 9, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 9, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 9, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 9, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 9, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 9, 9, 9, 9, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 9, 9, 9, 9, 9, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 9, 9, 9, 9, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 9, 9, 9, 9, 9, 9, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 9, 9, 9, 9, 9, 9, 9, 9, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

extern int MusicID_DestinyIslands;

LAND_ATTR_INDEX DI_WAYPOINTS
{
	2,
	{
		{0,0},
		{0,1}
	}
};

NJS_POINT3 ChaoSetPositionDI[16] =
{
	{33,16,-76},
	{-357,38,40},
	{-56,17,68},
	{-101,15,60},
	{-75,15,13},
	{-40,16,-37},
	{62,33,88},
	{-108,58,153},
	{-308,38,11},
	{-371,38,-11},
	{-191,15,228},
	{231,43,-62},
	{85,28,-125},
	{145,88,88},
	{233,123,42},
	{-202,58,101}
};


NJS_POINT3 DI_TreePos[10] =
{
	{-350.626f,37.1319f, 13.6635f},
	{-353.402f, 37.302f, -27.3965f},
	{-92.5175f, 57.1247f, 137.35f},
	{-122.828f, 57.1247f, 191.844f},
	{-388.712f, 35.0f, 34.6068f},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{150,0,0},
	{100,0,0}
};

NJS_TEXNAME DestinyIslands_TexName[56];
NJS_TEXLIST DestinyIslands_TexList{ DestinyIslands_TexName, 56 };

NJS_TEXNAME texname_di_waves0[32];
NJS_TEXLIST texlist_di_waves0 = { texname_di_waves0, 32 };

NJS_TEXLIST texlist_di_waves0_frames[] =
{
	{&texname_di_waves0[1], 1},
	{&texname_di_waves0[2], 1},
	{&texname_di_waves0[3], 1},
	{&texname_di_waves0[4], 1},
	{&texname_di_waves0[5], 1},
	{&texname_di_waves0[6], 1},
	{&texname_di_waves0[7], 1},
	{&texname_di_waves0[8], 1},
	{&texname_di_waves0[9], 1},
	{&texname_di_waves0[10], 1},
	{&texname_di_waves0[11], 1},
	{&texname_di_waves0[12], 1},
	{&texname_di_waves0[13], 1},
	{&texname_di_waves0[14], 1},
	{&texname_di_waves0[15], 1}

};

NJS_TEXLIST texlist_di_waves1_frames[] =
{
	{&texname_di_waves0[17], 1},
	{&texname_di_waves0[18], 1},
	{&texname_di_waves0[19], 1},
	{&texname_di_waves0[20], 1},
	{&texname_di_waves0[21], 1},
	{&texname_di_waves0[22], 1},
	{&texname_di_waves0[23], 1},
	{&texname_di_waves0[24], 1},
	{&texname_di_waves0[25], 1},
	{&texname_di_waves0[26], 1},
	{&texname_di_waves0[27], 1},
	{&texname_di_waves0[28], 1},
	{&texname_di_waves0[29], 1},
	{&texname_di_waves0[30], 1},
	{&texname_di_waves0[31], 1}
};

void DI_WriteFunctions()
{
	WriteJump((void*)0x73C200, AL_CheckWater_custom);
	WriteJump((void*)0x719040, AL_Garden01MasterCustom);

}

void DI_InitPositions()
{
	AL_StartPosEC_EC = AL_StartPosDI_DI;
	AL_StartPosEC_Warp = AL_StartPosDI_Warp;
	AL_StartPosEC_Odekake = AL_StartPosDI_Odekake;

	for (int i = 0; i < 10; i++)
	{
		TreeSetPos[1][i] = DI_TreePos[i];
	}

	for (int i = 0; i < 16; i++)
	{
		ChaoSetPositionEC[i] = ChaoSetPositionDI[i];
	}

	for (int i = 0; i < 40; i++)
	{
		for (int j = 0; j < 40; j++)
		{
			LandAttrEC[i][j] = LandAttrDI[i][j];
			LandAttrEC[i][j] = 1;
		}
	}

}

void AL_Garden01MasterCustom(task* tp)
{
	int v2; // r5
	int v3; // r5
	NJS_POINT3 v4; // [sp+50h] [-30h] BYREF

	___njSetBackColor(0xFF000000, 0xFF000000, 0xFF000000);
	gFog.u8Enable = 0;
	ALCAM_CreateCameraManager();
	AL_CreateNormalCameraTask();
	AlMsgFontInit();
	AL_LoadTex("AL_TEX_COMMON", &texlist_al_tex_common, 1u);
	AL_LoadTex("AL_DX_ETC", &AL_DX_ETC_TEXLIST, 1u);
	AL_LoadTex("AL_OBJECT", &texlist_al_object, 1u);
	AL_LoadTex("AL_TOY", &AL_TOY_TEXLIST, 1u);
	AL_LoadTex("AL_DX_OBJ_CMN", &AL_DX_OBJ_CMN_TEXLIST, 1u);
	AL_LoadTex("al_dx_tex_xybutton", &al_dx_tex_xybutton_TEXLIST, 1u);
	AL_MinimalInit();
	AL_CreateMinimalCreateManager();
	ALW_Create();
	AL_SetChaoOnTheGarden();
	AL_SetObjectOnTheGarden();
	ALO_OdekakeMachineCreate(&OdekakeMachinePos, floatToAngle(90.0f));
	ALO_WarpCreate(tp, CHAO_STG_MR, WarpPos_MR.x, WarpPos_MR.y, WarpPos_MR.z, floatToAngle(290.0f));
	ALO_WarpCreate(tp, CHAO_STG_SS, WarpPos_SS.x, WarpPos_SS.y, WarpPos_SS.z, floatToAngle(290.0f));
	ALO_WarpCreate(tp, CHAO_STG_QUIT, WarpPos_Quit.x, WarpPos_Quit.y, WarpPos_Quit.z, floatToAngle(0.0f));
	AL_CreateHoldingChao();
	AL_CreateHoldingItem();
	//CreateElementalTask(8u, 6, AL_G01_SeaScrollExec);
	ALO_XYButtonCreate();
	AL_SeqArrive();
	ADXTaskInit();
	BGM_Play(MusicID_DestinyIslands);
	tp->exec = AL_Garden01MasterExecutor;
	tp->dest = AL_Garden01MasterDestructor;
	tp->disp = 0;
	gClipMap.f32Near = -1.0;
	gClipMap.f32Far = -5000.0;

	CreateElementalTask(IM_TASKWK, LEV_1, bg_destinyIslands);
	CreateElementalTask(IM_TASKWK, LEV_1, rd_destinyIslands);

	incrementVisitCount();
}

int AL_CheckWater_custom(task* tp)
{
	taskwk* twp; // r31
	motionwk* mwp; // r29
	int result; // r3
	eCHAO_STAGE_NUMBER v5; // r3
	double water_level; // fp0
	taskwk* v7; // r11
	double v8; // fp11
	__int16 v9; // r11

	twp = tp->twp;
	mwp = tp->mwp;
	if ((twp->flag & 0x1000) != 0)
	{
		result = 0;
		twp[7].flag &= 0xFFFEu;
	}
	else
	{
		v5 = AL_GetStageNumber();
		if (v5 == CHAO_STG_SS)
		{
			water_level = -0.60000002;
		}
		else if (v5 == CHAO_STG_MR)
		{
			water_level = -0.0;
		}
		else if (v5 == CHAO_STG_EC)
		{

			water_level = 7.87962;


			NJS_VECTOR pond_pos = { 92.569f, 26.6823f, 167.955f };

			float dist = njDistanceP2P(&tp->twp->pos, &pond_pos);

			if (dist < 65.0f)
				water_level = 26.8302;
		}
		else
		{
			water_level = -10000.0;
		}
		v7 = tp->twp;
		*(float*)&mwp[3].ang_aim.x = water_level;
		v8 = (float)(v7->pos.y + (float)2.0);
		v9 = twp[7].flag;
		if (water_level <= v8)
		{
			result = 0;
			twp[7].flag = v9 & 0xFFFA;
		}
		else
		{
			if ((v9 & 1) == 0)
			{
				twp[7].flag = v9 | 1;
				AL_SetBehavior(tp, (void(__cdecl*)(task*))ALBHV_Swim);
			}
			if (mwp->spd.y < 0.0)
				mwp->spd.y = mwp->spd.y * (float)0.1;
			result = 1;
			twp[7].flag |= 4u;
		}
	}
	return result;
}

void draw_di_waves0(task* tp)
{
	taskwk* twp = tp->twp;

	if (loop_count)
		return;


	late_z_ofs___ = -17000.0f;

	njPushMatrix(0);

	int frame = tp->twp->counter.w[0];

	if (frame > 14)
		frame = 0;

	njSetTexture(&texlist_di_waves0_frames[frame]);
	//dsDrawObject(&asphodel_fx_distortion);
	late_DrawObject(&di_waves0, LATE_WZ);
	njPopMatrix(1);

	late_z_ofs___ = 0.0f;

	ResetMaterial();
}

void draw_di_waves1(task* tp)
{
	taskwk* twp = tp->twp;

	if (loop_count)
		return;

	njPushMatrix(0);
	njSetTexture(&texlist_di_waves1_frames[twp->btimer]);
	//dsDrawObject(&asphodel_fx_distortion);
	late_DrawObject(&di_waves1, LATE_WZ);
	njPopMatrix(1);


	ResetMaterial();
}



void draw_di_sky_clouds(task* tp)
{
	late_z_ofs___ = -17000.0f;

	___njFogDisable();
	njPushMatrix(0);
	njTranslate(0, camera_twp->pos.x, 0.0f, camera_twp->pos.z);
	ROTATEY(0, tp->twp->ang.x);
	tp->twp->ang.x += 3;
	njSetTexture(&DestinyIslands_TexList);
	//njScaleV(0, &gSkyScale);
	late_DrawObject(&di_sky_2, LATE_LIG);
	njScale(0, 1.0f, 1.0f, 1.0f);
	njPopMatrix(1);
	___njFogEnable();
	___njClipZ(gClipMap.f32Near, gClipMap.f32Far);
	ResetMaterial();

	late_z_ofs___ = 0.0f;
}

void draw_di_sky(task* tp)
{
	//___njClipZ(gClipSky.f32Near, gClipSky.f32Far);
	//late_z_ofs___ = -17000.0f;
	___njFogDisable();
	njPushMatrix(0);
	njTranslate(0, camera_twp->pos.x, 0.0f, camera_twp->pos.z);
	ROTATEY(0, tp->twp->ang.y);

	if(FrameCounter % 2 == 0)
		tp->twp->ang.y++;

	njSetTexture(&DestinyIslands_TexList);
	//njScaleV(0, &gSkyScale);
	dsDrawObject(&sky_g_1_1dfd60_1dfe20);
	//late_DrawObject(&sky_g_1_1dfd60_1dfe20, LATE_MAT);
	njScale(0, 1.0f, 1.0f, 1.0f);
	njPopMatrix(1);
	___njFogEnable();
	___njClipZ(gClipMap.f32Near, gClipMap.f32Far);
	ResetMaterial();
	//late_z_ofs___ = 0.0f;

	draw_di_sky_clouds(tp);
}



void draw_di_waterfall(task* tp)
{
	if (!loop_count)
	{
		scrollUVs(di_waterfall_mesh_uv0, 508, 0, 3);

		njPushMatrix(0);
		njTranslate(0, 0.0f, 0.0f, 0.0f);
		ROTATEY(0, 0);
		njSetTexture(&DestinyIslands_TexList);
		late_DrawObject(&di_waterfall, LATE_MAT);
		njScale(0, 1.0f, 1.0f, 1.0f);
		njPopMatrix(1);
		ResetMaterial();
	}
}

void draw_di_foliage(task* tp)
{

	if (!loop_count)
	{

		njPushMatrix(0);
		njTranslate(0, 0.0f, 0.0f, 0.0f);
		ROTATEY(0, 0);
		njSetTexture(&DestinyIslands_TexList);
		late_DrawObject(&di_foliage, LATE_WZ);
		//DrawObject(&di_foliage);
		njScale(0, 1.0f, 1.0f, 1.0f);
		njPopMatrix(1);
		ResetMaterial();

	}
}


void draw_di_waterflow(task* tp)
{
	if (!loop_count)
	{
		scrollUVs(di_waterflow_mesh_uv1, 840, 0, 3);

		njPushMatrix(0);
		njTranslate(0, 0.0f, 0.0f, 0.0f);
		ROTATEY(0, 0);
		njSetTexture(&DestinyIslands_TexList);
		late_DrawObject(&di_waterflow, LATE_MAT);
		njScale(0, 1.0f, 1.0f, 1.0f);
		njPopMatrix(1);
		ResetMaterial();
	}
}

void draw_di_water(task* tp)
{
	if (!loop_count)
	{
		if (FrameCounter % 2 == 0)
			scrollUVs(di_water_mesh_uv0, 2084, 0, 1);

		njPushMatrix(0);
		njTranslate(0, 0.0f, 0.0f, 0.0f);
		ROTATEY(0, 0);
		njSetTexture(&DestinyIslands_TexList);
		late_DrawObject(&di_water, LATE_MAT);
		njScale(0, 1.0f, 1.0f, 1.0f);
		njPopMatrix(1);
		ResetMaterial();
	}

}

void bg_destinyIslands(task* tp)
{

	taskwk* twp = tp->twp;

	if (!twp->mode)
	{
		LoadPVM("DI_WAVES", &texlist_di_waves0);
		LoadPVM("DESTINY_ISLANDS", &DestinyIslands_TexList);
		
		tp->disp = bg_destinyIslands;
		//TreeSetPos

		twp->mode++;
	}
	else
	{
		

		draw_di_waves0(tp);
		draw_di_waves1(tp);
		draw_di_water(tp);
		draw_di_waterfall(tp);
		draw_di_waterflow(tp);
		draw_di_foliage(tp);
		draw_di_sky(tp);


		if (FrameCounter % 8 == 0)
		{
			if (!tp->twp->value.b[0])
				tp->twp->btimer++;
			else
				tp->twp->btimer--;

			tp->twp->counter.w[0]++;

			if (tp->twp->counter.w[0] > 27)
				tp->twp->counter.w[0] = 0;
		}


		if (tp->twp->btimer == 14)
			tp->twp->value.b[0] = TRUE;
		else if (tp->twp->btimer == 0)
			tp->twp->value.b[0] = FALSE;
	}

}

void di_keepPlayerInBounds()
{
	for (int i = 0; i < 8; i++)
	{
		taskwk* ptwp = playertwp[i];

		if (ptwp)
		{
			float dist = njDistanceP2P(&DI_Center, &ptwp->pos);

			if (dist > 580.0f || ptwp->pos.y < -15.0f)
			{
				ptwp->pos = DI_ReturnPos.pos;
				ptwp->ang.y = DI_ReturnPos.angy;
			}

			
		}
	}
}

enum
{
	MD_INIT,
	MD_EXEC
};

void ambientWaveSound(task* tp)
{
	tp->exec = ambientWaveSound;

	

	if (FrameCounter % 200 == 0)
	{
		playCustomSound(&sfx_di_waves);
	}

	if (!tp->twp->value.w[0])
	{
		playCustomSound(&sfx_di_waves);
		tp->twp->value.w[0] = 220;
	}

	if (FrameCounter % tp->twp->value.w[0] == 0)
	{
		playCustomSound(&sfx_di_seagulls);
		tp->twp->value.w[0] = rand() % (5000 - 500 + 1) + 500;
	}

}

void drawDebugSphere(NJS_POINT3 pos)
{
	SetMaterial(1, 1, 1, 1);
	NJS_VECTOR scale = { 0.1f, 0.1, 0.1 };
	njPushMatrix(_nj_current_matrix_ptr_);
	njTranslate(_nj_current_matrix_ptr_, pos.x, pos.y, pos.z);
	njScaleV(0, &scale);
	DrawObject(&Sphere_Model);
	njPopMatrix(1);
	ResetMaterial();
}

void dbg_drawChaoSpawns()
{
	for (int i = 0; i < 16; i++)
	{
		drawDebugSphere(ChaoSetPositionDI[i]);
	}
}

task* SpringA = NULL;
task* SpringB = NULL;

task* Springs[3];

void setObjects()
{
	
	CreateElementalTask(IM_TASKWK, LEV_2, ambientWaveSound);

	for (int i = 0; i < nbSpring; i++)
	{
		task* spring = CreateElementalTask(IM_TASKWK | IM_ANYWK, LEV_3, ObjectSpring_DI);
		spring->twp->pos = SpringPos[i].pos;
		spring->awp->work.f[0] = SpringPos[i].baseForce;
	}

}

void rd_destinyIslands(task* tp)
{
	taskwk* twp = tp->twp;

	if (twp->counter.w[0] < 500)
	{
		twp->counter.w[0]++;
		//return;
	}

	switch (twp->mode)
	{

	case MD_DI_INIT:

		setObjects();
		twp->mode++;
		return;

	case MD_DI_EXEC:

		di_keepPlayerInBounds();
		//dbg_drawChaoSpawns();
		return;

	default:
		twp->mode++;
		return;
	}

}
