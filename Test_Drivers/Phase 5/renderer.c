#include"model.h"
#include"rast.h"
#include"render.h"
#include"events.h"
#include"bitmaps.h"
#include<stdio.h>
#include<osbind.h>


void render(MainGame game)
{
	clearchanging(game);
	plot_bitmap32(game.world.base32, game.player.xposmin, (game.player.yposmin + 51), THEUFO,
		32);
	render_soldiers(game.world.base08, game.soldiersright,&(game.tailSright));
	render_soldiers(game.world.base08, game.soldiersleft,&(game.tailSleft));

	render_planes(game.world.base32, game.planesright,&(game.tailPright));
	render_planes(game.world.base32, game.planesleft,&(game.tailPleft));
	renderscore(game.player.score,game.world.base08);
	renderlives(game.player.lives,game.world.base16);
	if(game.beamfiring == TRUE)
	{
		renderbeam(game.player, game.world.base08);
	}
	else if(game.clearbeam == TRUE)
	{
		clearbeam(game.player,game.world.base32);
	}

}

void clearbeam(UFO ufo, unsigned long* base){
		clear(base,ufo.xposold,ufo.xposold+31,ufo.yposold+51,GROUND+49);
}
void renderbeam(UFO ufo, unsigned char* base)
{
	vertline(base,ufo.yposmax+51,GROUND+50,ufo.xposmin);
	vertline(base,ufo.yposmax+51,GROUND+50,ufo.xposmax);
}

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
	render_soldiers(game.world.base08, game.soldiersright,&(game.tailSright));
	render_soldiers(game.world.base08, game.soldiersleft,&(game.tailSleft));
	render_planes(game.world.base32, game.planesright,&(game.tailPright));
	render_planes(game.world.base32, game.planesleft,&(game.tailPleft));
	renderscore(game.player.score,game.world.base08);
	renderlives(game.player.lives,game.world.base16);
}

void renderlives(int lives, unsigned int* base)
{
	int i = 0;
	while(i < lives)
	{
		plot_bitmap16(base,70+(i * 20),32,UFO_16,16);	
		i++;
	}
}

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

void clearchanging(MainGame game)
{
	clear(game.world.base32,70,125,32,47);
	clear(game.world.base32,73,107,20,27);
	clear(game.world.base32,game.player.xposold,game.player.xposold+31,game.player.yposold+51,game.player.yposold+31+51);
	clearsoldiers(game);
	clearplanes(game);
}

void clearplanes(MainGame game)
{
	int i = 0;

	if(game.tailPrightold != NULL)
	{
		while((game.tailPrightold->yposold != game.planesright[i].yposold))
		{
			clear(game.world.base32,game.planesright[i].xposold,game.planesright[i].xposold+31,
			game.planesright[i].yposold +51,game.planesright[i].yposold+23+51);
			i++;
		}
	}
	else
	{
		while(i < MAX_PLANES)
		{
			clear(game.world.base32,game.planesright[i].xposold,game.planesright[i].xposold+31,
			game.planesright[i].yposold +51,game.planesright[i].yposold+23+51);
			i++;
		}
	}
	i = 0;
	if(game.tailPleftold != NULL)
	{
		while((game.tailPleftold->yposold != game.planesleft[i].yposold))
		{
			clear(game.world.base32,game.planesleft[i].xposold,game.planesleft[i].xposold+31,
			game.planesleft[i].yposold +51,game.planesleft[i].yposold+23+51);
			i++;
		}
	}
	else
	{
		while(i < MAX_PLANES)
		{
			clear(game.world.base32,game.planesleft[i].xposold,game.planesleft[i].xposold+31,
			game.planesleft[i].yposold +51,game.planesleft[i].yposold+23+51);
			i++;
		}
	}
}
void clearsoldiers(MainGame game)
{
	int i = 0;
	if(game.tailSrightold != NULL)
	{
		while((game.tailSrightold->xposold != game.soldiersright[i].xposold))
		{
			clear(game.world.base32,game.soldiersright[i].xposold,game.soldiersright[i].xposold+7,381,391);
			i++;
		}
	}
	else
	{
		while(i<MAX_SOLDIERS)
		{
			clear(game.world.base32,game.soldiersright[i].xposold,game.soldiersright[i].xposold+7,381,391);
			i++;
		}
	}
	i = 0;
	if(game.tailSleftold != NULL)
	{
		while((game.tailSleftold->xposold != game.soldiersleft[i].xposold))
		{
			clear(game.world.base32,game.soldiersleft[i].xposold,game.soldiersleft[i].xposold+7,381,391);
			i++;
		}
	}
	else
	{
		while(i<MAX_SOLDIERS)
		{
			clear(game.world.base32,game.soldiersleft[i].xposold,game.soldiersleft[i].xposold+7,381,391);
			i++;
		}
	}
}

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