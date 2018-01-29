/*Team: Matthew Powaschuk, Brady Kirchner
COMP 2659 - Winter 2016
Prof: Paul Pospisil
Project: "Alien Abduction"
Details: Written by Brady

*/

#include<stdio.h>
#include<osbind.h>
#include"input.h"
#include"globals.h"
#include"model.h"

/*
Function: userinput

Purpose: Removes a value from the global IKBD_buffer keyboard input buffer and returns it to
the caller, updating the buffer's fill level and head pointer.

Returns: The value contained in the keyboard input buffer queue at the head, at the time of the
call.

*/
char userinput(){
    
	char value;            /* Value to be returned/removed from the buffer */
    unsigned int i = 0;
    long old_ssp;          
    int orig_ipl;
    
    old_ssp = Super(0);
    orig_ipl = set_ipl(7); /* Increase to maximum IPL for this critical section of code */
    Super(old_ssp);
    
    if(fillLevel != 0){
        fillLevel = fillLevel-1;
        value = *IKBD_headPtr; /* Get the value from the head of the buffer queue */
    
        if(IKBD_headPtr == &(IKBD_buffer[CHAR_BUFFER_SIZE-1]))
            IKBD_headPtr = &(IKBD_buffer[0]);
        else
            IKBD_headPtr = IKBD_headPtr + 1; /* Increment the head pointer of the character buffer */
    }
    old_ssp = Super(0);
    set_ipl(orig_ipl);    /* Reset IPL level back to original level */
    Super(old_ssp);
    
	return value;
}
