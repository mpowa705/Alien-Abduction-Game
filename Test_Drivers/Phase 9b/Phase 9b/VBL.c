#include"VBL.h"
#include"globals.h"
#include"PSG.h"
#include"model.h"
#include<osbind.h>

Vector install_vector(int num, Vector vector)
{
	Vector orig;
	Vector *vectp = (Vector *)((long)num << 2);
	long old_ssp = Super(0);
	orig = *vectp;
	*vectp = vector;
	Super(old_ssp);
	return orig;
}

void do_VBL_ISR()
{
	renderrequest = TRUE;
	timenow++;
	if(playmusic == TRUE)
	{
		if(currentnote == -1)
		{ 
		enable_channel(CHANNEL_A, ON, OFF);
		set_tone(CHANNEL_A, MUSIC[0]);
		set_volume(CHANNEL_A,9);
	/*	set_noise(0); */
		nextnote = timenow + MUSIC[1];
		currentnote = 0;
		}
		else
		{
            if(timenow >= nextnote)
            {
                currentnote = (currentnote + 1) % 60;
                set_noise(0);
                set_tone(CHANNEL_A, MUSIC[currentnote*2]);
                nextnote = timenow + MUSIC[(currentnote*2)+1];
            }
		}

	}	
	return;
}