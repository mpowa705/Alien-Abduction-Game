#include"events.h"
#include"model.h"
#include"effects.h"
#include"psg.h"
#include<osbind.h>
#include<stdio.h>

/*Team: Matthew Powaschuk, Brady Kirchner
COMP 2659 - Winter 2016
Prof: Paul Pospisil
Project: "Invader's revenge"
*/

/*
Purpose: This sets the model to it's initial state.

Details: This just sets all values to what they should be for the first frame.

Input: This takes the model by reference.
*/
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
	game->player.xposmin = UFO_MINX;
	game->player.xposmax = UFO_MINX+31;
	game->player.xposold = UFO_MINX;
	game->player.xposold2 = UFO_MINX;
	game->player.yposold = 176;
	game->player.yposold2 = 176;
	game->player.BeamReady = TRUE;
	game->planespeed = DEFAULTPLANESPEED;       
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
		/*y values that planes are occupying are set to true as planes spawn*/
		i++;
	}

	return;
}

/*
Purpose: This stores the result of the last frame in the "snapshot" variables (old and old2)

Details: This goes through all moving entities and pointers to those entities' tails and saves their
current state, and their states from 1 frame ago.

Input: This takes the model by reference.
*/
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
/*
Function Name: requestmoveufo
Purpose: 
If the UFO's beam is firing, the UFO does not move. Otherwise, this function will check whether a collision would 
occur on a given move_ufo instruction. The function calls subroutines to check for collisions against the
screen boundaries, the ground, the soldiers, and the planes. If a fatal collsion against the ground occurs,
the UFO is 'killed' and destroyUFO is called. If none of these checks result in a collision, the ufo will
move in the designated direction by calling the move_ufo function.

ASSUMPTIONS: 

- At maximum, one left moving + 1 right moving soldier may be collided with on a horizontal move UFO request.

Input Parameters:
int direction: ascii value of a direction input previously by the user
MainGame *game: pointer to the MainGame object containing information about the game world
UFO *ufo: pointer to the user's UFO object
Map *map: pointer to an instance of type Map which contains information on the coordinates of the planes

Returns: void

*/
void requestmoveufo(unsigned char direction, MainGame *game, UFO *ufo, Map *map){
	int invalidMove = 0; /*Bool*/
	int removeSleftcounter = 0;
	int removeSrightcounter = 0;
	int i=0;
	int soldierHit = 0; /*Bool*/
		
	if(!(game->beamfiring)){
	
		if(direction == ASCII_w){
			if(!(ufo->yposmin < (BOUNDARY_TOP+UFO_SPEED))){ /*Checks out-of-bounds*/
				invalidMove = check_collision_ufoVSplane_vert(direction, game, ufo, map); 
									/*Check UFO up vs plane collision*/
			}
			else
			{
				invalidMove = 1;
				ufo->yposmin = 0;
				ufo->yposmax = 31;
			}		
		}				

		else if(direction == ASCII_a){
			if(!(ufo->xposmin < (BOUNDARY_LEFT+UFO_SPEED))){ /*Checks out-of-bounds*/	
				invalidMove = check_collision_ufoVSplane_horiz(direction, game, ufo, map);
									/*Check UFO up vs plane collision*/

				if(invalidMove == 0 && ufo->yposmax >= GROUND-SOLDIER_HEIGHT){ 	/*Check UFO vs Soldier horizontal left collision*/
																				/*Only check if UFO at or below soldier height*/
					check_collision_ufoVSsoldier_horiz(direction, game, ufo);
				}
			}
			else
			{
				invalidMove = 1;
				ufo->xposmin = 1;
				ufo->xposmax = 32;
			}					
		}
	
		else if(direction == ASCII_d){
			if(!(ufo->xposmax > (BOUNDARY_RIGHT-UFO_SPEED))){ /*Checks out-of-bounds*/
				invalidMove = check_collision_ufoVSplane_horiz(direction, game, ufo, map);
									/*Check UFO up vs plane collision*/
				if(invalidMove == 0 && ufo->yposmax >= GROUND-SOLDIER_HEIGHT){ 	/*Check UFO vs Soldier horizontal right collision*/
																				/*Only check if UFO at or below soldier height*/
					check_collision_ufoVSsoldier_horiz(direction, game, ufo);
				}
			}
			else
			{
				invalidMove = 1;
				ufo->xposmax = 638;
				ufo->xposmin = 607;
			}	
		}

		else if(direction == ASCII_s){
			if(ufo->yposmax >= (GROUND-UFO_SPEED)){
				destroyUFO(game);
				/*UFO CRASHED INTO THE GROUND*/
				invalidMove = 1;
			}
			else{
				invalidMove = check_collision_ufoVSplane_vert(direction, game, ufo, map);
									/*Check UFO up vs plane collision*/
/*
				if(invalidMove == 0)
					invalidMove = check_collision_ufoVSmissile_vert(direction, game, ufo);
*/
									/*Check UFO up vs missile collision*/						
				if((invalidMove == 0 && ufo->yposmax >= GROUND-SOLDIER_HEIGHT-UFO_SPEED) &&		/*Check UFO vs Soldier vertical collision*/
						(ufo->yposmax < GROUND-SOLDIER_HEIGHT)){ 			/*Only check if UFO just above soldier height*/														
					check_collision_ufoVSsoldier_vert(game, ufo);
				}
			}				 
		}
		
		if(invalidMove == 0)
			move_ufo(direction, ufo);
	}
}

/*
Function Name: requestmoveplanes
Purpose: 
This function checks whether collisions would occur between any of the planes on the map (stored in 'left' and
'right' arrays) and the player's UFO object, based on the current coordinates of the planes/UFO, and the 
current speed of the planes. Plane speed is given as "pixels per move" and stored in an instance of MainGame.
If a plane would collide with the UFO, this function returns true, indicating player has died in this step, and
destroyUFO is called. If a plane is about to collide with the ednge of the screen, the plane is removed from the map
during the call to despawn_plane.

Input Parameters:
MainGame *game: pointer to the MainGame object containing information about the game world
UFO *ufo: pointer to the user's UFO object
Map *map: pointer to an instance of type Map which contains information on the coordinates of the planes

Returns: a zero if no collisions would occur, or a 1 if collisions are detected
*/
int requestmoveplanes(MainGame *game, UFO *ufo, Map *map){
	int i = 0;
	int despawned = FALSE;
	
	while((game->tailPleft != &(game->planesleft[i])) && i < MAX_PLANES){
		if(game->planesleft[i].xposmin <= 0+game->planespeed){ /*checks if plane would hit the edge of the screen*/
			despawn_plane(game->planesleft[i], game->planesleft, &(game->tailPleft), map);
			despawned = TRUE;
			i--;
		}
		else if(
			((((game->planesleft[i]).yposmax >= ufo->yposmax && (game->planesleft[i]).yposmin <= ufo->yposmax) ||
			((game->planesleft[i]).yposmax >= ufo->yposmin && (game->planesleft[i]).yposmax <= ufo->yposmax))
			&&
			(game->planesleft[i]).xposmin <= ufo->xposmax+(game->planespeed) &&
			(game->planesleft[i]).xposmax > ufo->xposmax))         /*checks if plane would hit the UFO*/
		{

			destroyUFO(game);
		}
		else{
			move_plane(&(game->planesleft[i]));
		}
		i++;
	}
	i = 0;
	while((game->tailPright != &(game->planesright[i])) && i < MAX_PLANES){
		if(game->planesright[i].xposmax >= 639-game->planespeed){ /*checks if plane would hit the edge of the screen*/
			despawn_plane(game->planesright[i], game->planesright, &(game->tailPright), map);
			despawned = TRUE;
			i--;
		}
		else if(
			((((game->planesright[i]).yposmax >= ufo->yposmax && (game->planesright[i]).yposmin <= ufo->yposmax) ||
			((game->planesright[i]).yposmax >= ufo->yposmin && (game->planesright[i]).yposmax <= ufo->yposmax))
			&&
			(game->planesright[i]).xposmax >= ufo->xposmin-(game->planespeed) &&
			(game->planesright[i]).xposmin < ufo->xposmin))       /*checks if plane would hit the UFO*/
		{
			destroyUFO(game);
		}
		else{
			move_plane(&(game->planesright[i]));
		}
		i++;
	}
	return despawned;
}
/*
Function Name: check_collision_ufoVSplane_vert
Purpose: 
This function checks whether collisions would occur between a vertically moving UFO and each of the planes on the map (stored in 'left' and
'right' arrays) based on the current coordinates of the planes/UFO, and the speed of the UFO (given as "pixels per move").
If the UFO would collide with a plane, this function returns true, indicating player has died in this step, and
destroyUFO is called.

Input Parameters:
int direction: ascii value of a direction input previously by the user
MainGame *game: pointer to the MainGame object containing information about the game world
UFO *ufo: pointer to the user's UFO object
Map *map: pointer to an instance of type Map which contains information on the coordinates of the planes

Returns: a zero if no collisions would occur, or a 1 if collisions are detected
*/

int check_collision_ufoVSplane_vert(int direction, MainGame *game, UFO *ufo, Map *map){
	int planeHit = 0;
	int i = 0;
	
	while(((game->tailPleft) != &(game->planesleft[i])) && i < MAX_PLANES){
		if((ufo->xposmax <= (game->planesleft[i]).xposmax && ufo->xposmax >= (game->planesleft[i]).xposmin) ||
				(ufo->xposmin >= (game->planesleft[i]).xposmin && ufo->xposmin <= (game->planesleft[i]).xposmax)){
					/*checks alignment of the UFO with a plane's max and min x-coordinates*/
			if(((direction == ASCII_W || direction == ASCII_w) && 
				(ufo->yposmin <= ((game->planesleft[i]).yposmax)+UFO_SPEED) && ufo->yposmax > (game->planesleft[i]).yposmax
					&& ufo->yposmin > (game->planesleft[i]).yposmax) 
				||
				((direction == ASCII_S || direction == ASCII_s) &&
				(ufo->yposmax >= ((game->planesleft[i]).yposmin)-UFO_SPEED) && ufo->yposmin < (game->planesleft[i]).yposmin
					&& ufo->yposmin < (game->planesleft[i]).xposmin))	
					/*checks if an upward or downward moving UFO would collide with the plane*/
			{
				planeHit = 1;
				destroyUFO(game);
			}
		}		
		i++;
	}
	i = 0;
	while(((game->tailPright) != &(game->planesright[i])) && i < MAX_PLANES){
		if((ufo->xposmax <= (game->planesright[i]).xposmax && ufo->xposmax >= (game->planesright[i]).xposmin) ||
				(ufo->xposmin >= (game->planesright[i]).xposmin && ufo->xposmin <= (game->planesright[i]).xposmax)){
					/*checks alignment of the UFO with a plane's max and min x-coordinates*/
			if(
				((direction == ASCII_W || direction == ASCII_w) && 
				(ufo->yposmin <= ((game->planesright[i]).yposmax)+UFO_SPEED) && ufo->yposmax > (game->planesright[i]).yposmax
					&& ufo->yposmin > (game->planesright[i]).yposmax) 
				||
				((direction == ASCII_S || direction == ASCII_s) &&
				(ufo->yposmax >= ((game->planesright[i]).yposmin)-UFO_SPEED) && ufo->yposmin < (game->planesright[i]).yposmin
					&& ufo->yposmin < (game->planesright[i]).xposmin))
					/*checks if an upward or downward moving UFO would collide with the plane*/					
			{
				planeHit = 1;
				destroyUFO(game);
			}
		}
		i++;		
	}
	return planeHit;
}
/*
Function Name: check_collision_ufoVSplane_horiz
Purpose: 
This function checks whether collisions would occur between a horizontally moving UFO and each of the planes on the map (stored in 'left' and
'right' arrays) based on the current coordinates of the planes/UFO, and the speed of the UFO (given as "pixels per move").
If the UFO would collide with a plane, this function returns true, indicating player has died in this step, and
destroyUFO is called.

Input Parameters:
int direction: ascii value of a direction input previously by the user
MainGame *game: pointer to the MainGame object containing information about the game world
UFO *ufo: pointer to the user's UFO object
Map *map: pointer to an instance of type Map which contains information on the coordinates of the planes

Returns: a zero if no collisions would occur, or a 1 if collisions are detected
*/
int check_collision_ufoVSplane_horiz(int direction, MainGame *game, UFO *ufo, Map *map){
	int planeHit = 0;
	int i = 0;

	while(planeHit == 0 && ((game->tailPleft) != &(game->planesleft[i])) && i < MAX_PLANES){
		if(((game->planesleft[i]).yposmax >= ufo->yposmin && (game->planesleft[i]).yposmax <= ufo->yposmax) ||
			((game->planesleft[i]).yposmin >= ufo->yposmin && (game->planesleft[i]).yposmin <= ufo->yposmax))
			/*checks alignment of the UFO with a plane's max and min y-coordinates*/
		{
			if(((direction == ASCII_D || direction == ASCII_d)
			&&
			(game->planesleft[i]).xposmin <= ufo->xposmax+UFO_SPEED &&
			(game->planesleft[i]).xposmax > ufo->xposmax)
			||
			((direction == ASCII_A || direction == ASCII_a) 
			&&
			(game->planesleft[i]).xposmax >= ufo->xposmin-UFO_SPEED &&
			(game->planesleft[i]).xposmin < ufo->xposmin))
			/*checks if the UFO would collide with a left moving plane*/	
			{
				planeHit = 1;
				destroyUFO(game);
			}
		}
		i++;
	}	
	i=0;
	while(planeHit == 0 && (game->tailPright) != &(game->planesright[i]) && i < MAX_PLANES){
		if(((game->planesright[i]).yposmax >= ufo->yposmin && (game->planesright[i]).yposmax <= ufo->yposmax) ||
			((game->planesright[i]).yposmin >= ufo->yposmin && (game->planesright[i]).yposmin <= ufo->yposmax))
			/*checks alignment of the UFO with a plane's max and min y-coordinates*/
		{
			if(((direction == ASCII_D || direction == ASCII_d) 
			&&
			(game->planesright[i]).xposmin <= ufo->xposmax+UFO_SPEED &&
			(game->planesright[i]).xposmax > ufo->xposmax)
			||
			((direction == ASCII_A || direction == ASCII_a) 
			&&
			(game->planesright[i]).xposmax >= ufo->xposmin-UFO_SPEED &&
			(game->planesright[i]).xposmin < ufo->xposmin))
			/*checks if the UFO would collide with a right moving plane*/
			{
				planeHit = 1;
				destroyUFO(game);
			}
		}
		i++;
	}
	return planeHit;
}
/*
Function Name: check_collision_ufoVSsoldier_horiz
Purpose: 
This function checks whether collisions would occur between a horizontally moving UFO and each of the soldiers on the map (stored in 'left' and
'right' arrays) based on the current coordinates of the soldiers/UFO, and the speed of the UFO (given as "pixels per move").
If the UFO would collide with a soldier, the soldier is destroyed, but no points are awarded.

Input Parameters:
int direction: ascii value of a direction input previously by the user
MainGame *game: pointer to the MainGame object containing information about the game world
UFO *ufo: pointer to the user's UFO object

Returns: void
*/
void check_collision_ufoVSsoldier_horiz(int direction, MainGame *game, UFO *ufo){
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
					/*checks if the UFO would collide with a left moving soldier*/
		{
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
					/*checks if the UFO would collide with a right moving soldier*/
		{
			soldierHit = 1;
			despawn_soldier(game->soldiersright[i], game->soldiersright, &(game->tailSright));
		}
		i++;
	}
}

/*
Function Name: check_collision_ufoVSsoldier_vert
Purpose: 
This function checks whether collisions would occur between a vertically moving UFO and each of the soldiers on the map (stored in 'left' and
'right' arrays) based on the current coordinates of the soldiers/UFO, and the speed of the UFO (given as "pixels per move").
If the UFO would collide with a soldier, the soldier is destroyed, but no points are awarded.

Input Parameters:
MainGame *game: pointer to the MainGame object containing information about the game world
UFO *ufo: pointer to the user's UFO object

Returns: void
*/
void check_collision_ufoVSsoldier_vert(MainGame *game, UFO *ufo){
	int i = 0;

	while((game->tailSleft) != &(game->soldiersleft[i]) && i < MAX_SOLDIERS){
		if(((game->soldiersleft[i]).xposmin >= ufo->xposmin &&
			(game->soldiersleft[i]).xposmin <= ufo->xposmax)
			|| (game->soldiersleft[i]).xposmax <= ufo->xposmax &&
			(game->soldiersleft[i]).xposmax >= ufo->xposmin)
								/*checks if the UFO would collide with a left moving soldier*/
		{
			despawn_soldier(game->soldiersleft[i], game->soldiersleft, &(game->tailSleft));
		}
		i++;
	}	
	i=0;
	while((game->tailSright) != &(game->soldiersright[i]) && i < MAX_SOLDIERS){
		if(((game->soldiersright[i]).xposmin >= ufo->xposmin &&
			(game->soldiersright[i]).xposmin <= ufo->xposmax)
			|| (game->soldiersright[i]).xposmax <= ufo->xposmax &&
			(game->soldiersright[i]).xposmax >= ufo->xposmin)
								/*checks if the UFO would collide with a right moving soldier*/
		{
			despawn_soldier(game->soldiersright[i], game->soldiersright, &(game->tailSright));
		}
		i++;
	}
}

/*

/*
Function Name: requestmovesoldiers
Purpose: 
This function checks whether collisions would occur between any of the soldiers on the map (stored in 'left' and
'right' arrays) and the player's UFO object, based on the current coordinates of the soldiers/UFO, and the 
speed of the soldiers. If a soldier is about to collide with the ednge of the screen, the soldier is removed from the map
during the call to despawn_soldier. If the UFO's beam is firing, none of the soldiers may move until the beam is finished firing.
If a soldier would collide with the UFO, this function returns a 1 indicating collisions detected.

Input Parameters:
MainGame *game: pointer to the MainGame object containing information about the game world
UFO *ufo: pointer to the user's UFO object

Returns: a zero if no collisions would occur, or a 1 if collisions are detected
*/
int requestmovesoldiers(MainGame *game, UFO *ufo){
	int i = 0;
	int mustDespawn = FALSE;
	
	if(game->beamfiring == FALSE){		
		while((game->tailSleft != &(game->soldiersleft[i])) && i < MAX_SOLDIERS){
			if(game->soldiersleft[i].xposmin <= 0+SOLDIERSPEED){
				despawn_soldier(game->soldiersleft[i], game->soldiersleft, &(game->tailSleft));
				mustDespawn = TRUE;
				i--;
			}
			else if(ufo->yposmax >= GROUND-SOLDIER_HEIGHT &&
					(game->soldiersleft[i]).xposmin <= ufo->xposmax+SOLDIERSPEED &&
					(game->soldiersleft[i]).xposmax > ufo->xposmax)
			{
				despawn_soldier(game->soldiersleft[i], game->soldiersleft, &(game->tailSleft));
				i--;
			}
			else{
				move_soldier(&(game->soldiersleft[i]));
			}
			i++;
		}
		i = 0;
		while((game->tailSright != &(game->soldiersright[i])) && i < MAX_SOLDIERS){
			if(game->soldiersright[i].xposmax >= 639-SOLDIERSPEED){
				despawn_soldier(game->soldiersright[i], game->soldiersright, &(game->tailSright));
				mustDespawn = TRUE;
				i--;
			}
			else if(ufo->yposmax >= GROUND-SOLDIER_HEIGHT &&
					(game->soldiersright[i]).xposmax >= ufo->xposmin-SOLDIERSPEED &&
					(game->soldiersright[i]).xposmin < ufo->xposmin)
			{
				despawn_soldier(game->soldiersright[i], game->soldiersright, &(game->tailSright));
				i--;
			}
			else{
				move_soldier(&(game->soldiersright[i]));
			}
			i = i+1;
		}
	}		
	return mustDespawn;
}

/*
Function Name: requestfirebeam
Purpose: 
This function checks if the beam is ready to be fired from the UFO. If yes, firebeam is called to check for
collisions with soldiers and to update the user's score.

Input Parameters:
MainGame *game: pointer to the MainGame object containing information about the game world
UFO *ufo: pointer to the user's UFO object
Soldier left[]: array of left moving soldiers
Soldier right[]: array of right moving soldiers
Soldier* *lefttail: tail pointer to array of left moving soldiers
Soldier* *righttail: tail pointer to array of right moving soldiers

Returns: 1 (or TRUE) if the beam was successfully fired.
*/
int requestfirebeam(MainGame *game, UFO *ufo, Soldier left[], Soldier right[], Soldier* *lefttail, Soldier* *righttail){
	int i=0;
	int result = FALSE;
	if(ufo->yposmin <= MIN_FIRE_BEAM_HEIGHT){
		if(ufo->BeamReady){
			firebeam(game, ufo, left, right, lefttail, righttail);
			ufo->BeamReady = FALSE;
			result = TRUE;
		}
	}
	return result;
}

/*
Purpose: Updates the planespeed based on the current score.

Details: This picks a speed based on score at increasing intervals.
*/
void updateplanespeed(int score, MainGame *game){
	if(score >= 1000)
		game->planespeed = 10;
	else if(score >= 500)
		game->planespeed = 8;
	else if(score >= 200)
		game->planespeed = 6;
	else if(score >= 100)
		game->planespeed = 5;
}

/*
Function Name: firebeam
Purpose: 
This function will set beamfiring to true, and detect any collisions between the beam and soldiers, 
updating the score in the UFO object based on how many soldiers were abducted. This function will
then call updateplanespeed with the new score, in order to determine whether the planes' speed should
be increased based on the player's current score.

Input Parameters:
MainGame *game: pointer to the MainGame object containing information about the game world
UFO *ufo: pointer to the user's UFO object
Soldier left[]: array of left moving soldiers
Soldier right[]: array of right moving soldiers
Soldier* *lefttail: tail pointer to array of left moving soldiers
Soldier* *righttail: tail pointer to array of right moving soldiers

Returns: 1 (or TRUE) if the beam was successfully fired.
*/
void firebeam(MainGame *game, UFO *ufo, Soldier left[], Soldier right[], Soldier* *lefttail, Soldier* *righttail){
	int i=0;
	int	mustDespawn = 0;
	
    play_effect_beam();
	game->beamfiring = TRUE;
	while(*(lefttail) != &(left[i]) && i < MAX_SOLDIERS){
		if(left[i].xposmin >= ufo->xposmin - 4
			&& left[i].xposmax <= ufo->xposmax + 4){
			mustDespawn = 1;
			despawn_soldier(game->soldiersleft[i], game->soldiersleft, &(game->tailSleft)); 
			i--;
			ufo->score = ufo->score + POINTS_PER_SOLDIER;
		}
		i++;
	}
	i=0;
	while(*(righttail) != &(right[i]) && i < MAX_SOLDIERS){
		if(right[i].xposmin >= ufo->xposmin - 4
			&& right[i].xposmax <= ufo->xposmax + 4){
			mustDespawn = 1;
			despawn_soldier(game->soldiersright[i], game->soldiersright, &(game->tailSright));
			i--;
			ufo->score = ufo->score + POINTS_PER_SOLDIER;
		}
		i++;
	}
	
	if(mustDespawn == 1)
		updateplanespeed(ufo->score, game);
}

void stopfiring(MainGame *game){
	long old_ssp = Super(0);
    enable_channel(CHANNEL_B, OFF, OFF);        /* stops sound effect */
    set_volume(CHANNEL_B, 0);
	game->beamfiring = FALSE;
	game->clearbeam = TRUE;
	Super(old_ssp);
}