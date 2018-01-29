/*Team: Matthew Powaschuk, Brady Kirchner
COMP 2659 - Winter 2016
Prof: Paul Pospisil
Project: "Alien Abduction"

Purpose: Plays the "Alien Abduction" game, on the Atari ST.

Details: After clicking "1 Player Game", this will start a loop
to process the synchronous and asynchronous events that make the game,
which can be ended by pressing the ESC key.

The vase majority of this source code was written by Matt.

2-player game never implemented.
*/

#include"model.h"
#include"rast.h"
#include"render.h"
#include"events.h"
#include"input.h"
#include"invasion.h"
#include"VBL.h"
#include"psg.h"
#include"effects.h"
#include"kybd.h"
#include"globals.h"
#include<osbind.h>

/*************** GLOBAL VARIABLES ***************/

unsigned int UFO_162[SM_UFO_HEIGHT];
unsigned long nextnote = 0;     /* Next note being pointed to in the MUSIC array */
int mouse_delta_x = 0;          /* Change in mouse's x position after a mouse packet received */
int mouse_delta_y = 0;          /* Change in mouse's y position after a mouse packet received */
unsigned char mouse_button_data = 0;   /* Which buttons pressed after a mouse packet received */
unsigned char input_state = 0;                     /* Current state of the FSM in the IKBD ISR */
unsigned char scancode_table[SCAN_CODE_TABLE_SIZE]; /* List of ascii values indexed by scancode */
extern unsigned int MUSIC[SONG_LENGTH];             /* Array of alternating music notes and note durations respectively */
unsigned int fillLevel = 0;                         /* Current fill-level of the keyboard buffer */
unsigned char IKBD_buffer[CHAR_BUFFER_SIZE];     /* Array of input chars referred to as the keyboard buffer */
unsigned char *IKBD_headPtr = &(IKBD_buffer[0]); /* Pointer to the head item in the IKBD buffer queue structure */
unsigned char *IKBD_tailPtr = &(IKBD_buffer[CHAR_BUFFER_SIZE-1]); /* Pointer to the tail item in the IKBD buffer queue structure */
int xCoord = XCOORD_CENTRE;     /* Current x-coordinate of the mouse cursor */
int yCoord = YCOORD_CENTRE;     /* Current y-coordinate of the mouse cursor */
unsigned long timenow = 0; /*This will hold the current
							content of the clock*/
int currentnote = -1;           /* Current note being played from the MUSIC array */
char altbuffer[32256];
int renderrequest;
int playmusic = FALSE;
unsigned long keyrepeatdelay;
unsigned long keyrepeatnext;
long old_ssp;     
int holdingdown = FALSE;
unsigned char makerepeat;
int mousemoved = FALSE;

int main()
{  
    int old_ssp;               /*Stores original stack pointer*/
    int orig_ipl;               /*Stores original interrupt priority level*/
    unsigned long spawnplane = 0,			   /*This times the spawning of the next plane*/
    spawnsoldier = 0,		   /*This times the spawning of the next soldier*/
    unfire = 0, 			   /*This times when to stop firing the beam*/
    beamready = 0; 			   /*This times when the beam is ready to fire again*/
    unsigned long addressofalt,/*Holds the location of the alternate buffer*/ 
    addressoforig;    		   /*Holds the location of the alternate buffer*/          
    MainGame game;			   /*Main game model*/
    int nextplane = R,nextsoldier = L, /*Decides the side from which to spawn enemies*/
	fired = FALSE, 			    /*Boolean indicating if beam was successfully fired*/
    orig = FALSE,				/*Boolean indicating if it is currently displaying the original video buffer*/
    soldierde = FALSE,  		/*These 2 variables prevent the spawning and despawning of a*/
    planede = FALSE,			/*soldier and plane in the same frame*/
    clearedbeamfirst = FALSE;	/*Clearing the beam takes 2 frames, so this indicates whether it's completed the first*/
    char inreq = 0;				/*User input character*/
	Vector orig_vbl_vector;
    Vector orig_ikbd_vector;
    int xCoordold = xCoord;
	int yCoordold = yCoord;
	int xCoordlocal = 0;
	int yCoordlocal = 0;
	int isdonesplash = FALSE;	/*indicates whether it is time to start the game/ quit from splash screen*/
	unsigned int behind[SM_UFO_HEIGHT];	/*The array that holds all saved bitmaps*/
    
    orig_vbl_vector = install_vector(VBL, vblisr);
    
    old_ssp = Super(0);
    orig_ipl = set_ipl(7); /* Increase to maximum IPL for this critical section of code */
    Super(old_ssp);
    orig_ikbd_vector = install_vector(IKBD_VECTORNUM, ikbd_isr); /* Install new IKBD vector */
    old_ssp = Super(0);
    set_ipl(orig_ipl);    /* Reset IPL level back to original level */
    Super(old_ssp);
    
    addressofalt = (unsigned long)&(altbuffer[0]);
    addressofalt = (addressofalt | 0xff ) + 1; /*Finds first address that is multiple of 256 in array*/
    addressoforig = (unsigned long)get_videobase();

    initializegame(&(game)); /* Initialize the game model */
	renderrequest = FALSE;
    stop_sound();            /* Initialize the YM2149 sound chip */
        
    old_ssp = Super(0);
    enable_channel(CHANNEL_A, ON, OFF); /* Activate YM2149 Channel A (for music) */
	Super(old_ssp);    
       
	/*Here starts the splash screen	*/   
	rendersplash(game);        /* Render the splash screen */
	save_bitmap16safe(game.world.base16,xCoord, yCoord,behind,16);
	plot_bitmap16safecurs(game.world.base16,game.world.base32,xCoord, yCoord,UFO_162,
		16);
	
			
	while(isdonesplash == FALSE){
		if((mouse_button_data == 0xFA) && ((xCoord >= 20) && (yCoord >= 200) && (xCoord < 156) && (yCoord < 215)))
			isdonesplash = TRUE;
		else if((mouse_button_data == 0xFA) && ((xCoord >= 483) && (yCoord >= 200) && (xCoord < 619) && (yCoord < 215)))
		{
			isdonesplash = TRUE;
			inreq = ASCII_esc;	/*signals that the player wants to close the game without playing*/
		}
			
		if(mousemoved == TRUE)
		{
			{
			xCoordlocal = xCoord;	/*This section redraws the mouse if it detected movement.*/
			yCoordlocal = yCoord;
			plot_bitmap16safecurs(game.world.base16,game.world.base32,xCoordold, yCoordold,behind,
			16);
			save_bitmap16safe(game.world.base16,xCoordlocal, yCoordlocal,behind,16);
			plot_bitmap16safecurs(game.world.base16,game.world.base32,xCoordlocal, yCoordlocal,UFO_162,
			16);
			mousemoved = FALSE;
			xCoordold = xCoordlocal;
			yCoordold = yCoordlocal;
			}
	
		}
	}
	/*here ends the splash screen*/
	if(inreq != ASCII_esc)
	{
        spawnplane = timenow + PLANESPAWNTICKS;
        spawnsoldier = timenow + SOLDIERSPAWNTICKS;

        renderfirst(game);
        playmusic = TRUE;
	
    
        swapbuffers(addressoforig,addressofalt,&(game));	

        orig = TRUE;
        renderfirst(game);
	}
     
        
       while((game.player.lives != 0) && (inreq != ASCII_esc))
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
               if(inreq == ASCII_space)
               {   
                   fired = requestfirebeam(&(game),&(game.player),game.soldiersleft,game.soldiersright, &(game.tailSleft),&(game.tailSright));
                   if(fired == TRUE)
                   {
                       unfire = timenow + BEAMTICKS;
                       beamready = timenow + BEAMREADYTICKS;
                   }
               }
               else
               {
                   requestmoveufo((int)inreq, &(game), &(game.player),&(game.world));
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
                clearedbeamfirst = TRUE; /*This tells gives the renderer one more frame to clear the beam*/
            }
        
        }
    if(orig == FALSE)	/*This makes sure that when exiting, the bufferer is set back to the original*/
         swapbuffers(addressoforig,addressofalt,&(game));       
      stop_sound();
    
	install_vector(VBL, orig_vbl_vector);
    install_vector(IKBD_VECTORNUM, orig_ikbd_vector);
	return 0;
}

/*
Function Name: swapbuffers

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
Function Name: pollclock

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