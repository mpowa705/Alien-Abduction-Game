/*Team: Matthew Powaschuk, Brady Kirchner
COMP 2659 - Winter 2016
Prof: Paul Pospisil
Project: "Alien Abduction"
*/

#include"kybd.h"
#include"vbl.h"
#include"globals.h"
#include"model.h"
#include<osbind.h>
#include<stdio.h>

/*
Function Name: do_IKBD_ISR

Purpose: The intelligent keyboard service routine function below is called by its vector # when a keyboard interrupt
is detected.

Details: A finite state machine is implemented by using the global varaiable "input_state" which has the following
possible values: 
0 - waiting for possible keyboard make code, or initial byte of mouse data
1 - initial byte of mouse data has been processed, x-coordinate of the mouse to be processed
2 - x-coordinate of the mouse has been processed, y-coordinate to be processed (sets the input_state back to 0 when finished)
The data from the IKBD data register is saved in a global variable (or global array in the case of the keyboard buffer
based on the type of data being received.

Written mostly by Brady

*/

void do_IKBD_ISR()
{   
    const int MAX_HORIZ = SCREEN_WIDTH-SM_UFO_WIDTH;
    const int MAX_VERT = SCREEN_HEIGHT-SM_UFO_HEIGHT;
    int spurious_interrupt = TRUE;
    unsigned int i = 0;
    unsigned char data; /* Incoming IKBD_data from the MC6850 */
    char mouse_data;    /* Incoming mouse data. Input as packets (groups of 3) */
    volatile char *in_service_B = 0xFFFA11; /* MFP68901 status register */
    volatile char *IKBD_status = 0xFFFC00;  /* MC6850 status register */
    volatile char *IKBD_data = 0xFFFC02;    /* MC6850 data register */
    
    if(*IKBD_status & 1){ /* Indicates whether the 6850 has pending input to be processed */
        data = *IKBD_data;
        if(input_state == 0){ /* State 0 of the FSM described above */
            if(!(data >= 0xF8)){    /* Indicates this data is not from a mouse packet */
                if(fillLevel <= (CHAR_BUFFER_SIZE-1)){ /* Checks if current keyboard buffer full */
                    if(data <= 128){                   /* If data > 128 then it's a break code, ignore */
                        if(IKBD_tailPtr == &(IKBD_buffer[CHAR_BUFFER_SIZE-1]))
                            IKBD_tailPtr = &(IKBD_buffer[0]);
                        else
                            IKBD_tailPtr = IKBD_tailPtr+1; /* Increment the tail pointer to the buffer queue */
                        *IKBD_tailPtr = scancode_table[data]; /* Get associated ASCII value from the scancode */    
                        fillLevel = fillLevel + 1;
						holdingdown = TRUE;
						keyrepeatdelay = timenow + 18;
						keyrepeatnext = timenow + 22;
						makerepeat = data;
                    }
                }
				if(data == (makerepeat + 0x80))
						holdingdown = FALSE; 
            }
            else{
                mouse_button_data = data;

                input_state = 1;
            }
        }
        else if(input_state == 1){
            mouse_data = (char)data;
			mousemoved = TRUE;
            xCoord = xCoord + mouse_data;
            if(xCoord > MAX_HORIZ)          
                xCoord = MAX_HORIZ;
            if(xCoord < 0)
                xCoord = 0;         
            input_state = 2;        
        }
        else if(input_state == 2){
            mouse_data = (char)data;
			mousemoved = TRUE;
            yCoord = yCoord + mouse_data;
            if(yCoord > MAX_VERT)
                yCoord = MAX_VERT;
            if(yCoord < 0)
                yCoord = 0;
            input_state = 0;
        }
    }
    else{
        while(spurious_interrupt == TRUE)
            ;
        /* Spurious interrupt, hang system */
    }
    
    *in_service_B = 0xBF; /* Resets bit 6 indicating the MFP68901 chip is done being serviced */
}
