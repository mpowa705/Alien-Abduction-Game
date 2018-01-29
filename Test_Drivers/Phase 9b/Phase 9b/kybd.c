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

void do_IKBD_ISR()
{
    unsigned int i = 0;
    unsigned char data;
    volatile char *in_service_B = 0xFFFA11;
    volatile char *IKBD_status = 0xFFFC00;
    volatile char *IKBD_data = 0xFFFC02;
    
    if(*IKBD_status & 1){
        data = *IKBD_data;
        if(input_state == 0){
            if(!(data >= 0xF8)){
                if(fillLevel <= (CHAR_BUFFER_SIZE-1)){
                    if(data < 10000000){
                        if(IKBD_tailPtr == &(IKBD_buffer[CHAR_BUFFER_SIZE-1]))
                            IKBD_tailPtr = &(IKBD_buffer[0]);
                        else
                            IKBD_tailPtr = IKBD_tailPtr+1;
                        *IKBD_tailPtr = scancode_table[data];        
                        fillLevel = fillLevel + 1;
                    }
                }
            }
            else{
                /*process_mouse_data = TRUE;*/
                mouse_button_data = data;
                if(mouse_button_data == 0xFA)
                    left_button_clicked = TRUE;
                input_state = 1;
            }
        }
        else if(input_state == 1){
            xCoord = xCoord + (char)data;
            if(xCoord > 639)
                xCoord = 639;
            if(xCoord < 0)
                xCoord = 0;
            input_state = 2;
        }
        else if(input_state == 2){
            yCoord = yCoord + (char)data;
            if(yCoord > 399)
                yCoord = 399;
            if(yCoord < 0)
                yCoord = 0;
            input_state = 0;
        }
    }
    else{
        /* Spurious interrupt? Throw exception? */
    }
    
    *in_service_B = 0xBF;
}
