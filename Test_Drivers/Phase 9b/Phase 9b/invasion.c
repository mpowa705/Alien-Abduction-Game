#include"model.h"
#include"rast.h"
#include"render.h"
#include"events.h"
#include"input.h"
#include"invasion.h"
#include"music.h"
#include"VBL.h"
#include"psg.h"
#include"effects.h"
#include"kybd.h"
#include"globals.h"
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
unsigned char left_button_clicked = FALSE;
int mouse_delta_x = 0;
int mouse_delta_y = 0;
unsigned char process_mouse_data = FALSE;
unsigned char mouse_button_data = 0;
unsigned char input_state = 0;
unsigned char scancode_table[SCAN_CODE_TABLE_SIZE];
extern unsigned int MUSIC[SONG_LENGTH];
unsigned int fillLevel = 0;
unsigned int buffer_empty = TRUE;
unsigned char IKBD_buffer[CHAR_BUFFER_SIZE];
unsigned char *IKBD_headPtr = &(IKBD_buffer[0]);
unsigned char *IKBD_tailPtr = &(IKBD_buffer[CHAR_BUFFER_SIZE-1]);
int xCoord = 319;
int yCoord = 199;
char altbuffer[32256];
int renderrequest;
int playmusic = FALSE;
unsigned long timenow = 0; /*This will hold the current
							content of the clock*/
int currentnote = -1;
unsigned long nextnote = 0;

int main()
{  
    char action = 0;
    unsigned long spawnplane = 0,			   /*This times the spawning of the next plane*/
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
	Vector orig_vbl_vector;
    Vector orig_ikbd_vector;
    
    orig_vbl_vector = install_vector(VBL, vblisr);
    orig_ikbd_vector = install_vector(IKBD_VECTORNUM, ikbd_isr);
    
    addressofalt = (unsigned long)&(altbuffer[0]);
    addressofalt = (addressofalt | 0xff ) + 1; /*Finds first address that is multiple of 256 in array*/
    addressoforig = (unsigned long)get_videobase();

    initializegame(&(game));
	renderrequest = FALSE;
	printf("\033f \n");
    stop_sound();
    
    action = rendersplash(game);
    if(action == 49){
        
        spawnplane = timenow + PLANESPAWNTICKS;
        spawnsoldier = timenow + SOLDIERSPAWNTICKS;

        renderfirst(game);
		playmusic = TRUE;
    
        swapbuffers(addressoforig,addressofalt,&(game));	

        orig = TRUE;
        renderfirst(game);
        
        while((game.player.lives != 0) && (inreq != 27))
        {
            nextframe(&(game));			/*This takes a "snapshot" of the moving parts of the model*/
            if((game.beamfiring == TRUE) && (timenow >= unfire))
            {
                stopfiring(&(game));
            }
            if((game.player.BeamReady == FALSE) && (timenow >= beamready))
            {
                game.player.BeamReady = TRUE;
            }
            
            if(fillLevel > 0) /* input pending in the buffer */
            {
                inreq = userinput();
                /*
                if(process_mouse_data == TRUE){
                    update_mouse();
                }
                */
                if(inreq == ASCII_space)
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
        
            /*render(game); */
            printf("x = %d y = %d \n", xCoord, yCoord);
            
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
    }
    
	install_vector(VBL, orig_vbl_vector);
    install_vector(IKBD_VECTORNUM, orig_ikbd_vector);
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
	while(renderrequest == FALSE)
		;
	set_video_base((unsigned int*) oldbuffer);
	game->world.base08 = (unsigned char*)nextbuffer;
	game->world.base16 = (unsigned int*)nextbuffer;
	game->world.base32 = (unsigned long*)nextbuffer;
	renderrequest = FALSE;
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