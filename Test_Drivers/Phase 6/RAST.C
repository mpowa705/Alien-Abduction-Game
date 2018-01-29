#include<osbind.h>
#include<stdio.h>
#include"rast.h"

/*Team: Matthew Powaschuk, Brady Kirchner
COMP 2659 - Winter 2016
Prof: Paul Pospisil
Project: "Invader's revenge"
*/

/*
Purpose:This function plots a single pixel to the frame buffer.

Input: This function is passed a pointer to the frame buffer, and a set of coordinates.

Output: The function returns nothing but the pixel is plotted to the screen on the next
	vsync.

Written by: Matt
*/
void plot_pixel(unsigned char* base,int x,int y)
{
		*(base + y * 80 + (x >> 3)) |= 1 << 7 - (x & 7);
}

/*
The following three functions assume that the "1" variable(s) being passed in 
has/have a lower value than the "2" variable(s)

Purpose: This function plots a horizontal line to the frame buffer,
	 1 pixel in height.

Input: 	This function is passed a pointer to the frame buffer, 
	a start x (x1) and a final x (x2), as well as the y pixel the
	line is drawn along. 

Output: This function doesn't return anything.

Details:This function is seperate from vertical line because
	this function, unlike the other one, does not call
	plot_pixel.

Written by: Matt
*/

void horizline(unsigned char* base,int x1, int x2, int y)
{
	if((x1 >> 3) == (x2 >> 3))
	{
		*(base + y * 80 + (x1 >> 3)) |= 
		((0xFF >> (x1 & 7)) & (0xFF << (7- (x2 & 7))));
	}
	else
	{
		*(base + y * 80 + (x1 >>3)) |= (0xFF >> (x1 & 7));
		for(x1 = ((x1 >> 3) + 1); x1 < (x2 >> 3); x1++)
		{
			*(base + y * 80 + x1) = 0xFF;
		}
		*(base + y * 80 + (x2 >>3)) |= (0xFF << (7-(x2 & 7)));
	}
}

/*
Purpose: This function plots a vertical line to the frame buffer,
	 1 pixel wide.

Input:  This function is passed a pointer to the frame buffer, 
	a start y (y1) and a final y (y2), as well as the x pixel the
	line is drawn along.

Written by: Matt
*/

void vertline(unsigned char* base,int y1, int y2, int x)
{
	while(y1 <= y2)
	{
		plot_pixel(base,x,y1);
		y1++;
	}
}

/*
Purpose: This function turns all pixels in the x1-x2 width and y1-y2 height
	 off in the frame buffer.

Input:	This pixel is passed a pointer to the frame buffer, and a region to clear.

Written by: Matt
*/

void clear(unsigned long* base,int x1, int x2, int y1, int y2)
{
	int xtemp = 0;
	while(y1 <= y2)
	{
	if((x1 >> 5) == (x2 >> 5))
	{
		*(base + y1 * 20 + (x1 >> 5)) &= 
		((0xFFFFFFFF << (32 - (x1 & 31))) | (0xFFFFFFFF >> (x2 & 31)+1));
	}
	else
	{
		*(base + y1 * 20 + (x1 >>5)) &= (0xFFFFFFFF << (32 - (x1 & 31)));
		for(xtemp = ((x1 >> 5) + 1); xtemp < (x2 >> 5); xtemp++)
		{
			*(base + y1 * 20 + xtemp) = 0x00000000;
		}
		*(base + y1 * 20 + (x2 >>5)) &= (0xFFFFFFFF >> ((x2 & 31)+1));
	}
	y1++;
	}
}

/*
Purpose: This function turns all pixels in the x width and y1-y2 height
	 off in the frame buffer.

Input:	This pixel is passed a pointer to the frame buffer, and a vetical line to clear.

Written by: Matt
*/
void clearvertline(unsigned char* base,int y1, int y2, int x)
{
	while(y1 <= y2)
	{
	*(base + y1 * 80 + (x >> 3)) &= ((0xFF << (8 - (x & 7))) | (0xFF >> (x & 7)+1));
	y1++;
	}
}

/*
Purpose: This function sets all pixels in the buffer to 0.

Input:	This pixel is passed a pointer to the frame buffer.

Written by: Matt
*/
void clearscreen(unsigned long* base)
{
	int x = 0;
	int y = 0;
	while(y <= 399)
	{
		while(x < 20)
		{
			*(base + y * 20 + x) = 0x00000000;
			x++;
		}
		x = 0;
		y++;
	}
}
/*
Purpose: This function prints a given 8-pixel wide bitmap of the indicated 
height to the screen.

Input:	
*base: pointer to first byte in the frame buffer
x: x-coordinate of where to print the bitmap
y: y-coordinate of where to print the bitmap
*bitmap: pointer to an array containing the bitmap data
height: height of the bitmap

Output: 8-pixel wide bitmap drawn to the screen

Written by: Brady
*/
void plot_bitmap08(unsigned char *base, int x, int y, unsigned char *bitmap,
			unsigned int height){
	unsigned int lineCount = 0;
	for(; lineCount < height; lineCount = lineCount + 1){
		*(base+((y+lineCount)*(80))+(x>>3)) |= 
			 (bitmap[lineCount] >> (x&7));
		*(base+1+((y+lineCount)*(80))+(x>>3)) |=
			 (bitmap[lineCount] << 8-(x&7));
	}
}
/*

Purpose: This function prints a given 16-pixel wide bitmap of the indicated 
height to the screen.

Input:	
*base: pointer to first byte in the frame buffer
x: x-coordinate of where to print the bitmap
y: y-coordinate of where to print the bitmap
*bitmap: pointer to an array containing the bitmap data
height: height of the bitmap

Output: 16-pixel wide bitmap drawn to the screen.

Written by: Brady
*/
void plot_bitmap16(unsigned int *base, int x, int y, unsigned int *bitmap,
			unsigned int height){

	unsigned int lineCount = 0;

	for(; lineCount < height; lineCount = lineCount + 1){
		*(base+((y+lineCount)*(40))+(x>>4))
			|= (bitmap[lineCount] >> (x&15));
		*(base+1+((y+lineCount)*(40))+(x>>4))
			|= (bitmap[lineCount] << 16-(x&15));

	}
}
/*
Purpose: This function prints a given 32-pixel wide bitmap of the indicated 
height to the screen.

Input:	
*base: pointer to first byte in the frame buffer
x: x-coordinate of where to print the bitmap
y: y-coordinate of where to print the bitmap
*bitmap: pointer to an array containing the bitmap data
height: height of the bitmap

Output: 32-pixel wide bitmap drawn to the screen.

Written by: Brady
*/
void plot_bitmap32(unsigned long *base, int x, int y, unsigned long *bitmap,
			unsigned int height){

	unsigned int lineCount = 0;
	for(; lineCount < height; lineCount = lineCount + 1){
		*(base+((y+lineCount)*20) + (x>>5))
			|= bitmap[lineCount] >> (x&31);
		*(base+1+((y+lineCount)*20)+(x>>5))
			|= bitmap[lineCount] << 32 - (x&31);
	}
}
