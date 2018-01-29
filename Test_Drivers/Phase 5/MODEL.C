#include"events.h"
#include"model.h"
#include<osbind.h>
#include<stdio.h>

void initializegame(MainGame *game)
{
	int i = 0;
	game->gameover = FALSE;
	game->world.base08 = Physbase();
	game->world.base16 = Physbase();
	game->world.base32 = Physbase();
	game->player.lives = 3;
	game->player.score = 0;
	game->player.yposmin = 176; /*coords of UFO when it spawns*/
	game->player.yposmax = 207;
	game->player.xposmin = 185;
	game->player.xposmax = 216;
	game->player.xposold = 185;
	game->player.xposold2 = 185;
	game->player.yposold = 176;
	game->player.yposold2 = 176;
	game->player.BeamReady = TRUE;
	game->planespeed = DEFAULTPLANESPEED;       
/* each "move" will move the plane 2 pixels to the right at first*/
	game->planespawnspeed = 5;  /* one plane per 5 seconds at first*/
	game->beamfiring = FALSE;
	game->player.lives = 3;
	/* These pointers point at the tail of the "queue"
		P = plane, M = missile, S = soldier */
	game->tailPleft = &(game->planesleft[0]); 
	game->tailPright = &(game->planesright[0]);	   
	game->tailPrightold = game->tailPright;
	game->tailPleftold = game->tailPleft;
	game->tailPrightold2 = game->tailPrightold;
	game->tailPleftold2 = game->tailPleftold;
	game->tailMright = &(game->missilesright[0]);
	game->tailMleft = &(game->missilesleft[0]);
	game->tailSright = &(game->soldiersright[0]);
	game->tailSleft = &(game->soldiersleft[0]);
	game->tailSrightold = game->tailSright;
	game->tailSleftold = game->tailSleft;
	game->tailSrightold2 = game->tailSrightold;
	game->tailSleftold2 = game->tailSleftold;


	while(i < HEIGHT_OF_SKY)
	{
		game->world.occupiedy[i] = FALSE; 
		/*y values that planes are "on" are set to true*/
		i++;
	}

	return;
}

void nextframe(MainGame* game)
{
	int i = 0;
	game->player.xposold2 = game->player.xposold; 
	game->player.xposold = game->player.xposmin;
	game->player.yposold2 = game->player.yposold; 
	game->player.yposold = game->player.yposmin;
	while(i < MAX_PLANES)
	{
		game->planesright[i].xposold2 = game->planesright[i].xposold;
		game->planesright[i].xposold = game->planesright[i].xposmin;
		game->planesright[i].yposold2 = game->planesright[i].yposold;
		game->planesright[i].yposold = game->planesright[i].yposmin;	
		i++;
	}
	i = 0;
	while(i < MAX_PLANES)
	{
		game->planesleft[i].xposold2 = game->planesleft[i].xposold;
		game->planesleft[i].xposold = game->planesleft[i].xposmin;
		game->planesleft[i].yposold2 = game->planesleft[i].yposold;
		game->planesleft[i].yposold = game->planesleft[i].yposmin;	
		i++;
	}
	i = 0;
	while(i < MAX_SOLDIERS)
	{
		game->soldiersright[i].xposold2 = game->soldiersright[i].xposold;
		game->soldiersright[i].xposold = game->soldiersright[i].xposmin;
		i++;
	}
	i = 0;
	while(i < MAX_SOLDIERS)
	{
		game->soldiersleft[i].xposold2 = game->soldiersleft[i].xposold;
		game->soldiersleft[i].xposold = game->soldiersleft[i].xposmin;
		i++;
	}
	game->tailPrightold2 = game->tailPrightold;
	game->tailPleftold2 = game->tailPleftold;
	game->tailPrightold = game->tailPright;
	game->tailPleftold = game->tailPleft;
	game->tailSrightold2 = game->tailSrightold;
	game->tailSleftold2 = game->tailSleftold;
	game->tailSrightold = game->tailSright;
	game->tailSleftold = game->tailSleft;

}

/* First, if beamfiring is true, the UFO does not move.

Otherwise, this function will check if a collision would occur, 
if yes, this function will not move the UFO (if the collision
is with a soldier(s) or missile(s), that/those stuct(s) will be despawned as well), 
if the collision is fatal, it will despawn the UFO, and return a true telling main()
it needs to respawn a UFO when it can. If the UFO survives it returns false.
if the move can be safely made, it will call moveufo in model.c 

ASSUMPTIONS: 

- At maximum, one left moving + 1 right moving soldier may be collided with on a horizontal move UFO request.

*/
void requestmoveufo(int direction, MainGame *game, UFO *ufo, Map *map){
	int invalidMove = 0; /*Bool*/
	int removeSleftcounter = 0;
	int removeSrightcounter = 0;
	int i=0;
	int soldierHit = 0; /*Bool*/
	Soldier *toremoveSleft[MAX_SOLDIERS];
	Soldier *toremoveSright[MAX_SOLDIERS];
		
	if(!(game->beamfiring)){
	
		if(direction == ASCII_W || direction == ASCII_w){
			if(!(ufo->yposmin < (BOUNDARY_TOP+UFO_SPEED))){ /*Checks out-of-bounds*/
				invalidMove = check_collision_ufoVSplane_vert(direction, game, ufo, map); 
									/*Check UFO up vs plane collision*/
				if(invalidMove == 0)
					invalidMove = check_collision_ufoVSmissile_vert(direction, game, ufo);
									/*Check UFO up vs missile collision*/
			}
			else invalidMove = 1;
		}				

		else if(direction == ASCII_A || direction == ASCII_a){
			if(!(ufo->xposmin < (BOUNDARY_LEFT+UFO_SPEED))){ /*Checks out-of-bounds*/	
				invalidMove = check_collision_ufoVSplane_horiz(direction, game, ufo, map);
									/*Check UFO up vs plane collision*/
				if(invalidMove == 0)
					invalidMove = check_collision_ufoVSmissile_horiz(direction, game, ufo);
									/*Check UFO up vs missile collision*/
				if(invalidMove == 0 && ufo->yposmax >= GROUND-SOLDIER_HEIGHT){ 	/*Check UFO vs Soldier horizontal left collision*/
															/*Only check if UFO at or below soldier height*/
					invalidMove = check_collision_ufoVSsoldier_horiz(direction, game, ufo);
				}
			}
			else invalidMove = 1;			
		}
	
		else if(direction == ASCII_D || direction == ASCII_d){
			if(!(ufo->xposmax > (BOUNDARY_RIGHT-UFO_SPEED))){ /*Checks out-of-bounds*/
				invalidMove = check_collision_ufoVSplane_horiz(direction, game, ufo, map);
									/*Check UFO up vs plane collision*/
				if(invalidMove == 0)
					invalidMove = check_collision_ufoVSmissile_horiz(direction, game, ufo);
									/*Check UFO up vs missile collision*/
				if(invalidMove == 0 && ufo->yposmax >= GROUND-SOLDIER_HEIGHT){ 	/*Check UFO vs Soldier horizontal right collision*/
															/*Only check if UFO at or below soldier height*/
					invalidMove = check_collision_ufoVSsoldier_horiz(direction, game, ufo);
				}
			}
			else invalidMove = 1;
		}

		else if(direction == ASCII_S || direction == ASCII_s){
			if(ufo->yposmax >= (GROUND-UFO_SPEED)){
				destroyUFO(game);
				/*UFO CRASHED INTO THE GROUND*/
				invalidMove = 1;
			}
			else{
				invalidMove = check_collision_ufoVSplane_vert(direction, game, ufo, map);
									/*Check UFO up vs plane collision*/
				if(invalidMove == 0)
					invalidMove = check_collision_ufoVSmissile_vert(direction, game, ufo);
									/*Check UFO up vs missile collision*/						
				if((invalidMove == 0 && ufo->yposmax >= GROUND-SOLDIER_HEIGHT-UFO_SPEED) &&		/*Check UFO vs Soldier vertical collision*/
						(ufo->yposmax < GROUND-SOLDIER_HEIGHT)){ 			/*Only check if UFO just above soldier height*/														
					invalidMove = check_collision_ufoVSsoldier_vert(game, ufo);
				}
			}				 
		}
		
		if(invalidMove == 0)
			move_ufo(direction, ufo);
	}
}

/*
This function checks one by one, if the planes are at the edge of the map,
if they are, it despawns them (using the function in condition based events)
, otherwise it moves them forward (with the function in model.c), checking
for collisions with the UFO (the only other possible collision), and destroys the UFO.
If it collides with the UFO, this returns true, indicating player has died in this step.
*/
void requestmoveplanes(MainGame *game, UFO *ufo, Map *map){
	int i = 0;
	int mustDespawn = 0;
	int planeHit = 0;
	Plane *toremovePleft[MAX_PLANES];
	Plane *toremovePright[MAX_PLANES];
	int removePleftcounter = 0;
	int removePrightcounter = 0;
	
	while((game->tailPleft != &(game->planesleft[i])) && i < MAX_PLANES){
		if(game->planesleft[i].xposmin <= 0+game->planespeed){
			mustDespawn = 1;
			toremovePleft[removePleftcounter] = &(game->planesleft[i]);
			removePleftcounter++;		
		}
		else if(
			((((game->planesleft[i]).yposmax >= ufo->yposmax && (game->planesleft[i]).yposmin <= ufo->yposmax) ||
			((game->planesleft[i]).yposmax >= ufo->yposmin && (game->planesleft[i]).yposmax <= ufo->yposmax))
			&&
			(game->planesleft[i]).xposmin <= ufo->xposmax+(game->planespeed) &&
			(game->planesleft[i]).xposmax > ufo->xposmax))
		{
			mustDespawn = 1;
			planeHit = 1;
			/*printf("left moving Plane crashed into the UFO [%d] \n", i);*/
			toremovePleft[removePleftcounter] = &(game->planesleft[i]);
			removePleftcounter++;		
		}
		else{
			move_plane(&(game->planesleft[i]));
		}
		i++;
	}
	i = 0;
	while((game->tailPright != &(game->planesright[i])) && i < MAX_PLANES){
		if(game->planesright[i].xposmax >= 639-game->planespeed){
			mustDespawn = 1;
			toremovePright[removePrightcounter] = &(game->planesright[i]);
			removePrightcounter++;		
		}
		else if(
			((((game->planesright[i]).yposmax >= ufo->yposmax && (game->planesright[i]).yposmin <= ufo->yposmax) ||
			((game->planesright[i]).yposmax >= ufo->yposmin && (game->planesright[i]).yposmax <= ufo->yposmax))
			&&
			(game->planesright[i]).xposmax >= ufo->xposmin-(game->planespeed) &&
			(game->planesright[i]).xposmin < ufo->xposmin))
		{
			mustDespawn = 1;
			planeHit = 1;
			/*printf("right moving Plane crashed into the UFO [%d] \n", i);*/
			toremovePright[removePrightcounter] = &(game->planesright[i]);
			removePrightcounter++;		
		}
		else{
			move_plane(&(game->planesright[i]));
		}
		i++;
	}
	
	if(mustDespawn == 1){
		for(i = 0; i < removePleftcounter; i++)
			despawn_plane(game->planesleft[i], game->planesleft, &(game->tailPleft), map);		
		for(i = 0; i < removePrightcounter; i++)
			despawn_plane(game->planesright[i], game->planesright, &(game->tailPright), map);
		if(planeHit == 1)
			destroyUFO(game);
	}		
}

/*Checks a collision between a vertically moving ufo and a missile*/
int check_collision_ufoVSmissile_vert(int direction, MainGame *game, UFO *ufo){
	int missileHit = 0;
	int i = 0;
	
	while(missileHit == 0 && ((game->tailMleft) != &(game->missilesleft[i])) && i < MAX_MISSILES){
		if((ufo->xposmax <= (game->missilesleft[i]).xposmax && ufo->xposmax >= (game->missilesleft[i]).xposmin) ||
				(ufo->xposmin >= (game->missilesleft[i]).xposmin && ufo->xposmin <= (game->missilesleft[i]).xposmax)){
			if(((direction == ASCII_W || direction == ASCII_w) && 
				(ufo->yposmin <= ((game->missilesleft[i]).yposmax)+UFO_SPEED)) 
				||
				((direction == ASCII_S || direction == ASCII_s) &&
				(ufo->yposmax >= ((game->missilesleft[i]).yposmin)-UFO_SPEED)))	
			{
				/*printf("UFO crashed into left moving missile on a vertical move ufo request [%d] \n", i);*/
				missileHit = 1;
				despawn_missile(game->missilesleft[i], game->missilesleft, &(game->tailMleft));
				destroyUFO(game);
			}
		}
		i++;
	}
	i = 0;
	while(missileHit == 0 && ((game->tailMright) != &(game->missilesright[i])) && i < MAX_MISSILES){
		if((ufo->xposmax <= (game->missilesright[i]).xposmax && ufo->xposmax >= (game->missilesright[i]).xposmin) ||
				(ufo->xposmin >= (game->missilesright[i]).xposmin && ufo->xposmin <= (game->missilesright[i]).xposmax)){
			if(((direction == ASCII_W || direction == ASCII_w) && 
				(ufo->yposmin <= ((game->missilesright[i]).yposmax)+UFO_SPEED)) 
				||
				((direction == ASCII_S || direction == ASCII_s) &&
				(ufo->yposmax >= ((game->missilesright[i]).yposmin)-UFO_SPEED)))	
			{
				/*printf("UFO crashed into right moving missile on a vertical move ufo request [%d] \n", i);*/
				missileHit = 1;
				despawn_missile(game->missilesright[i], game->missilesright, &(game->tailMright));
				destroyUFO(game);	
			}
		}
	    i++;
	}
	return missileHit;
}

/*Checks a collision between a horizontally moving ufo and a missile*/
int check_collision_ufoVSmissile_horiz(int direction, MainGame *game, UFO *ufo){
	
	int missileHit = 0;
	int i = 0;

	while(missileHit == 0 && ((game->tailMleft) != &(game->missilesleft[i])) && i < MAX_MISSILES){
		if(((direction == ASCII_D || direction == ASCII_d) && 
			(((game->missilesleft[i]).yposmax >= ufo->yposmin && (game->missilesleft[i]).yposmax <= ufo->yposmax) ||
			((game->missilesleft[i]).yposmin >= ufo->yposmin && (game->missilesleft[i]).yposmin <= ufo->yposmax))
			&&
			(game->missilesleft[i]).xposmin <= ufo->xposmax+UFO_SPEED &&
			(game->missilesleft[i]).xposmax > ufo->xposmax)
			||
			((direction == ASCII_A || direction == ASCII_a) &&
			(game->missilesleft[i]).xposmax >= ufo->xposmin-UFO_SPEED &&
			(game->missilesleft[i]).xposmin < ufo->xposmin))
		{
			/*printf("UFO crashed into left moving missile horizontally on a move ufo request [%d] \n", i);*/
			missileHit = 1;
			despawn_missile(game->missilesleft[i], game->missilesleft, &(game->tailMleft));
		}
		i++;
	}	
	i=0;
	missileHit = 0;
	while(missileHit == 0 && (game->tailMright) != &(game->missilesright[i]) && i < MAX_MISSILES){
		if(((direction == ASCII_D || direction == ASCII_d) && 
			(((game->missilesright[i]).yposmax >= ufo->yposmin && (game->missilesright[i]).yposmax <= ufo->yposmax) ||
			((game->missilesright[i]).yposmin >= ufo->yposmin && (game->missilesright[i]).yposmin <= ufo->yposmax))
			&&
			(game->missilesright[i]).xposmin <= ufo->xposmax+UFO_SPEED &&
			(game->missilesright[i]).xposmax > ufo->xposmax)
			||
			((direction == ASCII_A || direction == ASCII_a) &&
			(game->missilesright[i]).xposmax >= ufo->xposmin-UFO_SPEED &&
			(game->missilesright[i]).xposmin < ufo->xposmin))
		{
			/*printf("UFO crashed into right moving missile horizontally on a move ufo request [%d] \n", i);*/
			missileHit = 1;
			despawn_missile(game->missilesright[i], game->missilesright, &(game->tailMright));
		}
		i++;
	}
	return missileHit;
}

/*Checks a collision between a vertically moving ufo and a plane*/
int check_collision_ufoVSplane_vert(int direction, MainGame *game, UFO *ufo, Map *map){
	int planeHit = 0;
	int i = 0;
	
	while(planeHit == 0 && ((game->tailPleft) != &(game->planesleft[i])) && i < MAX_PLANES){
		if((ufo->xposmax <= (game->planesleft[i]).xposmax && ufo->xposmax >= (game->planesleft[i]).xposmin) ||
				(ufo->xposmin >= (game->planesleft[i]).xposmin && ufo->xposmin <= (game->planesleft[i]).xposmax)){
			if(((direction == ASCII_W || direction == ASCII_w) && 
				(ufo->yposmin <= ((game->planesleft[i]).yposmax)+UFO_SPEED)) 
				||
				((direction == ASCII_S || direction == ASCII_s) &&
				(ufo->yposmax >= ((game->planesleft[i]).yposmin)-UFO_SPEED)))	
			{
				/*printf("UFO crashed into left moving plane on a vertical move ufo request [%d] \n", i);*/
				planeHit = 1;
				despawn_plane(game->planesleft[i], game->planesleft, &(game->tailPleft), map);
				destroyUFO(game);	
			}
		}		
		i++;
	}
	i = 0;
	while(planeHit == 0 && ((game->tailPright) != &(game->planesright[i])) && i < MAX_PLANES){
		if((ufo->xposmax <= (game->planesright[i]).xposmax && ufo->xposmax >= (game->planesright[i]).xposmin) ||
				(ufo->xposmin >= (game->planesright[i]).xposmin && ufo->xposmin <= (game->planesright[i]).xposmax)){
			if(((direction == ASCII_W || direction == ASCII_w) && 
				(ufo->yposmin <= ((game->planesright[i]).yposmax)+UFO_SPEED)) 
				||
				((direction == ASCII_S || direction == ASCII_s) &&
				(ufo->yposmax >= ((game->planesright[i]).yposmin)-UFO_SPEED)))	
			{
				/*printf("UFO crashed into right moving plane on a vertical move ufo request [%d] \n", i);*/
				planeHit = 1;
				despawn_plane(game->planesright[i], game->planesright, &(game->tailPright), map);
				destroyUFO(game);	
			}
		}
		i++;		
	}
	return planeHit;
}

/*Checks a collision between a horizontally moving ufo and a plane*/
int check_collision_ufoVSplane_horiz(int direction, MainGame *game, UFO *ufo, Map *map){
	int planeHit = 0;
	int i = 0;

	while(planeHit == 0 && ((game->tailPleft) != &(game->planesleft[i])) && i < MAX_PLANES){
		if(((direction == ASCII_D || direction == ASCII_d) && 
			(((game->planesleft[i]).yposmax >= ufo->yposmin && (game->planesleft[i]).yposmax <= ufo->yposmax) ||
			((game->planesleft[i]).yposmin >= ufo->yposmin && (game->planesleft[i]).yposmin <= ufo->yposmax))
			&&
			(game->planesleft[i]).xposmin <= ufo->xposmax+UFO_SPEED &&
			(game->planesleft[i]).xposmax > ufo->xposmax)
			||
			((direction == ASCII_A || direction == ASCII_a) &&
			(game->planesleft[i]).xposmax >= ufo->xposmin-UFO_SPEED &&
			(game->planesleft[i]).xposmin < ufo->xposmin))
		{
			/*printf("UFO crashed into left moving plane horizontally on a move ufo request [%d] \n", i);*/
			planeHit = 1;
			despawn_plane(game->planesleft[i], game->planesleft, &(game->tailPleft), map);
		}
		i++;
	}	
	i=0;
	planeHit = 0;
	while(planeHit == 0 && (game->tailPright) != &(game->planesright[i]) && i < MAX_PLANES){
		if(((direction == ASCII_D || direction == ASCII_d) && 
			(((game->planesright[i]).yposmax >= ufo->yposmin && (game->planesright[i]).yposmax <= ufo->yposmax) ||
			((game->planesright[i]).yposmin >= ufo->yposmin && (game->planesright[i]).yposmin <= ufo->yposmax))
			&&
			(game->planesright[i]).xposmin <= ufo->xposmax+UFO_SPEED &&
			(game->planesright[i]).xposmax > ufo->xposmax)
			||
			((direction == ASCII_A || direction == ASCII_a) &&
			(game->planesright[i]).xposmax >= ufo->xposmin-UFO_SPEED &&
			(game->planesright[i]).xposmin < ufo->xposmin))
		{
			/*printf("UFO crashed into right moving plane horizontally on a move ufo request [%d] \n", i);*/
			planeHit = 1;
			despawn_plane(game->planesright[i], game->planesright, &(game->tailPright), map);
		}
		i++;
	}
	return planeHit;
}

/*Checks a collision between a horizontally moving ufo and a soldier*/
int check_collision_ufoVSsoldier_horiz(int direction, MainGame *game, UFO *ufo){
	int soldierHit = 0;
	int i = 0;
	
	while(soldierHit == 0 && ((game->tailSleft) != &(game->soldiersleft[i])) && i < MAX_SOLDIERS){
		if(((direction == ASCII_D || direction == ASCII_d) && 
			(game->soldiersleft[i]).xposmin <= ufo->xposmax+UFO_SPEED &&
			(game->soldiersleft[i]).xposmax > ufo->xposmax)
			||
			((direction == ASCII_A || direction == ASCII_a) &&
			(game->soldiersleft[i]).xposmax >= ufo->xposmin-UFO_SPEED &&
			(game->soldiersleft[i]).xposmin < ufo->xposmin))
		{
			/*printf("UFO crashed into left moving soldier horizontally on a move ufo request [%d] \n", i);*/
			soldierHit = 1;
			despawn_soldier(game->soldiersleft[i], game->soldiersleft, &(game->tailSleft));
		}
		i++;
	}	
	i=0;
	soldierHit = 0;
	while(soldierHit == 0 && (game->tailSright) != &(game->soldiersright[i]) && i < MAX_SOLDIERS){
		if(((direction == ASCII_D || direction == ASCII_d) && 
			(game->soldiersright[i]).xposmin <= ufo->xposmax+UFO_SPEED &&
			(game->soldiersright[i]).xposmax > ufo->xposmax)
			||
			((direction == ASCII_A || direction == ASCII_a) &&
			(game->soldiersright[i]).xposmax >= ufo->xposmin-UFO_SPEED &&
			(game->soldiersright[i]).xposmin < ufo->xposmin))
		{
			/*printf("UFO crashed into right moving soldier horizontally on a move ufo request [%d] \n", i);*/
			soldierHit = 1;
			despawn_soldier(game->soldiersright[i], game->soldiersright, &(game->tailSright));
		}
		i++;
	}
	return 0;
}

/*Checks a collision between a vertically moving ufo and a soldier*/
int check_collision_ufoVSsoldier_vert(MainGame *game, UFO *ufo){
	int removeSleftcounter = 0;
	int removeSrightcounter = 0;
	int mustDespawn = 0;
	Soldier *toremoveSleft[MAX_SOLDIERS];
	Soldier *toremoveSright[MAX_SOLDIERS];
	int i = 0;

	while((game->tailSleft) != &(game->soldiersleft[i]) && i < MAX_SOLDIERS){
		if(((game->soldiersleft[i]).xposmin >= ufo->xposmin &&
			(game->soldiersleft[i]).xposmin <= ufo->xposmax)
			|| (game->soldiersleft[i]).xposmax <= ufo->xposmax &&
			(game->soldiersleft[i]).xposmax >= ufo->xposmin)
		{
			mustDespawn = 1;
			/*printf("UFO crashed into left moving soldier vertically [%d] \n", i);*/
			toremoveSleft[removeSleftcounter] = &(game->soldiersleft[i]);
			removeSleftcounter = removeSleftcounter + 1;
		}
		i++;
	}	
	i=0;
	while((game->tailSright) != &(game->soldiersright[i]) && i < MAX_SOLDIERS){
		if(((game->soldiersright[i]).xposmin >= ufo->xposmin &&
			(game->soldiersright[i]).xposmin <= ufo->xposmax)
			|| (game->soldiersright[i]).xposmax <= ufo->xposmax &&
			(game->soldiersright[i]).xposmax >= ufo->xposmin)
		{
			mustDespawn = 1;
			/*printf("UFO crashed into right moving soldier vertically [%d] \n", i);*/
			toremoveSright[removeSrightcounter] = &(game->soldiersright[i]);
			removeSrightcounter = removeSrightcounter + 1;
		}
		i++;
	}
	if(mustDespawn == 1){
		for(i = 0; i < removeSleftcounter; i++)
			despawn_soldier(*(toremoveSleft[i]), game->soldiersleft, &(game->tailSleft));		
		for(i = 0; i < removeSrightcounter; i++)
			despawn_soldier(*(toremoveSright[i]), game->soldiersright, &(game->tailSright));
	}
	return 0;
}

/*
First, if beamfiring is true, the soldiers do not move.

This function checks one by one, if the soldiers are at the edge of the map,
if they are, it despawns them (using the function in condition based events),
otherwise it moves them forward (with the function in model.c), checking
for collisions with the UFO (the only collision it cares about as soldier vs soldier
does nothing) and destroys the UFO and the colliding soldier despawns as well.
if it collides with the UFO, this returns true, indicating player has died in this step.
*/
void requestmovesoldiers(MainGame *game, UFO *ufo){
	int i = 0;
	int mustDespawn = 0;
	Soldier *toremoveSleft[MAX_SOLDIERS];
	Soldier *toremoveSright[MAX_SOLDIERS];
	int removeSleftcounter = 0;
	int removeSrightcounter = 0;
	
	if(game->beamfiring == FALSE){		
		while((game->tailSleft != &(game->soldiersleft[i])) && i < MAX_SOLDIERS){
			if(game->soldiersleft[i].xposmin <= 0+SOLDIERSPEED){
				mustDespawn = 1;
				toremoveSleft[removeSleftcounter] = &(game->soldiersleft[i]);
				removeSleftcounter++;		
			}
			else if(ufo->yposmax >= GROUND-SOLDIER_HEIGHT &&
					(game->soldiersleft[i]).xposmin <= ufo->xposmax+SOLDIERSPEED &&
					(game->soldiersleft[i]).xposmax > ufo->xposmax)
			{
				mustDespawn = 1;
				/*printf("left moving Soldier hit the UFO on move soldier request [%d] \n", i);*/
				toremoveSleft[removeSleftcounter] = &(game->soldiersleft[i]);
				removeSleftcounter++;		
			}
			else{
				move_soldier(&(game->soldiersleft[i]));
			}
			i++;
		}
		i = 0;
		while((game->tailSright != &(game->soldiersright[i])) && i < MAX_SOLDIERS){
			if(game->soldiersright[i].xposmax >= 639-SOLDIERSPEED){
				/*printf("Right moving soldier hit the wall");*/
				mustDespawn =1;
				toremoveSright[removeSrightcounter] = &(game->soldiersright[i]);
				removeSrightcounter++;		
			}
			else if(ufo->yposmax >= GROUND-SOLDIER_HEIGHT &&
					(game->soldiersright[i]).xposmax >= ufo->xposmin-SOLDIERSPEED &&
					(game->soldiersright[i]).xposmin < ufo->xposmin)
			{
				mustDespawn = 1;
				/*printf("right moving soldier hit the UFO on move soldier request [%d] \n", i);*/
				toremoveSright[removeSrightcounter] = &(game->soldiersright[i]);
				removeSrightcounter++;		
			}
			else{
				move_soldier(&(game->soldiersright[i]));
			}
			i = i+1;
		}
	
		if(mustDespawn == 1){
			for(i = 0; i < removeSleftcounter; i++)
				despawn_soldier(game->soldiersleft[i], game->soldiersleft, &(game->tailSleft));		
			for(i = 0; i < removeSrightcounter; i++)
				despawn_soldier(game->soldiersright[i], game->soldiersright, &(game->tailSright));
		}	
	}		
}

/*
This function checks one by one, if the missiles are at the edge of the map,
if they are, it despawns them, otherwise it moves them forward (with the function in model.c), checking
for collisions with the UFO, and destroys the UFO and the colliding missile despawns as well.
if it collides with the UFO, this returns true, indicating player has died in this step.
*/
void requestmovemissiles(MainGame *game, UFO *ufo){
	int i = 0;
	int	mustDespawn = 0;
	int missileHit = 0;
	Missile *toremoveMleft[MAX_MISSILES];
	Missile *toremoveMright[MAX_MISSILES];
	int removeMleftcounter = 0;
	int removeMrightcounter = 0;
	
	while((game->tailMleft != &(game->missilesleft[i])) && i < MAX_MISSILES){
		if(game->missilesleft[i].xposmin <= 0+(game->missilesleft[i].speed)){
			mustDespawn = 1;
			toremoveMleft[removeMleftcounter] = &(game->missilesleft[i]);
			removeMleftcounter++;		
		}
		else if(
			((((game->missilesleft[i]).yposmin <= ufo->yposmax && (game->missilesleft[i]).yposmin >= ufo->yposmin) ||
			((game->missilesleft[i]).yposmax >= ufo->yposmin && (game->missilesleft[i]).yposmax <= ufo->yposmax))
			&&
			(game->missilesleft[i]).xposmin <= ufo->xposmax+(game->missilesleft[i].speed) &&
			(game->missilesleft[i]).xposmax > ufo->xposmax))
		{
			missileHit = 1;
			mustDespawn = 1;
			/*printf("left moving Missile hit the UFO on move missile request [%d] \n", i);*/
			toremoveMleft[removeMleftcounter] = &(game->missilesleft[i]);
			removeMleftcounter++;		
		}
		else{
			move_missile(&(game->missilesleft[i]));
		}
		i++;
	}
	i = 0;
	while((game->tailMright != &(game->missilesright[i])) && i < MAX_MISSILES){
		if(game->missilesright[i].xposmax >= 639-(game->missilesright[i].speed)){
			mustDespawn = 1;
			toremoveMright[removeMrightcounter] = &(game->missilesright[i]);
			removeMrightcounter++;		
		}
		else if(
			((((game->missilesright[i]).yposmin <= ufo->yposmax && (game->missilesright[i]).yposmin >= ufo->yposmin) ||
			((game->missilesright[i]).yposmax >= ufo->yposmin && (game->missilesright[i]).yposmax <= ufo->yposmax))
			&&
			(game->missilesright[i]).xposmax >= ufo->xposmin-(game->missilesright[i].speed) &&
			(game->missilesright[i]).xposmin < ufo->xposmin))
		{
			missileHit = 1;
			mustDespawn = 1;
			/*printf("right moving missile hit the UFO on move missile request [%d] \n", i);*/
			toremoveMright[removeMrightcounter] = &(game->missilesright[i]);
			removeMrightcounter++;		
		}
		else{
			move_missile(&(game->missilesright[i]));
		}
		i++;
	}
	
	if(mustDespawn  == 1){
		for(i = 0; i < removeMleftcounter; i++)
			despawn_missile(game->missilesleft[i], game->missilesleft, &(game->tailMleft));		
		for(i = 0; i < removeMrightcounter; i++)
			despawn_missile(game->missilesright[i], game->missilesright, &(game->tailMright));
		if(missileHit == 1)
			destroyUFO(game);
	}		
}

/* This function will check if the beam is ready to be fired
on the UFO, if yes, it will call firebeam(), and then call
incrementscore() on the UFO, passing in what firebeam() returned
as the score, no need to check collision, the beam will pass through
everything and will only collide with soldiers, which is handled in firebeam()
*/
int requestfirebeam(MainGame *game, UFO *ufo, Soldier left[], Soldier right[], Soldier* *lefttail, Soldier* *righttail){
	int i=0;
	int result = FALSE;
	if(ufo->yposmin <= MIN_FIRE_BEAM_HEIGHT){
		if(ufo->BeamReady){
			/*firebeam(game, ufo, left, right, lefttail, righttail);
			brady, when you uncomment firebeam here, comment game->beamfiring = TRUE; out */
			game->beamfiring = TRUE;
			ufo->BeamReady = FALSE;
			result = TRUE;
		}
	}
	return result;
}

/*
Performs a calculation based on score (TBD) and updates both values based
on result.
*/
void updateplanespeed(int score, MainGame *game){
	if(score >= 1000)
		game->planespeed = 12;
	else if(score >= 500)
		game->planespeed = 11;
	else if(score >= 200)
		game->planespeed = 10;
	else if(score >= 100)
		game->planespeed = 8;
}

/*
This function must set beamfiring to true, and detect any collisions with soldiers, updating the
score in the UFO object based on how many soldiers were abducted. This function must
then call updateplanespeed with the new score.
*/

void firebeam(MainGame *game, UFO *ufo, Soldier left[], Soldier right[], Soldier* *lefttail, Soldier* *righttail){
	int i=0;
	int removeleftcounter = 0;
	int removerightcounter = 0;
	int	mustDespawn = 0;
	Soldier *toremoveleft[MAX_SOLDIERS];
	Soldier *toremoveright[MAX_SOLDIERS];
	
	game->beamfiring = TRUE;
	while(*(lefttail) != &(left[i]) && i < MAX_SOLDIERS){
		if(left[i].xposmin >= ufo->xposmin 
			&& left[i].xposmax <= ufo->xposmax){
			mustDespawn = 1;
			/*printf("left[%d] hit \n", i);*/
			toremoveleft[removeleftcounter] = &(left[i]);
			removeleftcounter = removeleftcounter + 1;
			ufo->score = ufo->score + POINTS_PER_SOLDIER;
		}
		i++;
	}
	i=0;
	while(*(righttail) != &(right[i]) && i < MAX_SOLDIERS){
		if(right[i].xposmin >= ufo->xposmin 
			&& right[i].xposmax <= ufo->xposmax){
			mustDespawn = 1;
			/*printf("right[%d] hit \n", i);*/
			toremoveright[removerightcounter] = &(right[i]);
			removerightcounter = removerightcounter + 1;
			ufo->score = ufo->score + POINTS_PER_SOLDIER;
		}
		i++;
	}
	if(mustDespawn == 1){
		for(i = 0; i < removeleftcounter; i++)
				despawn_soldier(*(toremoveleft[i]), left, &(game->tailSleft));
		
		for(i = 0; i < removerightcounter; i++)
				despawn_soldier(*(toremoveright[i]), right, &(game->tailSright));

		updateplanespeed(ufo->score, game);
	}
}

/*
This function must set beamfiring to false
(timing handled by main)
DONE
*/
void stopfiring(MainGame *game){
	game->beamfiring = FALSE;
	game->clearbeam = TRUE;
}