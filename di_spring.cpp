#include <SADXModLoader.h>
#include "HelperFunctions.h"
#include "DecompFunctions.h"

enum {
	SPRINGMD_INIT,
	SPRINGMD_NORMAL,
	SPRINGMD_LAUNCH,
	SPRINGMD_END
};

CCL_INFO spr_colli_info[1] = { {0, CI_FORM_CYLINDER, CI_PUSH_TH_ALL, CI_DMG_ALL, 0, { 0.0f, 0.0f, 0.0f }, 6.0f, 10.0f, 0.0f, 0.0f, 0, 0, 0} };
CCL_INFO spr_colli_info_b[1] = { {0, CI_FORM_SPHERE, CI_PUSH_TH_ALL, CI_DMG_ALL, 0, { 0.0f, 0.0f, 0.0f }, 14.0f, 0.0f, 0.0f, 0.0f, 0, 0, 0} };

Bool GetPlayerRunningSpeed(Uint8 pno, Float* v)
{
	taskwk* twp = playertwp[pno];
	if (twp && twp->flag & PL_FLAG_FLOOR)
	{
		*v = playerpwp[pno]->spd.x;
		return TRUE;
	}
	return FALSE;
}

void SetVelocityYAndRotationAndNoconTimeP(Uint8 pno, Float y, Angle3* ang, Sint32 tm)
{
	taskwk* twp = playertwp[pno];
	motionwk2* mwp = playermwp[pno];
	playerwk* pwp = playerpwp[pno];
	pwp->nocontimer = (Sint16)tm;
	SetInputP(pno, PL_OP_CANON);

	NJS_VECTOR v;
	v.x = 0.0f;
	v.y = y;
	v.z = 0.0f;
	pwp->spd = v;

	twp->ang.x = ang->x;
	twp->ang.y = 0x4000 - ang->y;
	mwp->ang_aim.y = 0x4000 - ang->y;
	twp->ang.z = ang->z;

	PConvertVector_P2G(twp, &v);
	mwp->spd = v;
}


void VacumePlayer(task* tp, Uint8 flag)
{
	taskwk* twp = tp->twp;
	Sint8 pnum = twp->smode;
	taskwk* ptwp = playertwp[pnum];

	float baseForce = tp->awp->work.f[0];

	if (!ptwp)
		return;

	NJS_VECTOR v = { 0.0f, baseForce, 0.0f };
	NJS_VECTOR dir;
	v.y = baseForce;
	njPushMatrix(_nj_unit_matrix_);
	ROTATEZ(0, twp->ang.z);
	ROTATEX(0, twp->ang.x);
	ROTATEY(0, twp->ang.y);
	njCalcPoint(0, &v, &dir);
	njPopMatrix(1);

	NJS_POINT3 tgtpos;
	tgtpos.x = dir.x + twp->pos.x;
	tgtpos.y = dir.y + twp->pos.y;
	tgtpos.z = dir.z + twp->pos.z;

	Float pspd;
	GetPlayerRunningSpeed(pnum, &pspd);
	if (pspd >= 1.0f && twp->counter.f == 0.1f)
	{
		// These SetInputP calls are completely useless
		SetInputP(pnum, PL_OP_PLACEON);
		ptwp->pos = tgtpos;
		SetInputP(pnum, PL_OP_LETITGO);

		if (flag == 0)
		{
			SetSpringVelocityP(pnum, dir.x, dir.y, dir.z);
		}
		else
		{
			if (twp->scl.x <= 0.0f)
				twp->scl.x = 0.0f;

			Angle3 ang3 = twp->ang;
			SetVelocityYAndRotationAndNoconTimeP(pnum, baseForce, &ang3, (Sint32)twp->scl.x);
		}
	}
	else
	{
		Float x = tgtpos.x - ptwp->pos.x;
		Float z = tgtpos.z - ptwp->pos.z;
		Float dist = njHypot(x, z);
		if (dist < 14.0f && dist >= 2.0f)
		{
			SetInputP(pnum, PL_OP_PLACEON);
			Float factor = 1.0f / dist;
			ptwp->pos.x += x * factor + x * factor;
			ptwp->pos.z += z * factor + z * factor;
			SetInputP(pnum, PL_OP_LETITGO);
		}

		if (flag == 0)
		{
			SetSpringVelocityP(pnum, dir.x, dir.y, dir.z);
		}
		else
		{
			ptwp->pos.x = tgtpos.x;
			ptwp->pos.y = dir.y * 0.8f + twp->pos.y;
			ptwp->pos.z = tgtpos.z;

			if (twp->scl.x <= 0.0f)
				twp->scl.x = 0.0f;

			Angle3 ang3 = twp->ang;
			SetVelocityYAndRotationAndNoconTimeP(pnum, baseForce, &ang3, (Sint32)twp->scl.x);
		}
	}

	twp->mode = SPRINGMD_NORMAL;
	twp->counter.f = 0.1f;
	VibShot(pnum, 0);
}


void SpringDie(task* tp)
{
}

void SpringDieB(task* tp)
{
}

void ObjectSpring_DI(task* tp)
{

	taskwk* twp = tp->twp;

	switch (twp->mode)
	{
	case SPRINGMD_INIT:
		tp->dest = SpringDie;
		tp->disp = DrawSpring;
		CCL_Init(tp, spr_colli_info, 1, CID_OBJECT);
		twp->cwp->flag |= CWK_FLAG_HOMING;
		twp->scl.x = 180.0f;
		twp->scl.z = 0.0f;
		twp->value.f = 0.5f;
		twp->btimer = 10;
		twp->mode = SPRINGMD_NORMAL;
		break;
	case SPRINGMD_NORMAL:
		twp->counter.f = -njAbs(njSin(NJM_DEG_ANG(twp->scl.x)) * twp->value.f);

		if (OnEdit(tp))
		{
			njPrintC(NJM_LOCATION(11, 13), "<- NO TIMER");
			njPrintC(NJM_LOCATION(11, 14), "<- ADD YSPD");
		}

		if (!CheckEditMode())
		{
			twp->scl.x += twp->scl.z;
			if (twp->scl.x >= 290.0f && twp->scl.x < (twp->scl.z + 290.0f))
				twp->scl.z = 18.0f;
			twp->value.f = njCos(NJM_DEG_ANG((twp->scl.x - 180.0f) * 0.1f)) * 0.6f;
			if (twp->value.f < 0.0f)
			{
				twp->scl.x = 180.0f;
				twp->scl.z = 0.0f;
			}

			if (++twp->btimer > 10)
			{
				if (twp->cwp->flag & CWK_FLAG_HIT)
				{
					twp->cwp->flag &= ~CWK_FLAG_HIT;

					for (int i = 0; i < 2; ++i)
					{
						if (twp->cwp->hit_cwp->mytask == playertp[i])
						{
							twp->counter.f = 0.1f;
							twp->smode = i;
							twp->scl.z = 20.0f;
							twp->scl.x = 270.0f;
							twp->mode = SPRINGMD_LAUNCH;
							DrawSpring(tp);
							return;
						}
					}
				}
				EntryColliList(twp);
			}

			if (twp->btimer > 50)
			{
				twp->btimer = 50;
			}
		}

		DrawSpring(tp);
		break;
	case SPRINGMD_LAUNCH:
		twp->btimer = 0;
		VacumePlayer(tp, 0);
		DrawSpring(tp);
		dsPlay_oneshot(SE_SPRING, 0, 0, 0);
		break;
	case MD_END:
		DeadOut(tp);
		break;
	default:
		break;
	}
}

