#pragma once
#ifndef _RD_DESTINYISLANDS_H_
#define _RD_DESTINYISLANDS_H_

#include <SADXModLoader.h>
#include "DecompFunctions.h"
#include "HelperFunctions.h"

void AL_Garden01MasterCustom(task* tp);
int AL_CheckWater_custom(task* tp);
void draw_di_waves0(task* tp);
void draw_di_waves1(task* tp);
void draw_di_sky_clouds(task* tp);
void draw_di_sky(task* tp);
void draw_di_waterfall(task* tp);
void draw_di_waterflow(task* tp);
void draw_di_water(task* tp);

void DI_WriteFunctions();
void DI_InitPositions();
void bg_destinyIslands(task* tp);
void rd_destinyIslands(task* tp);




#endif // !_RD_DESTINYISLANDS_H_



