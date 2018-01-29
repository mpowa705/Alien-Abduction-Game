/*Team: Matthew Powaschuk, Brady Kirchner
COMP 2659 - Winter 2016
Prof: Paul Pospisil
Project: "Alien Abduction"
Details: Mostly written by Brady

*/

#include"psg.h"
#include<osbind.h>
#include<stdio.h>

/*
Function Name: write_psg

Purpose: Writes a given value to a given register on the YM2149 sound chip

Input Parameters:
UINT reg: YM2149 register being selected
unsigned int val: value to write to the selected register

*/
void write_psg(UINT8 reg, UINT8 val){
	volatile char *PSG_reg_select = 0xFF8800; /* Register select for YM2149 */
	volatile char *PSG_reg_write  = 0xFF8802; /* Write register for YM2149 */
    
	if(reg >= 0 && reg <= MAX_REGISTERS && val >= 0 && val <= MAX_WRITE_VALUE){
		*PSG_reg_select = reg;
		*PSG_reg_write = val;
	}
}

/*
Function Name: write_psg_select

Purpose: Writes to a given register using bitwise AND/bitwise OR operations depending on the
desired outcome.

Details: Either a value of AND or OR is passed in, as well as the register and the value
to write to it. The associated bitwise operation dictates what kind of assignment will be
performed during the write operation.

Input Parameters: 
UINT8 reg: YM2149 register being selected
UINT8 val: value to write to the selected register
int andOr: bitwise operation to be performed during write operation
*/
void write_psg_select(UINT8 reg, UINT8 val, int andOr){
	volatile char *PSG_reg_select = 0xFF8800; /* Register select for YM2149 */
	volatile char *PSG_reg_write  = 0xFF8802; /* Write register for YM2149 */
    UINT8 registerValue = 0; /* Since read and write are seperate operations, this is a temporary storage value for reg data */
    
    if(andOr == AND){       /* Read the register, perform a bitwise AND assignment and write back to the YM2149 */
        if(reg >= 0 && reg <= MAX_REGISTERS && val >= 0 && val <= MAX_WRITE_VALUE){
        *PSG_reg_select = reg;
        registerValue = *PSG_reg_select;
        registerValue &= val;
        *PSG_reg_select = reg;
		*PSG_reg_write = registerValue;
        }
    }
    else if(andOr == OR){   /* Read the register, perform a bitwise OR assignment and write back to the YM2149 */
        if(reg >= 0 && reg <= MAX_REGISTERS && val >= 0 && val <= MAX_WRITE_VALUE){
        *PSG_reg_select = reg;
        registerValue = *PSG_reg_select;
        registerValue |= val;
        *PSG_reg_select = reg;
		*PSG_reg_write = registerValue;
        }
    }
}

/*
Function Name: set_tone

Purpose: Sets the tone to be played on a specified channel on the YM2149 sound chip.

Details: The value of the tone to write is a 12 bit value divided accross 2 bytes: fine and rough.
The highest possible value to write is (2^12)-1 or 4095. The low byte is cast to an unsigned char
and assignment of the high byte is achieved using a shift to the right, of 8 bits.

Input:
UINT8 channel: channel to write to (A, B, or C)
unsigned int tuning: tone to write to the given channel from 0-4095

*/
void set_tone(UINT8 channel, unsigned int tuning){
	UINT8 fine;
	UINT8 rough;
	
	if(channel >= 0 && channel <= 5 && tuning >=0 && tuning <= MAX_TONE){   /* MAX_TONE = 4095 = (2^12)-1 */
		fine = tuning;
		rough = (tuning >> 8);
		
		write_psg((channel*2), fine);
		write_psg(((channel*2)+1), rough);
	}
}

/*
Function Name: set_volume

Purpose: Sets the given volume level of a given channel.

Input:
UINT8 channel: channel of which to set the volume (A, B or C)
UINT9 volume: volume level to be assigned (from 0 to a max of 15)

*/
void set_volume(UINT8 channel, UINT8 volume){
	if(channel >= 0 && channel <= 2 && volume >= 0 && volume <= MAX_VOLUME)
        write_psg(channel+CHANNEL_OFFSET, volume);
}

/*
Function Name: enable_channnel

Purpose: Each channel (A, B or C) has enable/disable modes for both their tone (tuning) output
as well as their noise output. This function enables or disables the given channel's tone and noise
outputs as specified by the input parameters.

Details: In order to turn of an output that is already on, the OR value is passed in to the
write_psg_select function in order to specify that a bitwise OR is desired. Otherwise, the AND
value will be passed to turn on an output that is currently off.

Input:
int channel: selected channel to modify (A, B or C)
UINT8 tone_on: indicates whether the tone output is to be enabled or disabled
UINT8 noise_on: indicates whether the noise output is to be enabled or disabled

*/
void enable_channel(int channel, UINT8 tone_on, UINT8 noise_on){
	
	if(channel == CHANNEL_A){			/* If channel = A */
		if(tone_on == TRUE){
			if(noise_on == TRUE)				
				write_psg_select(REG_7, 0xF6, AND);
			
			else if(noise_on == FALSE){
				write_psg_select(REG_7, 0xFE, AND);
                
                write_psg_select(REG_7, 0x02, OR);
                
            }
		}
		else if(tone_on == FALSE){
			if(noise_on == TRUE){				
				write_psg_select(REG_7, 0xF7, AND);
                
                write_psg_select(REG_7, 0x01, OR);
                
			}
			else if(noise_on == FALSE)
				write_psg_select(REG_7, 0x09, OR);
		}
	}
	if(channel == CHANNEL_B){			/* If channel = B */
		if(tone_on == TRUE){
			if(noise_on == TRUE)				
				write_psg_select(REG_7, 0xED, AND);
			
			else if(noise_on == FALSE){
				write_psg_select(REG_7, 0xFD, AND);
                write_psg_select(REG_7, 0x10, OR);
                
			}
		}
		else if(tone_on == FALSE){
			if(noise_on == TRUE){				
				write_psg_select(REG_7, 0xEF, AND);
                write_psg_select(REG_7, 0x02, OR);
            }
			else if(noise_on == FALSE)
				write_psg_select(REG_7, 0x12, OR);
		}
	}
	if(channel == CHANNEL_C){			/* If channel = C */
		if(tone_on == TRUE){
			if(noise_on == TRUE)				
				write_psg_select(REG_7, 0xDB, AND);
			
			else if(noise_on == FALSE){
				write_psg_select(REG_7, 0xFB, AND);
                write_psg_select(REG_7, 0x20, OR);
            }
		}
		else if(tone_on == FALSE){
			if(noise_on == TRUE){				
				write_psg_select(REG_7, 0xDF, AND);
                write_psg_select(REG_7, 0x04, OR);
            }
			else if(noise_on == FALSE)
				write_psg_select(REG_7, 0x24, OR);
		}
	}
}

/*
Function Name: stop_sound

Purpose: Stops all sound generation by the YM2149
*/
void stop_sound(){
	long old_ssp = Super(0);
	write_psg(REG_0, 0);
	write_psg(REG_1, 0);
	write_psg(REG_2, 0);
	write_psg(REG_3, 0);
	write_psg(REG_4, 0);
	write_psg(REG_5, 0);	
	write_psg(REG_6, 0);
	
	enable_channel(CHANNEL_A, OFF, OFF);
	enable_channel(CHANNEL_B, OFF, OFF);
	enable_channel(CHANNEL_C, OFF, OFF);
	Super(old_ssp);
}

/*
Function Name: set_noise

Purpose: Specifies the tone of the noise output (reg 6) given a value between 0 and 31.

Input:
UINT8 tuning: specified frequency (low to high) of which the noise channel is to output

*/
void set_noise(UINT8 tuning){
	if(tuning >= 0 && tuning <= MAX_NOISE_VALUE)
		write_psg(REG_6, tuning);
}

/*
Function Name: set_envelope

Purpose: Sets the YM2149's envelope to be of a given shape over a given period duration.

Input:
UINT8 shape: shape of the envelope to be generated
unsigned int sustain: duration of a period over which a given shape is to be generated (from 0 to 65535)

*/
void set_envelope(UINT8 shape, unsigned int sustain){
	UINT8 fine;
	UINT8 rough;
	if(sustain >= 0 && sustain <= MAX_SUSTAIN){
		if(shape >= 0 && shape <= 15){
			fine = sustain;
			rough = (sustain >> 8);
			write_psg(REG_11, fine);
			write_psg(REG_12, rough);
			write_psg(REG_13, shape);
		}
	}
}