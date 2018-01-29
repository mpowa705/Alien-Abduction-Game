#include<stdio.h>
#include<osbind.h>
#include"input.h"
#include"globals.h"
#include"model.h"

/*Team: Matthew Powaschuk, Brady Kirchner
COMP 2659 - Winter 2016
Prof: Paul Pospisil
Project: "Alien Abduction"
*/

char userinput()
{
	char answer;
    unsigned int i = 0;
    long old_ssp;
    int orig_ipl;
    
    old_ssp = Super(0);
    orig_ipl = set_ipl(7);
    Super(old_ssp);
    
    if(fillLevel != 0)
        fillLevel = fillLevel-1;
	answer = *IKBD_headPtr;
    
    if(IKBD_headPtr == &(IKBD_buffer[CHAR_BUFFER_SIZE-1]))
        IKBD_headPtr = &(IKBD_buffer[0]);
    else
        IKBD_headPtr = IKBD_headPtr + 1;
    
    old_ssp = Super(0);
    set_ipl(orig_ipl);
    Super(old_ssp);
    
	return answer;
}
/*
void update_mouse(){
    if(input_state == 1){ /* Processing new mouse button data 
    
        if(mouse_button_data ==  0xFA)
            left_button_clicked = TRUE;
        
        printf("1 \n");
    }
    else if(input_state == 2){ /* Processing new mouse xCoord data 
    
        xCoord = xCoord + mouse_delta_x;
        
        printf("2 \n");
    }
    else if(input_state == 0){ /* Processing new mouse yCoord data 
    
        yCoord = yCoord + mouse_delta_y;
        process_mouse_data == FALSE;
        printf("3 \n");
    }
}
*/

void update_mouse(){
    
        if(mouse_button_data ==  0xFA)
            left_button_clicked = TRUE;
        
        printf("1 \n");
        xCoord = xCoord + mouse_delta_x;
        printf("2 \n");
        yCoord = yCoord + mouse_delta_y;
        printf("3 \n \n");
        process_mouse_data == FALSE;
}/*
    else if(input_state == 2){ /* Processing new mouse xCoord data 
    
        xCoord = xCoord + mouse_delta_x;
        
        printf("2 \n");
    }
    else if(input_state == 0){ /* Processing new mouse yCoord data 
    
        yCoord = yCoord + mouse_delta_y;
        process_mouse_data == FALSE;
        printf("3 \n");
    }
}
*/