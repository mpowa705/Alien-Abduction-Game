#include"psg.h"
#include<osbind.h>
#include<stdio.h>

void write_psg(UINT8 reg, unsigned int val){
	volatile char *PSG_reg_select = 0xFF8800;
	volatile char *PSG_reg_write  = 0xFF8802;
	long old_ssp = Super(0);
    
	if(reg >= 0 && reg <=15 && val >= 0 && val <= 255){
		*PSG_reg_select = reg;
		*PSG_reg_write = val;
	}
	Super(old_ssp);
}

void write_psg_select(UINT8 reg, UINT8 val, int andOr){
	volatile char *PSG_reg_select = 0xFF8800;
	volatile char *PSG_reg_write  = 0xFF8802;
	long old_ssp = Super(0);
    UINT8 registerValue = 0;
    
    if(andOr == AND){
        if(reg >= 0 && reg <=15 && val >= 0 && val <= 255){
        *PSG_reg_select = reg;
        registerValue = *PSG_reg_select;
        registerValue &= val;
        *PSG_reg_select = reg;
		*PSG_reg_write = registerValue;
        }
    }
    else if(andOr == OR){
        if(reg >= 0 && reg <=15 && val >= 0 && val <= 255){
        *PSG_reg_select = reg;
        registerValue = *PSG_reg_select;
        registerValue |= val;
        *PSG_reg_select = reg;
		*PSG_reg_write = registerValue;
        }
    }
    Super(old_ssp);
}

void set_tone(UINT8 channel, unsigned int tuning){
	UINT8 fine;
	UINT8 rough;
	
	if(channel >= 0 && channel <= 5 && tuning >=0 && tuning <= 4095){   /* 4096 = 2^12 */
		fine = tuning;
		rough = (tuning >> 8);
		
		write_psg((channel*2), fine);
		write_psg(((channel*2)+1), rough);
	}
}

void set_volume(UINT8 channel, UINT8 volume){
	if(channel >= 0 && channel <= 2 && volume >= 0 && volume <= 15)
        write_psg(channel+CHANNEL_OFFSET, volume);
}

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

void stop_sound(){
	write_psg(REG_0, 0);
	write_psg(REG_1, 0);
	write_psg(REG_2, 0);
	write_psg(REG_3, 0);
	write_psg(REG_4, 0);
	write_psg(REG_5, 0);	
	write_psg(REG_6, 0);
	
	enable_channel(CHANNEL_A, 0, AND);
	enable_channel(CHANNEL_B, 0, AND);
	enable_channel(CHANNEL_C, 0, AND);
}

void set_noise(UINT8 tuning){
	if(tuning >= 0 && tuning <= 31)
		write_psg(REG_6, tuning);
}

void set_envelope(UINT8 shape, unsigned int sustain){
	UINT8 fine;
	UINT8 rough;
	
	if(sustain >= 0 && sustain <= 65535){
		if(shape >= 0 && shape <= 15){
			fine = sustain;
			rough = (sustain >> 8);
			write_psg(REG_11, fine);
			write_psg(REG_12, rough);
			write_psg(REG_13, shape);
		}
	}
}