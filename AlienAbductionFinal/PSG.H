/*Team: Matthew Powaschuk, Brady Kirchner
COMP 2659 - Winter 2016
Prof: Paul Pospisil
Project: "Alien Abduction"
*/

#ifndef PSG_H
#define PSG_H
#include"model.h"

#define CHANNEL_OFFSET 8
#define CHANNEL_A 0
#define CHANNEL_B 1
#define CHANNEL_C 2
#define TRUE 1
#define FALSE 0
#define ON 1
#define OFF 0
#define AND 0
#define OR 1
#define REG_0 0
#define REG_1 1
#define REG_2 2
#define REG_3 3
#define REG_4 4
#define REG_5 5
#define REG_6 6
#define REG_7 7
#define REG_11 11
#define REG_12 12
#define REG_13 13
#define MAX_TONE 4095
#define MAX_SUSTAIN 65535
#define MAX_WRITE_VALUE 255
#define MAX_REGISTERS 15
#define MAX_VOLUME 15
#define MAX_NOISE_VALUE 15

void write_psg_select(UINT8 reg, UINT8 val, int andOr);
void write_psg(UINT8 reg, UINT8 val);
void set_tone(UINT8 channel, unsigned int tuning);
void set_volume(UINT8 channel, UINT8 volume);
void enable_channel(int channel, UINT8 tone_on, UINT8 noise_on);
void stop_sound();

void set_noise(UINT8 tuning);
void set_envelope(UINT8 shape, unsigned int sustain);

#endif