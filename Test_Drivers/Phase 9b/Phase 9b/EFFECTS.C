#include"psg.h"
#include"globals.h"
#include"effects.h"
#include"invasion.h"
#include<stdio.h>
#include<osbind.h>

void play_effect_beam(){
	long old_ssp = Super(0);
	set_tone(CHANNEL_B, 284);
	set_volume(CHANNEL_B,11);
/*set_envelope(8, 50000);*/
	enable_channel(CHANNEL_B, ON, OFF);
	Super(old_ssp);
}
void play_effect_crash(){
	unsigned long initialtime = timenow;
	long old_ssp = Super(0);

	set_volume(CHANNEL_C,12);
	set_envelope(4, 255);
	set_noise(31);
	
	enable_channel(CHANNEL_C, OFF, ON);
  
	Super(old_ssp);
}

void stop_effect_crash(){
	long old_ssp = Super(0);
	enable_channel(CHANNEL_C, OFF, OFF);
    set_volume(CHANNEL_C,0);
	set_noise(0);
	Super(old_ssp);
}