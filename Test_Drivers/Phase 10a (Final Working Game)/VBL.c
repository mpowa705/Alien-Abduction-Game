/*Team: Matthew Powaschuk, Brady Kirchner
COMP 2659 - Winter 2016
Prof: Paul Pospisil
Project: "Alien Abduction"
Details: All source code by Matt
*/

#include"VBL.h"
#include"globals.h"
#include"PSG.h"
#include"model.h"
#include<osbind.h>

/*
Purpose: This installs an ISR into the given vector

This function was taken from a COMP2659 lab,
written up by Paul Pospisil.
*/
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

/*
Purpose: This is the ISR for the vertical blank of the display.

Details: This ISR controls music by manipulating globals, and 
controls key_repetition in the same way. Also, it increments
a global timenow variable which is used to time allocmem
synchronous events.
*/
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
		set_noise(0);
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
	if(holdingdown == TRUE)
	{
		
		if((timenow >= keyrepeatnext) && (timenow >= keyrepeatdelay) && (fillLevel <= (CHAR_BUFFER_SIZE-1)))
		{
			if(IKBD_tailPtr == &(IKBD_buffer[CHAR_BUFFER_SIZE-1]))
                IKBD_tailPtr = &(IKBD_buffer[0]);
            else
                IKBD_tailPtr = IKBD_tailPtr+1;
			
            *IKBD_tailPtr = scancode_table[makerepeat];      /*makerepeat is the global make key of the last pressed character*/   
            fillLevel = fillLevel + 1;
			keyrepeatnext = timenow + 4;
		}
	}
	return;
}