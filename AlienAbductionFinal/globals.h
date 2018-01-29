/*Team: Matthew Powaschuk, Brady Kirchner
COMP 2659 - Winter 2016
Prof: Paul Pospisil
Project: "Alien Abduction"
*/

#ifndef GLOB_H
#define GLOB_H
#define CHAR_BUFFER_SIZE 256
#define SCAN_CODE_TABLE_SIZE 128
#define SONG_LENGTH 120
#define SM_UFO_HEIGHT 16
#define SM_UFO_WIDTH 16
#define SCREEN_WIDTH 639
#define SCREEN_HEIGHT 399

extern int xCoord;
extern int yCoord;
extern int mousemoved;
extern unsigned int fillLevel;
extern unsigned char IKBD_buffer[CHAR_BUFFER_SIZE];
extern unsigned char *IKBD_tailPtr;
extern unsigned char *IKBD_headPtr;
extern char altbuffer[32256];
extern int renderrequest;
extern int playmusic;
extern int currentnote;
extern int holdingdown;
extern unsigned char makerepeat;
extern unsigned long keyrepeatdelay;
extern unsigned long keyrepeatnext;
extern unsigned long timenow;
extern unsigned long nextnote;
extern unsigned char input_state;
extern unsigned char mouse_button_data;
extern int mouse_delta_x;
extern int mouse_delta_y;
extern unsigned char scancode_table[SCAN_CODE_TABLE_SIZE] = {
    0x00,0x1B,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x30,0x2D,0x3D,
    0x08,0x09,0x71,0x77,0x65,0x72,0x74,0x79,0x75,0x69,0x6F,0x70,0x5B,0x5D,
    0x0D,0x00,0x61,0x73,0x64,0x66,0x67,0x68,0x6A,0x6B,0x6C,0x3B,0x27,0x60,
    0x00,0x5C,0x7A,0x78,0x63,0x76,0x62,0x6E,0x6D,0x2C,0x2E,0x2F,0x00,0x00,
    0x00,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x2D,0x00,0x00,0x00,0x2B,0x00,0x00,0x00,0x00,0x7F,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x28,0x29,0x2F,0x2A,0x37,0x38,0x39,0x34,0x35,0x36,0x31,
    0x32,0x33,0x30,0x2E,0x0D,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00
    };

 extern unsigned int UFO_162[SM_UFO_HEIGHT] = 
	{
		0xFFFF,
		0x881F,
		0x924F,
		0xA187,
		0xC7E3,
		0x8811,
		0x8811,
		0x8421,
		0xFFFF,
                0X8001,
		0xCC33,
		0xCDB3,
		0xE183,
		0xFC3F,
		0xEFF7,
		0xFFFF
	};

extern unsigned int MUSIC[SONG_LENGTH]  = {
    (284*2),10,
    478,10,
    379,10,
    358,10,        
    478,10,
    379,10,
    358,10,
    478,10,
    379,10,   
    (284*2),30,
    379,30,
    426,30,
    379,30,
    319,30,
    379,119, 
    (284*2),10,
    478,10,
    379,10,
    358,10,        
    478,10,
    379,10,
    358,10,
    478,10,
    379,10,
    (284*2),30,
    379,30,
    426,30,
    379,30,
    284,30,
    379,119,
    (284*2),10,
    478,10,
    379,10,
    358,10,        
    478,10,
    379,10,
    358,10,
    478,10,
    379,10,
    (478/2),30,
    253,30,
    284,30,
    319,30,
    284,30,
    379,119,
    (284*2),10,
    478,10,
    379,10,
    358,10,        
    478,10,
    379,10,
    358,10,
    478,10,
    379,10,
    (478/2),30,
    253,30,
    284,30,
    319,30,
    253,30,
    379,119 
    };
	
	

#endif