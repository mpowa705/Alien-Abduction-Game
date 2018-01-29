#include"model.h"
#include"rast.h"
#include"render.h"
#include"events.h"
#include"input.h"
#include"invasion.h"
#include"music.h"
#include"psg.h"
#include"effects.h"
#include<stdio.h>
#include<osbind.h>

/*Team: Matthew Powaschuk, Brady Kirchner
COMP 2659 - Winter 2016
Prof: Paul Pospisil
Project: "Invader's revenge"

Purpose: Plays the "Invader's revenge" game, on the Atari ST.

Details: This game will start a loop
to process the synchronous and asynchronous events that make the game,
which can be ended by pressing the p key.

Known bugs: There is a known bug where if a plane or soldier despawns on the
same frame that one spawns, there is a problem clearing those entities, there
is a temporary workaround where the soldierde and planede variables
prevent that condition from happening.
*/

char altbuffer[32256];

int main()
{
    unsigned int MUSIC[SONG_LENGTH] = {
        (284*2),13,
        478,13,
        379,13,
        358,13,        
        478,13,
        379,13,
        358,13,
        478,13,
        379,13,
        
        (284*2),45,
        379,45,
        426,45,
        379,45,
        319,45,
        379,180,
        
        (284*2),13,
        478,13,
        379,13,
        358,13,        
        478,13,
        379,13,
        358,13,
        478,13,
        379,13,
        
        (284*2),45,
        379,45,
        426,45,
        379,45,
        284,45,
        379,180,
        
        (284*2),13,
        478,13,
        379,13,
        358,13,        
        478,13,
        379,13,
        358,13,
        478,13,
        379,13,
        
        (478/2),45,
        253,45,
        284,45,
        319,45,
        284,45,
        379,180,
        
        (284*2),13,
        478,13,
        379,13,
        358,13,        
        478,13,
        379,13,
        358,13,
        478,13,
        379,13,
        
        (478/2),45,
        253,45,
        284,45,
        319,45,
        253,45,
        379,180 
    };
    SONG theSong;
    
    unsigned long timenow = 0, /*This will hold the current
							content of the clock*/
    spawnplane = 0,			   /*This times the spawning of the next plane*/
    spawnsoldier = 0,		   /*This times the spawning of the next soldier*/
    unfire = 0, 			   /*This times when to stop firing the beam*/
    beamready = 0; 			   /*This times when the beam is ready to fire again*/
    unsigned long addressofalt,/*Holds the location of the alternate buffer*/ 
    addressoforig;    		   /*Holds the location of the alternate buffer*/          
    MainGame game;
    int nextplane = R,nextsoldier = L, fired = FALSE,
    beamcleared = FALSE,
    orig = FALSE,
    soldierde = FALSE,  		/*These 2 variables are for the workaround*/
    planede = FALSE,			/*of the bug, they will be removed once fixed*/
		
    clearedbeamfirst = FALSE;
    char inreq;

    addressofalt = (unsigned long)&(altbuffer[0]);
    addressofalt = (addressofalt | 0xff ) + 1; /*Finds first address that is multiple of 256 in array*/
    addressoforig = (unsigned long)Physbase();

    initializegame(&(game));

    timenow = pollclock();
    spawnplane = timenow + PLANESPAWNTICKS;
    spawnsoldier = timenow + SOLDIERSPAWNTICKS;

    renderfirst(game);
    start_music(&(theSong), &(theSong.notePtr), &(theSong.durationPtr), MUSIC);
    
    swapbuffers(addressoforig,addressofalt,&(game));	
    orig = TRUE;

    renderfirst(game);

    while((game.player.lives != 0) && (inreq != 'p'))
    {
        nextframe(&(game));			/*This takes a "snapshot" of the moving parts of the model*/
        timenow = pollclock();
        if(timenow-(theSong.timeElapsed) >= *(theSong.durationPtr))
        {
            update_music((timenow-(theSong.timeElapsed)), &(theSong), &(theSong.notePtr), &(theSong.durationPtr), MUSIC);
            theSong.timeElapsed = timenow;      
        }
        if((game.beamfiring == TRUE) && (timenow >= unfire))
        {
            stopfiring(&(game));
        }
        if((game.player.BeamReady == FALSE) && (timenow >= beamready))
        {
            game.player.BeamReady = TRUE;
        }
        
        if(Cconis())
        {
            inreq = userinput();
            if(inreq == ' ')
            {   
                fired = requestfirebeam(&(game),&(game.player),game.soldiersleft,game.soldiersright, &(game.tailSleft),&(game.tailSright));
                fseek(stdin,0,SEEK_END);
                if(fired == TRUE)
                {
                    unfire = timenow + BEAMTICKS;
                    beamready = timenow + BEAMREADYTICKS;
                }
            }
            else
            {
                requestmoveufo((int)inreq, &(game), &(game.player),&(game.world));
                fseek(stdin,0,SEEK_END);
            }
        }
	
        if(timenow >= spawnplane && planede == FALSE)
        {
            spawnplane = spawnplane + PLANESPAWNTICKS;
            if(nextplane == R)
            {
                spawn_plane(R,game.planesright,&(game.tailPright),&(game.world),
                game.planespeed);
                nextplane = L;
            }
            else
            {
                spawn_plane(L,game.planesleft,&(game.tailPleft),&(game.world),
                game.planespeed);
                nextplane = R;
            }
        }
	
        if(timenow >= spawnsoldier && soldierde == FALSE)
        {
            spawnsoldier = spawnsoldier + SOLDIERSPAWNTICKS;
            if(nextsoldier == R)
            {
                spawn_soldier(R, game.soldiersright,&(game.tailSright));
                nextsoldier = L;
            }
            else
            {
                spawn_soldier(L, game.soldiersleft,&(game.tailSleft));
                nextsoldier = R;
            }
		
        }
	
        soldierde = FALSE;
        planede = FALSE;
	
        soldierde = requestmovesoldiers(&(game),&(game.player));
        planede = requestmoveplanes(&(game), &(game.player), &(game.world));	
	
        render(game);
	
        if(orig == FALSE)
        {
            swapbuffers(addressoforig,addressofalt,&(game));
            orig = TRUE;
        }
        else
        {
            swapbuffers(addressofalt,addressoforig,&(game));	
            orig = FALSE;
        }
	
        if(game.clearbeam == TRUE && clearedbeamfirst == TRUE)
        {
            game.clearbeam = FALSE;
            clearedbeamfirst = FALSE;
        }
        else if(game.clearbeam == TRUE && clearedbeamfirst == FALSE)
        {
            clearedbeamfirst = TRUE;
        }
	
    }
	if(orig == FALSE)	/*This makes sure that when exiting, the bufferer is set back to the original*/
		swapbuffers(addressoforig,addressofalt,&(game));
        
    stop_sound();
	return 0;
}

/*
Purpose: This function swaps the active buffer with the back buffer to be rendered.
Details: This function sets the oldbuffer as the active buffer
and sets the buffers in the model structure to the buffer that was 
previously active.
Input: takes in the oldbuffer that was being rendered, and the nextbuffer to
render to, as well as the model by reference.
*/
void swapbuffers(unsigned long oldbuffer, unsigned long nextbuffer, MainGame *game)
{
	Vsync();
	Setscreen(oldbuffer,oldbuffer,-1);
	game->world.base08 = (unsigned char*)nextbuffer;
	game->world.base16 = (unsigned int*)nextbuffer;
	game->world.base32 = (unsigned long*)nextbuffer;
}

/*
Purpose: This function renders all soldiers to the buffer.
Details: This function temporarily goes into
supervisor mode to retrieve the value at address
0x462
Output: Returns the value of the clock.
*/
unsigned long pollclock()
{
	unsigned long *timer = (unsigned long *)0x462;
	unsigned long timeNow;
	unsigned long old_ssp;
	old_ssp = Super(0); /* enter privileged mode */
	timeNow = *timer;
	Super(old_ssp); 
	return timeNow;
}