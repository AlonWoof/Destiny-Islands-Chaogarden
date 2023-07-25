#pragma once

#ifndef HELPERFUNCTIONS_H
#define HELPERFUNCTIONS_H

void startFrameTimer();
void stopFrameTimer();
void drawFrameTimer();

float lerp(float a, float b, float f);
float clamp(float tg, float min, float max);
NJS_VECTOR lerpVector(NJS_VECTOR a, NJS_VECTOR b, float f);
NJS_VECTOR getPlayerSidePos(taskwk* twp, double dist, int angle_offset = 0);
float getVectorMagnitude(NJS_VECTOR vec);
float angleToFloat(Angle angle);
Angle floatToAngle(float degrees);
void copyAction(PL_ACTION* original, PL_ACTION* copy);
void copyPlayerParameter(player_parameter* original, player_parameter* copy);
Float njDistanceP2P(NJS_POINT3* p1, NJS_POINT3* p2);
void scrollUVs(NJS_TEX* tx, int tx_size, int u, int v);

#define SHORT_ANG(ang) ((ang) & 0xFFFF)
#define ROTATEX(m, ang) if (ang != 0) njRotateX(m, SHORT_ANG(ang));
#define ROTATEY(m, ang) if (ang != 0) njRotateY(m, SHORT_ANG(ang));
#define ROTATEZ(m, ang) if (ang != 0) njRotateZ(m, SHORT_ANG(ang));

#define PNUM(twp) twp->counter.b[0]
#define PLNO(twp) twp->counter.b[1] 

#define IM_MOTIONWK 0x1
#define IM_TASKWK   0x2
#define IM_FORCEWK  0x4
#define IM_ANYWK    0x8



#endif // !HELPERFUNCTIONS_H
