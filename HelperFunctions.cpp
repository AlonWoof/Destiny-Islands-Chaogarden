
#include "SADXModLoader.h"
#include "HelperFunctions.h"
#include <math.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <chrono>
#include <ctime>   
#include <sstream>

bool frameTimerActive = false;
int frameTimerFrames = 0;

void startFrameTimer()
{
	frameTimerActive = true;
	frameTimerFrames = 0;
}

void stopFrameTimer()
{
	frameTimerActive = false;
}

void drawFrameTimer()
{

	if (frameTimerFrames > 0)
		njPrint(NJM_LOCATION(32, 31), "TIMER FRAME: %i", frameTimerFrames);

	if (frameTimerActive)
		frameTimerFrames++;

}



using namespace std;

template <typename T> string tostr(const T& t) {
	ostringstream os;
	os << t;
	return os.str();
}


template <typename T> string tohexstr(const T& t) {
	ostringstream os;
	os << std::hex << t;
	return os.str();
}

float lerp(float a, float b, float f)
{
	return (a * (1.0 - f)) + (b * f);
}

NJS_VECTOR lerpVector(NJS_VECTOR a, NJS_VECTOR b, float f)
{
	NJS_VECTOR ret;

	ret.x = lerp(a.x, b.x, f);
	ret.y = lerp(a.y, b.y, f);
	ret.z = lerp(a.z, b.z, f);

	return ret;
}

float clamp(float tg, float min, float max)
{
	if (tg < min)
		return min;

	if (tg > max)
		return max;

	return tg;
}


NJS_VECTOR getPlayerSidePos(taskwk* twp, double dist, int angle_offset)
{
	double sin; // fp30

	NJS_VECTOR pos;

	pos.x = 0;
	pos.y = 0;
	pos.z = 0;

	if (twp)
	{
		sin = njSin(twp->ang.y + angle_offset);
		pos.x = -(float)((float)((float)njCos(twp->ang.y + angle_offset) * (float)dist) - twp->pos.x);
		pos.y = twp->pos.y;
		pos.z = -(float)((float)((float)sin * (float)dist) - twp->pos.z);
	}

	return pos;
}



float getVectorMagnitude(NJS_VECTOR vec)
{
	// Stores the sum of squares
	  // of coordinates of a vector
	float sum = vec.x * vec.x + vec.y * vec.y + vec.z * vec.z;

	// Return the magnitude
	return sqrt(sum);
}

float angleToFloat(Angle angle)
{
	//return ((float)angle / (360.0f)) * 2;

	return (360.0f / 65535.0f) * angle;
}

Angle floatToAngle(float degrees)
{
	return (65535.0f / 360.0f) * degrees;
}

void copyAction(PL_ACTION* original, PL_ACTION* copy)
{
	copy->actptr = original->actptr;
	copy->frame = original->frame;
	copy->mtnmode = original->mtnmode;
	copy->next = original->next;
	copy->objnum = original->objnum;
	copy->racio = original->racio;
}

void copyPlayerParameter(player_parameter* original, player_parameter* copy)
{
	copy->jump2_timer = original->jump2_timer;
	copy->pos_error = original->pos_error;
	copy->lim_h_spd = original->lim_h_spd;
	copy->lim_v_spd = original->lim_v_spd;
	copy->max_x_spd = original->max_x_spd;
	copy->max_psh_spd = original->max_psh_spd;
	copy->jmp_y_spd = original->jmp_y_spd;
	copy->nocon_speed = original->nocon_speed;
	copy->slide_speed = original->slide_speed;
	copy->jog_speed = original->jog_speed;
	copy->run_speed = original->run_speed;
	copy->rush_speed = original->rush_speed;
	copy->crash_speed = original->crash_speed;
	copy->dash_speed = original->dash_speed;
	copy->jmp_addit = original->jmp_addit;
	copy->run_accel = original->run_accel;
	copy->air_accel = original->air_accel;
	copy->slow_down = original->slow_down;
	copy->run_break = original->run_break;
	copy->air_break = original->air_break;
	copy->air_resist_air = original->air_resist_air;
	copy->air_resist = original->air_resist;
	copy->air_resist_y = original->air_resist_y;
	copy->air_resist_z = original->air_resist_z;
	copy->grd_frict = original->grd_frict;
	copy->grd_frict_z = original->grd_frict_z;
	copy->lim_frict = original->lim_frict;
	copy->rat_bound = original->rat_bound;
	copy->rad = original->rad;
	copy->height = original->height;
	copy->weight = original->weight;
	copy->eyes_height = original->eyes_height;
	copy->center_height = original->center_height;
}

const string CharNames[]
{
	"SONIC",
	"EGGMAN",
	"MILES",
	"KNUCKLES",
	"TIKAL",
	"AMY",
	"GAMMA",
	"BIG"
};

void writeParam(string paramName, float param, ofstream* outfile)
{
	string str = paramName + " = " + tostr(param) + "f;\n";
	outfile->write(str.c_str(), str.size());
}

void writeParam(string paramName, int param, ofstream* outfile)
{
	string str = paramName + " = " + tostr(param) + ";\n";
	outfile->write(str.c_str(), str.size());
}

DataArray(NJS_ACTION*, s_snowboard_action, 0x3C561B0, 23);

void writeAction(NJS_ACTION* action, ofstream* outfile)
{
	string str = "	{ " + tohexstr(&action->motion) + ", " + tohexstr(&action->object) + " }, \n";
	outfile->write(str.c_str(), str.size());
}


Float njDistanceP2P(NJS_POINT3* p1, NJS_POINT3* p2)
{
	Float y = p1->y - p2->y;
	Float x = p1->x - p2->x;
	Float z = p1->z - p2->z;
	return njSqrt(x * x + y * y + z * z);
}

void scrollUVs(NJS_TEX* tx, int tx_size, int u, int v)
{
	for (int i = 0; i < tx_size; i++)
	{
		tx[i].u += u;
		tx[i].v += v;
	}
}


