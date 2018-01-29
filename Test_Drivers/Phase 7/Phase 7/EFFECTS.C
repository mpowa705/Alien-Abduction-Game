#include"psg.h"
#include"effects.h"
#include"invasion.h"
#include<stdio.h>
#include<osbind.h>

void play_effect_beam(){
	set_tone(CHANNEL_B, 284);
	set_volume(CHANNEL_B,11);
/*set_envelope(8, 50000);*/
	enable_channel(CHANNEL_B, ON, OFF);
}
void play_effect_crash(){
	
	unsigned long timenow;
	unsigned long initialtime;
	
	timenow = pollclock();
	initialtime = pollclock();

	set_volume(CHANNEL_C,12);
	set_envelope(8, 255);
	set_noise(31);
	
	enable_channel(CHANNEL_C, OFF, ON);
	
	while(timenow != (initialtime + 70))
		timenow = pollclock();
    
    enable_channel(CHANNEL_C, OFF, OFF);
    set_volume(CHANNEL_C,0);
}