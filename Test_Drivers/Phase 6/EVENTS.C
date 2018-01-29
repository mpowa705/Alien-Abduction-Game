#include"events.h"
#include"model.h"
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

/*Team: Matthew Powaschuk, Brady Kirchner
COMP 2659 - Winter 2016
Prof: Paul Pospisil
Project: "Invader's revenge"
*/

/***************** ASYNCHRONOUS EVENTS *****************/


/*
Function Name: get_char_input
Purpose: Takes in input as a character from the keyboard

Input Parameters: none

Returns: ascii value of the given character
*/
int get_char_input(){

	int charInput = 0;
	charInput = getchar();			
	
	return charInput;
}

/*
Function Name: move_plane
Purpose: Moves an instance of a Plane object from its current x-coordinates to new x-coordinates
         based on current maximum and minumum x-coordinates and the value of its speed field.

Input Parameters: pointer to a Plane object

Returns: void
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

Input Parameters: pointer to a Soldier object

Returns: void
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

/*void move_missile(int left_to_right, Missile *missile){*/
/*
void move_missile(Missile *missile){

	if(missile->lefttoright == 1){
		missile->xposmax = missile->xposmax + missile->speed;
		missile->xposmin = missile->xposmin + missile->speed;
	}
	else if(missile->lefttoright == 0){
		missile->xposmax = missile->xposmax - missile->speed;
		missile->xposmin = missile->xposmin - missile->speed;
	}
}
*/

/*
Function Name: move_ufo
Purpose: Moves an instance of the UFO object from its current x/y-coordinates to new x/y-coordinates
         based on its current maximum and minumum x/y-coordinates and the value of the UFO_SPEED constant,
		 as well as the indicated direction.

Input Parameters: 
UFO *ufo: pointer to the UFO object
int direction: direction recorded previously as an ascii value given by the user

Returns: void
*/
void move_ufo(int direction, UFO *ufo){

	if(direction == ASCII_W || direction == ASCII_w)
	{
		ufo->yposmax = ufo->yposmax - UFO_SPEED;
		ufo->yposmin = ufo->yposmin - UFO_SPEED;		 
	}
		
	if(direction == ASCII_S || direction == ASCII_s)
	{
		ufo->yposmax = ufo->yposmax + UFO_SPEED;
		ufo->yposmin = ufo->yposmin + UFO_SPEED;
	}
	
	if(direction == ASCII_A || direction == ASCII_a)
	{
		ufo->xposmin = ufo->xposmin - UFO_SPEED;
		ufo->xposmax = ufo->xposmax - UFO_SPEED;			 
	}
	
	if(direction == ASCII_D || direction == ASCII_d)
	{
		ufo->xposmin = ufo->xposmin + UFO_SPEED;
		ufo->xposmax = ufo->xposmax + UFO_SPEED;			 
	}
}

/***************** SNCHRONOUS EVENTS *****************/
/*
Purpose: This spawns a soldier going in a direction.
Details: If the toadd array has room ((*tail) != null), this function will spawn a soldier from the side.
The soldier is added to the array at the tail, 
and the tail moved over one up, if the array is full and the tail can't be moved, 
it's pointed it to null signifying fullness.
Input: takes a direction, the array to add to, and the tail pointer.
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
Purpose: This spawns a plane in one of the arrays.

Details: If the toadd array has room (tail != null), this function spawns a plane from the side,
The plane is added to the array at the tail and the tail moved over one up, 
if the array is full and the tail can't be moved, it points to null signifying fullness.
This function randomly generates a min height, and calculates a max height from it,
it checks that the map is not occupied by planes at any height it wants to spawn at,
if it is, it shifts down until you find an available spot, if it does not find an available spot, 
it shifts up until it finds one. The map's occupiedy array is also be updated.

input: This takes a direction, the array, tail pointer, map to update, and a speed.
*/
void spawn_plane(int left_to_right, Plane toadd[],Plane* *tail,Map *map,
			int speed){
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
Purpose: This function destroys the UFO and removes a life, also updating the scoreboard.
Input: This takes the model by reference.
*/

void destroyUFO(MainGame* game){
	game->player.lives = (game->player.lives)-1;
	game->beamfiring = FALSE;
	game->clearbeam = TRUE;
	if(game->player.lives == 0)
		game->gameover = TRUE;
	else
		respawnUFO(&(game->player), &(game->world));	
}

/*
Purpose: This function finds a location to spawn the UFO and spawns it there.
Details: Resets ufo's x values to the center, and its y values to the highest possible safe range in the 
occupiedy array.
*/

void respawnUFO(UFO *ufo, Map *theMap){
	ufo->xposmax = UFO_MINX +31;
	ufo->xposmin = UFO_MINX;
	ufo->yposmin = find_acceptable_y(theMap);
	ufo->yposmax = ufo->yposmin + 31;	
}

/* 
Purpose: Finds an acceptable y range in which to respawn the UFO at.
Details:finds the first possible block of 16 consecutive y-coordinates that are
not currently occupied and returns a pointer to the lower bound of the block */

int find_acceptable_y(Map *theMap){
	int j = 0;
	int notFound = 0;
	int acceptableValue = 0;

	while(acceptableValue == 0){
		while(notFound == 0 && acceptableValue == 0)
		{
			if(theMap->occupiedy[j] == 1 || theMap->occupiedy[j + 15] == 1 ||
				theMap->occupiedy[j+31] == 1)
				notFound = 1;
			if(notFound == 0)
				acceptableValue = 1;
			else if(notFound == 1){
				notFound = 0;
				j = j+32;
			}
			
			if(j > HEIGHT_OF_SKY-PLANEHEIGHT)
				acceptableValue = 1;
		}
	} 
	if(j > HEIGHT_OF_SKY-PLANEHEIGHT)
	{
	srand((unsigned)(time(NULL))); /*if it couldn't find a range, it just picks one at random*/
	j = (rand()) % (HEIGHT_OF_SKY-UFOHEIGHT);
	}
	return j;
}

/*
void despawn_missile(Missile toremove, Missile toremovefrom[], Missile* *tail){
	int index = 0;

	printf("Despawning missile \n");
	
	while(toremovefrom[index].xposmin != toremove.xposmin)
	{
		index++;
	}
	
	if(*(tail) != NULL)
	{
		while((index < MAX_MISSILES-1) && (((*(*tail)).xposmin != toremovefrom[index].xposmin) || ((*(*tail)).yposmin != toremovefrom[index].yposmin)))
		{
			toremovefrom[index] = toremovefrom[index+1];
			index++;
		}	
		(*tail) = toremovefrom+(index-1);
		}
	else
	{
		while(index < MAX_MISSILES-1)
		{
			toremovefrom[index] = toremovefrom[index+1];
			index++;
		}
		(*tail) = &(toremovefrom[MAX_MISSILES-1]);
	}
}
*/


/*
Purpose: This removes a soldier from the toremovefrom array.
Details: This removes toremove from the array and shifts everything that was ahead of it back by one, if any.
It also adjusts the tail pointer so that it points to the next available spot.
*/
void despawn_soldier(Soldier toremove, Soldier toremovefrom[], Soldier* *tail){
	int index = 0;

	/*printf("Despawning Soldier \n");*/
	
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
Purpose: This removes a plane from the toremovefrom array.
Details: This removes toremove from the array and shifts everything that was ahead of it back by one, if any.
It also adjusts the tail pointer so that it points to the next available spot. This also frees the occupiedy 
spots in the map object that the plane was occupying.
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

