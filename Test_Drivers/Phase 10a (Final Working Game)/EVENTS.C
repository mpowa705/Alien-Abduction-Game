/*Team: Matthew Powaschuk, Brady Kirchner
COMP 2659 - Winter 2016
Prof: Paul Pospisil
Project: "Alien Abduction"
Details: Spawning and despawning functions handled by Matt, move functions handled by Brady.
find_acceptable_y was a team effort.

*/

#include"input.h"
#include"events.h"
#include"model.h"
#include"effects.h"
#include"globals.h"
#include<stdlib.h>
#include<osbind.h>
#include<time.h>

/***************** ASYNCHRONOUS EVENTS *****************/

/*
Function Name: move_plane

Purpose: Moves an instance of a Plane object from its current x-coordinates to new x-coordinates
         based on current maximum and minumum x-coordinates and the value of its speed field.

Input Parameters: pointer to a Plane to be moved

*/
void move_plane(Plane *plane){

	if(plane->lefttoright == 1){
		plane->xposmax = plane->xposmax + plane->speed;
		plane->xposmin = plane->xposmin + plane->speed;
	}
	else if(plane->lefttoright == 0){
		plane->xposmax = plane->xposmax - plane->speed;
		plane->xposmin = plane->xposmin - plane->speed;
	}
}

/*
Function Name: move_soldier

Purpose: Moves an instance of a Soldier object from its current x-coordinates to new x-coordinates
         based on current maximum and minumum x-coordinates and the value of the SOLDIER_SPEED constant.

Input Parameters: pointer to a Soldier to be moved

*/
void move_soldier(Soldier *soldier){

	if(soldier->lefttoright == 1){
		soldier->xposmax = soldier->xposmax + SOLDIER_SPEED;
		soldier->xposmin = soldier->xposmin + SOLDIER_SPEED;
	}
	else if(soldier->lefttoright == 0){
		soldier->xposmax = soldier->xposmax - SOLDIER_SPEED;
		soldier->xposmin = soldier->xposmin - SOLDIER_SPEED;
	}
}

/*
Function Name: move_ufo

Purpose: Moves an instance of the UFO object from its current x/y-coordinates to new x/y-coordinates
         based on its current maximum and minumum x/y-coordinates and the value of the UFO_SPEED constant,
		 as well as the indicated direction.

Input Parameters: 
UFO *ufo: pointer to the UFO object
int direction: direction recorded previously as an ascii value given by the user

*/
void move_ufo(UINT8 direction, UFO *ufo){

	if(direction == ASCII_w)
	{
		ufo->yposmax = ufo->yposmax - UFO_SPEED;
		ufo->yposmin = ufo->yposmin - UFO_SPEED;		 
	}

	if(direction == ASCII_s)
	{
		ufo->yposmax = ufo->yposmax + UFO_SPEED;
		ufo->yposmin = ufo->yposmin + UFO_SPEED;
	}
	
	if(direction == ASCII_a)
	{
		ufo->xposmin = ufo->xposmin - UFO_SPEED;
		ufo->xposmax = ufo->xposmax - UFO_SPEED;			 
	}
	
	if(direction == ASCII_d)
	{
		ufo->xposmin = ufo->xposmin + UFO_SPEED;
		ufo->xposmax = ufo->xposmax + UFO_SPEED;			 
	}
}

/***************** SNCHRONOUS EVENTS *****************/
/*
Function Name: spawn_soldier

Purpose: This spawns a soldier going in a direction.
Details: If the toadd array has room ((*tail) != null), this function will spawn a soldier from the side.
The soldier is added to the array at the tail, 
and the tail moved over one up, if the array is full and the tail can't be moved, 
it's pointed it to null signifying fullness.

Input:  This takes in a direction of the Soldier to spawn, the array of Soldier objects on the map,
a tail pointer to the array of Soldiers on the map
*/
void spawn_soldier(int left_to_right, Soldier toadd[],Soldier* *tail){
	int i = 0;
	if((*tail) != NULL)
	{	
		if(left_to_right == TRUE)
		{
			(*(*tail)).xposmin = 1;
			(*(*tail)).xposmax = 8;
			(*(*tail)).xposold = 1;
			(*(*tail)).xposold2 = 1;
		}
		else
		{
			(*(*tail)).xposmin = 630;
			(*(*tail)).xposmax = 638;
			(*(*tail)).xposold = 630;
			(*(*tail)).xposold2 = 630;
		}
	
		(*(*tail)).lefttoright = left_to_right;	
	
		if((*(*tail)).xposmin == toadd[MAX_SOLDIERS-1].xposmin)
			(*tail) = NULL;
		else
		{
			while((*(*tail)).xposmin != toadd[i].xposmin)
				i++;
			*(tail) = &(toadd[i+1]);
		}	
	}
}

/* 
Function Name: spawn_plane

Purpose: This spawns a plane in one of the arrays.

Details: If the toadd array has room (tail != null), this function spawns a plane from the side,
The plane is added to the array at the tail and the tail moved over one up, 
if the array is full and the tail can't be moved, it points to null signifying fullness.
This function randomly generates a min height, and calculates a max height from it,
it checks that the map is not occupied by planes at any height it wants to spawn at,
if it is, it shifts down until you find an available spot, if it does not find an available spot, 
it shifts up until it finds one. The map's occupiedy array is also be updated.

Input Parameters: This takes in a direction of the Plane to spawn, the array of Plane objects on the map,
a tail pointer to the array of Planes, pointer to the map to update with new occupiedy data, and a speed of the
plane to be spawned.

*/
void spawn_plane(int left_to_right, Plane toadd[],Plane* *tail,Map *map, int speed){
    
	int start, try, i;
	int found = FALSE;
	srand((unsigned)(time(NULL)));
	start = (rand()) % (HEIGHT_OF_SKY-PLANEHEIGHT);
	try = start;
    
	if((*tail) != NULL)
	{
		while((found == FALSE) && (try <= HEIGHT_OF_SKY-PLANEHEIGHT))
		{
			if((map->occupiedy[try] == FALSE) && (map->occupiedy[try+PLANEHEIGHT-1] == FALSE))
				found = TRUE;
			else
			{
			try = try + 5;
			}
		}
		if(found == FALSE)
		{
			try = start;
			while((found == FALSE) && (try > 0))
			{
				if((map->occupiedy[try] == FALSE) && (map->occupiedy[try+PLANEHEIGHT-1] == FALSE))
					found = TRUE;
				else
				{
				try = try - 5;
				}
			}
		}
		for(i = try; i < try + PLANEHEIGHT; i++)
		{
			map->occupiedy[i] = TRUE;
		}
		(*(*tail)).yposmin = try;
		(*(*tail)).yposmax = try + PLANEHEIGHT-1;
		(*(*tail)).yposold = try;
		(*(*tail)).yposold2 = try;
		if(left_to_right == TRUE)
		{
			(*(*tail)).xposmin = 1;
			(*(*tail)).xposold = 1;
			(*(*tail)).xposold2 = 1;
			(*(*tail)).xposmax = PLANEWIDTH;
		}
		else
		{
			(*(*tail)).xposmin = 607;
			(*(*tail)).xposold = 607;
			(*(*tail)).xposold2 = 607;
			(*(*tail)).xposmax = 638;
		}

		(*(*tail)).lefttoright = left_to_right;
		(*(*tail)).speed = speed;	
	
		if((*(*tail)).yposmin == toadd[MAX_PLANES-1].yposmin)
			(*tail) = NULL;
		else
		{
			i = 0;
			while((*(*tail)).yposmin != toadd[i].yposmin)
				i++;
			*(tail) = &(toadd[i+1]);
		}
	}
}

/***************** CONDITION-BASED EVENTS ************/

/*
Function Name: destroyUFO

Purpose: This function destroys the UFO and removes a life, also updating the scoreboard.

Input Parameters: Pointer to the MainGame object "game" containing the data for the game model
*/

void destroyUFO(MainGame* game){
    play_effect_crash();
	game->player.lives = (game->player.lives)-1;
	game->beamfiring = FALSE;
	game->clearbeam = TRUE;
	if(game->player.lives == 0)
		game->gameover = TRUE;
	else
		respawnUFO(&(game->player), &(game->world));	
}

/*
Function Name: respawnUFO

Purpose: This function finds a location to spawn the UFO and spawns it there.

Details: Resets ufo's x values to the center, and its y values to the highest possible safe range in the 
occupiedy array.

Input Parameters: Pointer to the UFO object to respawn, pointer to the map to pass to 
find_acceptable_y function which finds a safe y-coordinate range wherein spawns the UFO

*/

void respawnUFO(UFO *ufo, Map *theMap){
    
	unsigned long respawntime = timenow + 50;
    
	while(timenow < respawntime)
	{
        userinput();
	}
    
	ufo->xposmax = UFO_MINX + 31;
	ufo->xposmin = UFO_MINX;
	ufo->yposmin = find_acceptable_y(theMap);
	ufo->yposmax = ufo->yposmin + 31;
	stop_effect_crash();
}

/* 
Function Name: find_acceptable_y

Purpose: Finds an acceptable y-coordinate range in which to respawn the UFO at.

Details:finds the first possible block of 32 consecutive y-coordinates that are
not currently occupied and returns a pointer to the lower bound of the block 

Input Parameters: Pointer to the Map object which has data on which y-coordinates have
plane objects occupying a range of y-values.

*/

int find_acceptable_y(Map *theMap){
	int j = 0;                                                  /* y-coordinate representing the first line of the UFO position */
	int notFound = FALSE;                                       /* Boolean indicating an acceptable range has been found for the UFO in the sky */
	int acceptableValue = FALSE;                                /* Boolean indicating the acceptable value doesn't allow for crashing into the ground or a plane instantly */

	while(acceptableValue == FALSE){
		while(notFound == FALSE && acceptableValue == FALSE)
		{
			if(theMap->occupiedy[j] == TRUE || theMap->occupiedy[j + 15] == TRUE ||
				theMap->occupiedy[j+UFOHEIGHT-1] == TRUE)        /* Finds a block within the array occupiedy */
				notFound = TRUE;                                 /* of size UFOHEIGHT containing the FALSE value */
			if(notFound == FALSE)                                /* indicating an acceptable range in which to place the UFO */
				acceptableValue = TRUE;
			else if(notFound == TRUE){
				notFound = FALSE;                                /* If no acceptable range found, j is incremented by UFOHEIGHT */
				j = j+UFOHEIGHT;                                 /* and a new check is initiated */
			}
			
			if(j > HEIGHT_OF_SKY-UFOHEIGHT)                      /* Checks to make sure the UFO wouldn't crash into the ground */
				acceptableValue = TRUE;
		}
	}
	return j;
}

/*
Function Name: despawn_soldier

Purpose: This removes a soldier from the toremovefrom array.

Details: This removes toremove from the array and shifts everything that was ahead of it back by one, if any.
It also adjusts the tail pointer so that it points to the next available spot.

Input Parameters:
toremove: the Soldier object to be removed from the map
toremovefrom[]: the array containing all Soldier objects on the map
*tail: pointer to the tail object of the toremovefrom array.

*/
void despawn_soldier(Soldier toremove, Soldier toremovefrom[], Soldier* *tail){
	int index = 0;
	
	while(toremovefrom[index].xposmin != toremove.xposmin)
	{
		index++;
	}
	
	if(*(tail) != NULL)
	{
		while((index < MAX_SOLDIERS-1) && ((*tail)->xposmin != toremovefrom[index].xposmin))
		{
			toremovefrom[index].xposmin = toremovefrom[index+1].xposmin;
			toremovefrom[index].xposmax = toremovefrom[index+1].xposmax;
			index++;
		}	
		(*tail) = toremovefrom+(index-1);
		}
	else
	{
		while(index < MAX_SOLDIERS-1)
		{
			toremovefrom[index].xposmin = toremovefrom[index+1].xposmin;
			toremovefrom[index].xposmax = toremovefrom[index+1].xposmax;
			index++;
		}
		(*tail) = &(toremovefrom[MAX_SOLDIERS-1]);
	}
	(*tail)->xposmin = -1;
}

/*
Funtion Name: desapwn_plane

Purpose: This removes a plane from the toremovefrom array.

Details: This removes toremove from the array and shifts everything that was ahead of it back by one, if any.
It also adjusts the tail pointer so that it points to the next available spot. This also frees the occupiedy 
spots in the map object that the plane was occupying.

Input Parameters:
toremove: the Plane object to be removed from the map
toremovefrom[]: the array containing all Plane objects on the map
*tail: pointer to the tail object of the toremovefrom array
*map: pointer the the Map object which must be updated with new data on which y values are
occupied by planes

*/
void despawn_plane(Plane toremove, Plane toremovefrom[] ,Plane* *tail, Map *map)
{
	int clear = toremove.yposmin;
	int index = 0;

	
	while(clear <= toremove.yposmax)
	{
		map->occupiedy[clear] = FALSE;
		clear++;
	}
	while(toremovefrom[index].yposmin != toremove.yposmin)
	{
		index++;
	}
	if(*(tail) != NULL)
	{
		while((index < MAX_PLANES-1) && ((*tail)->yposmin != toremovefrom[index].yposmin))
		{
			toremovefrom[index].xposmin = toremovefrom[index+1].xposmin;
			toremovefrom[index].xposmax = toremovefrom[index+1].xposmax;
			toremovefrom[index].yposmin = toremovefrom[index+1].yposmin;
			toremovefrom[index].yposmax = toremovefrom[index+1].yposmax;
			toremovefrom[index].speed = toremovefrom[index+1].speed;
			index++;
		}	
		(*tail) = toremovefrom+(index-1);
		}
	else
	{
		while(index < MAX_PLANES-1)
		{
			toremovefrom[index].xposmin = toremovefrom[index+1].xposmin;
			toremovefrom[index].xposmax = toremovefrom[index+1].xposmax;
			toremovefrom[index].yposmin = toremovefrom[index+1].yposmin;
			toremovefrom[index].yposmax = toremovefrom[index+1].yposmax;
			toremovefrom[index].speed = toremovefrom[index+1].speed;
			index++;
		}
		(*tail) = &(toremovefrom[MAX_PLANES-1]);
	}
	(*tail)->yposmin = -1;
}

