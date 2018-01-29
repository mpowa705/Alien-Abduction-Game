#include"model.h"
#include"rast.h"
#include"VBL.h"
#include"render.h"
#include"events.h"
#include"bitmaps.h"
#include"globals.h"
#include"input.h"
#include<stdio.h>
#include<osbind.h>

/*Team: Matthew Powaschuk, Brady Kirchner
COMP 2659 - Winter 2016
Prof: Paul Pospisil
Project: "Invader's revenge"
*/

/*
Purpose: This function renders the entire state of the model to the back 
buffer.
Details: This begins by clearing all moving parts to the model, and plotting
each moving part where it is currently. If the player is currently
firing, is renders the beam, if the player has recently stopped 
firing (game.cleabeam is TRUE), the beam is cleared.
Input: Takes in the model.
*/
void render(MainGame game)
{
	clearchanging(game);
	plot_bitmap32(game.world.base32, game.player.xposmin, (game.player.yposmin + 51), THEUFO,
		32);
	preventcursor(game.world.base08,game.world.base32);
	if(game.beamfiring == TRUE)
	{
		renderbeam(game.player, game.world.base08);
	}
	else if(game.clearbeam == TRUE)
	{
		clearbeam(game.player,game.world.base08);
	}
	render_soldiers(game.world.base08, game.soldiersright,&(game.tailSright));
	render_soldiers(game.world.base08, game.soldiersleft,&(game.tailSleft));
	render_planes(game.world.base32, game.planesright,&(game.tailPright));
	render_planes(game.world.base32, game.planesleft,&(game.tailPleft));
	renderscore(game.player.score,game.world.base08);
	renderlives(game.player.lives,game.world.base16);


}


/*Note: The reason for +50s and +51s is because what the model thinks
of as position 0, is pixel 51, due to the presence of the 
scoreboard.
*/

/*
Purpose: This function renders the beam underneath the UFO
Details: This works by drawing 2 vertical lines at the ends of the UFO
starting directly underneath the UFO until the ground.
Input: Takes in the UFO and a pointer to the buffer.
*/
void renderbeam(UFO ufo, unsigned char* base)
{
	vertline(base,ufo.yposmax+51,GROUND+50,ufo.xposmin);
	vertline(base,ufo.yposmax+51,GROUND+50,ufo.xposmax);
}

/*
Purpose: This sets up the first buffer with all the unmoving parts,
as well as all moving parts.
Details: This works by calling the raster functions that draw the 
exterior lines, the ground, the text, and all moving entities.
Input: This takes in the model.
*/
void renderfirst(MainGame game){
	clearscreen(game.world.base32);
	horizline(game.world.base08,0,149,0);
	horizline(game.world.base08,0,149,48);
	vertline(game.world.base08,0, 48, 149);
	vertline(game.world.base08,0, 48, 0);
	vertline(game.world.base08,49, 399, 0);
	vertline(game.world.base08,49, 399, 639);
	horizline(game.world.base08,0,639,50);
	horizline(game.world.base08,0,639,392);
	horizline(game.world.base08,0,639,399);
	plot_bitmap08(game.world.base08,4,4,font+(8*48),8);
	plot_bitmap08(game.world.base08,14,4,font+(8*44),8);
	plot_bitmap08(game.world.base08,24,4,font+(8*33),8);
	plot_bitmap08(game.world.base08,34,4,font+(8*57),8);
	plot_bitmap08(game.world.base08,44,4,font+(8*37),8);
	plot_bitmap08(game.world.base08,54,4,font+(8*50),8);
	plot_bitmap08(game.world.base08,70,4,font+(8*17),8);
	plot_bitmap08(game.world.base08,4,20,font+(8*51),8);
	plot_bitmap08(game.world.base08,14,20,font+(8*35),8);
	plot_bitmap08(game.world.base08,24,20,font+(8*47),8);
	plot_bitmap08(game.world.base08,34,20,font+(8*50),8);
	plot_bitmap08(game.world.base08,44,20,font+(8*37),8);
	plot_bitmap08(game.world.base08,54,20,font+(8*26),8);
	plot_bitmap08(game.world.base08,4,36,font+(8*44),8);
	plot_bitmap08(game.world.base08,14,36,font+(8*41),8);
	plot_bitmap08(game.world.base08,24,36,font+(8*54),8);
	plot_bitmap08(game.world.base08,34,36,font+(8*37),8);
	plot_bitmap08(game.world.base08,44,36,font+(8*51),8);
	plot_bitmap08(game.world.base08,54,36,font+(8*26),8);	
	plot_bitmap32(game.world.base32, game.player.xposmin, (game.player.yposmin + 51), THEUFO,
		32);
	preventcursor(game.world.base08,game.world.base32);
	render_soldiers(game.world.base08, game.soldiersright,&(game.tailSright));
	render_soldiers(game.world.base08, game.soldiersleft,&(game.tailSleft));
	render_planes(game.world.base32, game.planesright,&(game.tailPright));
	render_planes(game.world.base32, game.planesleft,&(game.tailPleft));
	renderscore(game.player.score,game.world.base08);
	renderlives(game.player.lives,game.world.base16);

}

/*
Purpose: This renders the 16x16 UFO bitmaps that represent the lives
the player has remaining.
Details: This works by calling the raster function that draws a 16-wide
bitmap for each life, which are drawn 4 pixels apart.
Input: This takes in the amount of lives, and a pointer to the buffer.
*/
void renderlives(int lives, unsigned int* base)
{
	int i = 0;
	while(i < lives)
	{
		plot_bitmap16(base,70+(i * 20),32,UFO_16,16);	
		i++;
	}
}

/*
Purpose: This doesn't fully prevent the cursor from showing, but
prevents it from clearing soldiers.
Details: This works by calling the raster functions to clear the area
where the cursor is rendered, and draws the ends of the lines that were
there.
Input: This takes in 2 pointers to the buffer.
*/
void preventcursor(unsigned char* base, unsigned long* base2)
{
	clear(base2,0,15,384,399);
	horizline(base,0,15,392);
	horizline(base,0,15,399);
	vertline(base,384, 399, 0);
}

/*
Purpose: This renders the 8X8 number bitmaps that represent the score
the player has accumulated.
Details: This works by calling the raster function that draws a 8-wide
bitmap for each of the digits, which are taken by modding 10.
Input: This takes in the score, and a pointer to the buffer.
*/
void renderscore(int score, unsigned char* base)
{
	int toprint = 0;
	int i = 0;
	if(score == 0)
		plot_bitmap08(base,100-(9*i),20,font+(8*(toprint+16)),8);
	else if(score < 10000)
	{
		while(score >= 10)
		{
			toprint = score % 10;
			score = score / 10;
			plot_bitmap08(base,100-(9*i),20,font+(8*(toprint+16)),8);
			i++;
		}
		toprint = score % 10;
		score = score / 10;
		plot_bitmap08(base,100-(9*i),20,font+(8*(toprint+16)),8);
	}
}


/*
Purpose: This function clears the area where the beam was fired by the UFO
Details: This function clears the 2 lines underneath the UFO at it's lowest
and highest x locations, from directly underneath the UFO to the ground - 1 pixel.
Input: Takes in the UFO and a pointer to the buffer.
*/
void clearbeam(UFO ufo, unsigned char* base){
		clearvertline(base,ufo.yposold2+31+50, GROUND-1+50, ufo.xposold2);
		clearvertline(base,ufo.yposold2+31+50, GROUND-1+50, ufo.xposold2+31);
}

/*
Purpose: This function clears all moving entities, from 2 frames ago.
Details: This function clears where the score and lives are, and where the UFO,
the soldiers and the planes.
Input: Takes in the model.
*/
void clearchanging(MainGame game)
{
	clear(game.world.base32,70,125,32,47);
	clear(game.world.base32,73,107,20,27);
	clear(game.world.base32,game.player.xposold2,game.player.xposold2+31,game.player.yposold2+51,game.player.yposold2+31+51);
	clearsoldiers(game);
	clearplanes(game);
}

/*
Purpose: This function clears all planes, where they were 2 frames ago.
Details: This function clears all planes until it reaches the tail pointer or the end
of the array.
Input: Takes in the model.
*/
void clearplanes(MainGame game)
{
	int i = 0;

	if(game.tailPrightold2 != NULL)
	{
		while((game.tailPrightold2->yposold2 != game.planesright[i].yposold2))
		{
			clear(game.world.base32,game.planesright[i].xposold2,game.planesright[i].xposold2+31,
			game.planesright[i].yposold2 +51,game.planesright[i].yposold2+23+51);
			i++;
		}
	}
	else
	{
		while(i < MAX_PLANES)
		{
			clear(game.world.base32,game.planesright[i].xposold2,game.planesright[i].xposold2+31,
			game.planesright[i].yposold2+51,game.planesright[i].yposold2+23+51);
			i++;
		}
	}
	i = 0;
	if(game.tailPleftold2 != NULL)
	{
		while((game.tailPleftold2->yposold2 != game.planesleft[i].yposold2))
		{
			clear(game.world.base32,game.planesleft[i].xposold2,game.planesleft[i].xposold2+31,
			game.planesleft[i].yposold2 +51,game.planesleft[i].yposold2+23+51);
			i++;
		}
	}
	else
	{
		while(i < MAX_PLANES)
		{
			clear(game.world.base32,game.planesleft[i].xposold2,game.planesleft[i].xposold2+31,
			game.planesleft[i].yposold2 +51,game.planesleft[i].yposold2+23+51);
			i++;
		}
	}
}

/*
Purpose: This function clears all soldiers, where they were 2 frames ago.
Details: This function clears all soldiers until it reaches the tail pointer or the end
of the array.
Input: Takes in the model.
*/
void clearsoldiers(MainGame game)
{
	int i = 0;
	if(game.tailSrightold2 != NULL)
	{
		while((game.tailSrightold2->xposold2 != game.soldiersright[i].xposold2))
		{
			clear(game.world.base32,game.soldiersright[i].xposold2,game.soldiersright[i].xposold2+7,381,391);
			i++;
		}
	}
	else
	{
		while(i<MAX_SOLDIERS)
		{
			clear(game.world.base32,game.soldiersright[i].xposold2,game.soldiersright[i].xposold2+7,381,391);
			i++;
		}
	}
	i = 0;
	if(game.tailSleftold2 != NULL)
	{
		while((game.tailSleftold2->xposold2 != game.soldiersleft[i].xposold2))
		{
			clear(game.world.base32,game.soldiersleft[i].xposold2,game.soldiersleft[i].xposold2+7,381,391);
			i++;
		}
	}
	else
	{
		while(i<MAX_SOLDIERS)
		{
			clear(game.world.base32,game.soldiersleft[i].xposold2,game.soldiersleft[i].xposold2+7,381,391);
			i++;
		}
	}
}

/*
Purpose: This function renders all planes to the buffer.
Details: This function renders all planes as 32-wide bitmaps
until it reaches the tail pointer or the end of the array.
Input: Takes in the model.
*/
void render_planes(unsigned long* base, Plane toshow[],Plane* *tail)
{
	int i = 0;
	if(*(tail) != NULL)
	{
		while((*(*tail)).yposmin != toshow[i].yposmin)
		{
			if(toshow[i].lefttoright == TRUE)
			{
				plot_bitmap32(base,toshow[i].xposmin,toshow[i].yposmin+51,PLANE_RIGHT,24);
				i = i+1;
			}
			else
			{
				plot_bitmap32(base,toshow[i].xposmin,toshow[i].yposmin+51,PLANE_LEFT,24);
				i = i+1;
			}
		}
	}
	else
	{
		while(i < MAX_PLANES)
		{
		if(toshow[i].lefttoright == TRUE)
			{
				plot_bitmap32(base,toshow[i].xposmin,toshow[i].yposmin+51,PLANE_RIGHT,24);
				i = i+1;
			}
			else
			{
				plot_bitmap32(base,toshow[i].xposmin,toshow[i].yposmin+51,PLANE_LEFT,24);
				i = i+1;
			}	
		}
	}
}

/*
Purpose: This function renders all soldiers to the buffer.
Details: This function renders all soldiers as 32-wide bitmaps
until it reaches the tail pointer or the end of the array.
Input: Takes in the model.
*/
void render_soldiers(unsigned char* base, Soldier toshow[],Soldier* *tail)
{
	int i = 0;
	if(*(tail) != NULL)
	{
		while((*(*tail)).xposmin != toshow[i].xposmin)
		{
			if(toshow[i].lefttoright == TRUE)
			{
			plot_bitmap08(base,toshow[i].xposmin,381,SOLDIER_RIGHT,12);
			i = i+1;
			}
			else
			{
			plot_bitmap08(base,toshow[i].xposmin,381,SOLDIER_LEFT,12);
			i = i+1;
			}
		}
	}
	else
	{
		while(i < MAX_SOLDIERS)
		{
			if(toshow[i].lefttoright == TRUE)
			{
			plot_bitmap08(base,toshow[i].xposmin,381,SOLDIER_RIGHT,12);
			i = i+1;
			}
			else
			{
			plot_bitmap08(base,toshow[i].xposmin,381,SOLDIER_LEFT,12);
			i = i+1;
			}
		}
	}
}

char rendersplash(MainGame game){
    
    unsigned char *charBase = Physbase();
    unsigned long *longBase = Physbase();
    unsigned char input;
    
    set_splash(charBase, splash);
    clear(longBase, 20, 156, 200, 215);
    clear(longBase, 252, 388, 200, 215);
    clear(longBase, 483, 619, 200, 215);
    plot_bitmap08(game.world.base08,30,204,font+(8*0),8);
	plot_bitmap08(game.world.base08,37,204,font+(8*0),8);
	plot_bitmap08(game.world.base08,44,204,font+(8*17),8);
	plot_bitmap08(game.world.base08,51,204,font+(8*0),8);
	plot_bitmap08(game.world.base08,58,204,font+(8*48),8);
	plot_bitmap08(game.world.base08,65,204,font+(8*44),8);
	plot_bitmap08(game.world.base08,72,204,font+(8*33),8);
	plot_bitmap08(game.world.base08,79,204,font+(8*57),8);
    plot_bitmap08(game.world.base08,86,204,font+(8*37),8);
	plot_bitmap08(game.world.base08,93,204,font+(8*50),8);
	plot_bitmap08(game.world.base08,100,204,font+(8*0),8);
	plot_bitmap08(game.world.base08,107,204,font+(8*39),8);
	plot_bitmap08(game.world.base08,114,204,font+(8*33),8);
	plot_bitmap08(game.world.base08,121,204,font+(8*45),8);
	plot_bitmap08(game.world.base08,128,204,font+(8*37),8);
	plot_bitmap08(game.world.base08,135,204,font+(8*0),8);
    plot_bitmap08(game.world.base08,142,204,font+(8*0),8);
    
    plot_bitmap08(game.world.base08,262,204,font+(8*0),8);
	plot_bitmap08(game.world.base08,269,204,font+(8*0),8);
	plot_bitmap08(game.world.base08,276,204,font+(8*18),8);
	plot_bitmap08(game.world.base08,283,204,font+(8*0),8);
	plot_bitmap08(game.world.base08,290,204,font+(8*48),8);
	plot_bitmap08(game.world.base08,297,204,font+(8*44),8);
	plot_bitmap08(game.world.base08,304,204,font+(8*33),8);
	plot_bitmap08(game.world.base08,311,204,font+(8*57),8);
    plot_bitmap08(game.world.base08,318,204,font+(8*37),8);
	plot_bitmap08(game.world.base08,325,204,font+(8*50),8);
	plot_bitmap08(game.world.base08,332,204,font+(8*0),8);
	plot_bitmap08(game.world.base08,339,204,font+(8*39),8);
	plot_bitmap08(game.world.base08,346,204,font+(8*33),8);
	plot_bitmap08(game.world.base08,353,204,font+(8*45),8);
	plot_bitmap08(game.world.base08,360,204,font+(8*37),8);
	plot_bitmap08(game.world.base08,367,204,font+(8*0),8);
    plot_bitmap08(game.world.base08,374,204,font+(8*0),8);
    
    plot_bitmap08(game.world.base08,493,204,font+(8*0),8);
	plot_bitmap08(game.world.base08,500,204,font+(8*0),8);
	plot_bitmap08(game.world.base08,507,204,font+(8*33),8);
	plot_bitmap08(game.world.base08,514,204,font+(8*34),8);
	plot_bitmap08(game.world.base08,521,204,font+(8*33),8);
	plot_bitmap08(game.world.base08,528,204,font+(8*46),8);
	plot_bitmap08(game.world.base08,535,204,font+(8*36),8);
	plot_bitmap08(game.world.base08,542,204,font+(8*47),8);
    plot_bitmap08(game.world.base08,549,204,font+(8*46),8);
	plot_bitmap08(game.world.base08,556,204,font+(8*0),8);
	plot_bitmap08(game.world.base08,563,204,font+(8*51),8);
	plot_bitmap08(game.world.base08,570,204,font+(8*40),8);
	plot_bitmap08(game.world.base08,577,204,font+(8*41),8);
	plot_bitmap08(game.world.base08,584,204,font+(8*48),8);
	plot_bitmap08(game.world.base08,591,204,font+(8*1),8);
	plot_bitmap08(game.world.base08,598,204,font+(8*0),8);
    plot_bitmap08(game.world.base08,605,204,font+(8*0),8);
    
    while(input != 49){
        input = userinput();
    }

    return input;
    
}