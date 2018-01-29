/*Team: Matthew Powaschuk, Brady Kirchner
COMP 2659 - Winter 2016
Prof: Paul Pospisil
Project: "Alien Abduction"
Details: play_effects by Brady, stop_effect_crash by Matt
*/

#include"psg.h"
#include"globals.h"
#include"effects.h"
#include"invasion.h"
#include<stdio.h>
#include<osbind.h>

/*
Function Name: play_effect_beam

Purpose: Plays the sound effect of the beam being fired from the UFO.

*/
void play_effect_beam(){
	long old_ssp = Super(0);
	set_tone(CHANNEL_B, BEAM_TONE);
	set_volume(CHANNEL_B,11);
	enable_channel(CHANNEL_B, ON, OFF);
	Super(old_ssp);
}

/*
Function Name: play_effect_crash

Purpose: Plays the "crash" sound effect of the UFO crashing into something.

*/
void play_effect_crash(){
	long old_ssp = Super(0);
	set_volume(CHANNEL_B,0);
	set_volume(CHANNEL_C, CRASH_VOLUME);
	set_envelope(ENV_SHAPE, ENV_FREQ);
	enable_channel(CHANNEL_C, OFF, ON);
  
	Super(old_ssp);
}

/*
Function Name: stop_effect_crash

Purpose: Ends the "crash" sound effect by setting the volume of that channel to zero
and disabling the channel's input.

*/
void stop_effect_crash(){
	long old_ssp = Super(0);
	enable_channel(CHANNEL_C, OFF, OFF);
    set_volume(CHANNEL_C, OFF);
	Super(old_ssp);
}