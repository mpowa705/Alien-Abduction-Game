#include"psg.h"
#include"music.h"
#include"invasion.h"
#include<stdio.h>
#include<osbind.h>

void start_music(SONG *theSong, unsigned int* *current_note, unsigned int* *note_duration, unsigned int MUSIC[]){

    int i = 0;
    int j = 0;
    
    enable_channel(CHANNEL_A, ON, OFF);

    (*current_note) = &(MUSIC[0]);
    (*note_duration) = &(MUSIC[1]);
    theSong->timeElapsed = pollclock();
	/*
	 printf("CURRENT_NOTE %d \n", *(*current_note));
*/
    set_tone(CHANNEL_A, *(*current_note));
    set_volume(CHANNEL_A,9);
}

void update_music(unsigned long time_elapsed, SONG *theSong, unsigned int* *current_note, unsigned int* *note_duration, unsigned int MUSIC[]){
    int i = 0;
 
        while((*current_note) != &(MUSIC[i])){
            i++;
        }
        if(i >= (SONG_LENGTH-2)){
			(*current_note) = &(MUSIC[0]);
			(*note_duration) = &(MUSIC[1]);
			set_tone(CHANNEL_A, *(*current_note));
        }
		else{
			(*current_note) = &(MUSIC[i+2]);
		/*
			printf("CURRENT_NOTE %d \n", *(*current_note));
		*/
			(*note_duration) = &(MUSIC[i+3]);
			set_tone(CHANNEL_A, *(*current_note));
		}
    
}
