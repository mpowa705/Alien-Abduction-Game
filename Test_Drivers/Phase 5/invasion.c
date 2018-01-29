#include"model.h"
#include"rast.h"
#include"render.h"
#include"events.h"
#include"input.h"
#include"invasion.h"
#include<stdio.h>
#include<osbind.h>


int main()
{

unsigned long timenow = 0, spawnplane = 0,spawnsoldier = 0,unfire = 0, beamready = 0; 
MainGame game;
int nextplane = R,nextsoldier = L, fired = FALSE, beamcleared = FALSE;
char inreq;
int soldiers = 0;
initializegame(&(game));
timenow = pollclock();
spawnplane = timenow + 70;
spawnsoldier = timenow + 140;
renderfirst(game);
while(game.player.lives != 0)
{
	nextframe(&(game));
	timenow = pollclock();
	if((game.beamfiring == TRUE) && (timenow >= unfire))
	{
		stopfiring(&(game));
	}
	if((game.player.BeamReady == FALSE) && (timenow >= beamready))
	{
		game.player.BeamReady = TRUE;
	}
	if(Cconis())
	{
		inreq = userinput();
		if(inreq == ' ')
		{
		fired = requestfirebeam(&(game),&(game.player),game.soldiersleft,game.soldiersleft, &(game.tailSleft),&(game.tailSright));
		fseek(stdin,0,SEEK_END);
		if(fired == TRUE)
			{
			unfire = timenow + 20;
			beamready = timenow + 300;
			/*since firebeam isnt working yet, it just incs by 20 every beam fired*/
			game.player.score += 20;
			}
		}
		else
		{
		requestmoveufo((int)inreq, &(game), &(game.player),&(game.world));
		fseek(stdin,0,SEEK_END);
		}
	}
	if(timenow >= spawnplane)
	{
		spawnplane = spawnplane + 70;
		if(nextplane == R)
		{
			spawn_plane(R,game.planesright,&(game.tailPright),&(game.world),
			&(game));
			nextplane = L;
		}
		else
		{
			spawn_plane(L,game.planesleft,&(game.tailPleft),&(game.world),
			&(game));
			nextplane = R;
		}
	}
	if(timenow >= spawnsoldier)
	{
		spawnsoldier = spawnsoldier + 140;
		soldiers++;
		if(nextsoldier == R)
		{
			spawn_soldier(R, game.soldiersright,&(game.tailSright));
			nextsoldier = L;
		}
		else
		{
			spawn_soldier(L, game.soldiersleft,&(game.tailSleft));
			nextsoldier = R;
		}
		
	}
	requestmovesoldiers(&(game),&(game.player));
	requestmoveplanes(&(game), &(game.player), &(game.world));	
	render(game);
	if(game.clearbeam == TRUE)
	{
		game.clearbeam = FALSE;
	}
	
}
/*spawn_soldier(R, game.soldiersright,&(game.tailSright));
renderfirst(game);
nextframe(&(game));
game.soldiersright[0].xposmin = 40;
spawn_soldier(R, game.soldiersright,&(game.tailSright));
render(game);
nextframe(&(game));
game.soldiersright[1].xposmin = 20;
spawn_soldier(R, game.soldiersright,&(game.tailSright));
render(game);
nextframe(&(game));
game.soldiersright[2].xposmin = 60;
spawn_soldier(R, game.soldiersright,&(game.tailSright));
render(game);
nextframe(&(game));
game.soldiersright[3].xposmin = 80;
spawn_soldier(R, game.soldiersright,&(game.tailSright));
render(game);
nextframe(&(game));
game.soldiersright[4].xposmin = 100;*/
/*spawn_soldier(FALSE, game.soldiersright,&(game.tailSright));
renderfirst(game);
Cnecin();
game.soldiersright[0].xposmin = 400;
game.soldiersright[0].xposmax = 407;
render(game);
Cnecin();*/
return 0;
}

unsigned long pollclock()
{
	unsigned long *timer = (unsigned long *)0x462;
	unsigned long timeNow;
	unsigned long old_ssp;
	old_ssp = Super(0); /* enter privileged mode */
	timeNow = *timer;
	Super(old_ssp); 
	return timeNow;
}