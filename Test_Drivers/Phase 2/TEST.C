#include<osbind.h>
#include"rast.h"

#define PLANE_HEIGHT 24
#define SOLDIER_HEIGHT 12
#define INVADER_32_HEIGHT 32
#define INVADER_16_HEIGHT 16
#define INVADER_08_HEIGHT 8
#define UFO_HEIGHT 32


int main()
{
	char* base = Physbase();
	int y = 0;
	int x = 161;

/*	
 unsigned long plane[PLANE_HEIGHT] = {
		0x0007E000,
		0x00060000,
		0x00050000,
		0x00048000,
		0x00047800,
		0x00044000,
		0x20042000,
		0x30041000,
		0x28080FC0,
		0x27F00030,	
		0x4000000C,
		0xC0000003,
		0x6000003C,	
		0x17F007C0,
		0x18080800,
		0x10041000,
		0x00042000,
		0x00044000,
		0x00047800,
		0x00048000,
		0x00048000,
		0x00050000,
		0x00060000,	
		0x0007E000
};
*/
 unsigned long plane_right[PLANE_HEIGHT] = {
		0x0007E000,
		0x00060000,
		0x00050000,
		0x00048000,
		0x00047800,
		0x00044000,
		0x20042000,
		0x30041000,
		0x28080FC0,
		0x27F00830,	
		0x400007EC,
		0xC0000003,
		0x6000001C,	
		0x17F007E0,
		0x18080800,
		0x10041000,
		0x00042000,
		0x00044000,
		0x00047800,
		0x00048000,
		0x00048000,
		0x00050000,
		0x00060000,	
		0x0007E000
};

 unsigned long plane_left[PLANE_HEIGHT] = {
		0x0007E000,
		0x00006000,
		0x0000A000,
		0x00012000,
		0x001E2000,
		0x00022000,
		0x00042004,
		0x0008200C,
		0x03F01014,
		0x0C100FE4,	
		0x37E00002,
		0xC0000003,
		0x38000006,	
		0x07E00FE8,
		0x00101018,
		0x00082008,
		0x00042000,
		0x00022000,
		0x001E2000,
		0x00012000,
		0x00012000,
		0x0000A000,
		0x00006000,	
		0x0007E000
};
	unsigned long theUfo[UFO_HEIGHT] = {
		0x0007E000,
		0x00181800,
		0x00600600,
		0x00881100,
		0x01081080,
		0x02042040,
		0x04024020,
		0x081FF810,
		0x10200408,
		0x20466204,
		0x20400204,
		0x4043C202,
		0x40242402,
		0x80100801,
		0xFFFFFFFF,
		0x80000001,
		0x80000001,
		0x40181802,
		0x7824241E,
		0x26242464,
		0x21181884,
		0x10C18308,
		0x10224408,
		0x08124810,
		0x08099010,
		0x0F0420F0,
		0x08842110,
		0x0FE427F0,
		0x04581A20,
		0x02866140,
		0x02818140,
		0x01000080
	};
	unsigned long invader_32_bitmap[INVADER_32_HEIGHT] =
	{
		0x00000000,
		0x08100810,
		0x08100810,
		0x04200420,
		0x02400240,
		0x1FF81FF8,
		0x20042004,
		0x46624662,
		0x40024002,
		0x43C243C2,
		0x24242424,
		0x10081008,
		0x0FF00FF0,
		0x02400240,
		0x0E700E70,
		0x00000000,
		0x00000000,
		0x08100810,
		0x08100810,
		0x04200420,
		0x02400240,
		0x1FF81FF8,
		0x20042004,
		0x46624662,
		0x40024002,
		0x43C243C2,
		0x24242424,
		0x10081008,
		0x0FF00FF0,
		0x02400240,
		0x0E700E70,
		0x00000000
	};
	
	unsigned int invader_16_bitmap[INVADER_16_HEIGHT] =
	{
		0x0000,
		0x0810,
		0x0810,
		0x0420,
		0x0240,
		0x1FF8,
		0x2004,
		0x4662,
		0x4002,
		0x43C2,
		0x2424,
		0x1008,
		0x0FF0,
		0x0240,
		0x0E70,
		0x0000
	};
    unsigned int cursor_16_bitmap[INVADER_16_HEIGHT] =
	{
		0x8000,
		0xC000,
		0xA000,
		0x9000,
		0x8800,
		0x8400,
		0x8200,
		0x8100,
		0x8080,
		0x8040,
		0x8020,
		0x8FF0,
		0x9000,
		0xA000,
		0xC000,
		0x8000
	};

	unsigned int ufo_16[16] = 
	{
		0x03E0,
		0x0810,
		0x1248,
		0x2184,
		0x43E2,
		0x4812,
		0x8811,
		0x8421,
		0xFFFF,
		0x8001,
		0x4C32,
		0x4DB2,
		0x2184,
		0x3C3C,
		0x2BD4,
		0x1008
	};	
	
	unsigned char invader_08_bitmap[INVADER_08_HEIGHT] =
	{
		0x00,
		0x10,
		0x10,
		0x20,
		0x40,
		0xF8,
		0x04,
		0x62
	};

	unsigned char tinyUFO[8] =
	{
		0x3C,
		0x42,
		0x99,
		0x99,
		0xFF,
		0x42,
		0x24,
		0x5A
	};	


	unsigned char soldier_right[SOLDIER_HEIGHT] = 
	{
		0x99,
		0x99,
		0x52,
		0x3C,
		0x18,
		0x18,
		0x18,
		0x18,
		0x1E,
		0x12,
		0xF2,
		0x83
	};

	unsigned char soldier_left[SOLDIER_HEIGHT] = 
	{
		0x99,
		0x99,
		0x4A,
		0x3C,
		0x18,
		0x18,
		0x18,
		0x18,
		0x78,
		0x48,
		0x4F,
		0xC1
	};

	unsigned char *base08 = Physbase();
	unsigned int *base16 = Physbase();	
	unsigned long *base32 = Physbase();
/*
	while(y <= 200)
	{
		horizline(base,0,160,y);
		y++;
	}
	while(x < 320)
	{
		vertline(base,0,200,x);
		x++;
	}
*/
	clear(base,0,100,0,100);

	x = 196;
	y = 196;

	plot_bitmap16(base16, x, y, cursor_16_bitmap, 16);

	x = 9;
	y = 9;
/*
	plot_bitmap16(base16, x, y, invader_16_bitmap, INVADER_16_HEIGHT);

	x = 293;
	y = 300;
	
	plot_bitmap08(base08, x, y, invader_08_bitmap, INVADER_08_HEIGHT);
	
	y = y+20;

*/	
	plot_bitmap32(base32, x, y, plane_right, PLANE_HEIGHT);
	x = x+40;
	plot_bitmap32(base32, x, y, plane_left, PLANE_HEIGHT);

	y = y+40;

	plot_bitmap32(base32, x, y, invader_32_bitmap, INVADER_32_HEIGHT);
	
	x = x+40;
	plot_bitmap32(base32, x, y, theUfo, UFO_HEIGHT);

	x = x+40;
	plot_bitmap08(base08, x, y, tinyUFO, 8);

	x = x+40;
	plot_bitmap08(base08, x, y, soldier_left, SOLDIER_HEIGHT);

	y = y+30;
	plot_bitmap08(base08, x, y, soldier_right, SOLDIER_HEIGHT);

	return 0;
}
