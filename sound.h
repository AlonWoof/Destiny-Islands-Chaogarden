#pragma once
#ifndef ALON_SOUND_H_
#define ALON_SOUND_H_

#include <SADXModLoader.h>

typedef struct CustomSound
{
	unsigned char* buffer;
	int size;
	const char* name;
	float volume;
} CustomSound;

void playCustomSound(CustomSound* snd);

#endif