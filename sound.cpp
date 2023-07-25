
#include "sound.h"
#include <bass.h>
#include <bass_vgmstream.h>

void playCustomSound(CustomSound * snd)
{
	int sfxchan = BASS_VGMSTREAM_StreamCreateFromMemory(snd->buffer, snd->size, snd->name, BASS_STREAM_AUTOFREE);
	if (sfxchan != 0)
	{

		BASS_ChannelPlay(sfxchan, false);
		BASS_ChannelSetAttribute(sfxchan, BASS_ATTRIB_VOL, snd->volume);
		
	}


}
