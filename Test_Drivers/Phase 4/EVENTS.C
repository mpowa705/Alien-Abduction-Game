#include"events.h"
#include"model.h"
#include<stdio.h>
#include<stdlib.h>
#include<time.h>



/***************** ASYNCHRONOUS EVENTS *****************/

int get_char_input(){

	int charInput = 0;
	charInput = getchar();			
	
	return charInput;
}

void move_plane(Plane *plane){

	/*printf("Plane old xposmin: %d \n", plane->xposmin);*/
	if(plane->lefttoright == 1){
		plane->xposmax = plane->xposmax + plane->speed;
		plane->xposmin = plane->xposmin + plane->speed;
	}
	else if(plane->lefttoright == 0){
		plane->xposmax = plane->xposmax - plane->speed;
		plane->xposmin = plane->xposmin - plane->speed;
	}
	
	/*printf("Plane new xposmin: %d", plane->xposmin);*/
}

void move_soldier(Soldier *soldier){

	/*printf("Soldier old xposmin: %d \n", soldier->xposmin);*/
	if(soldier->lefttoright == 1){
		soldier->xposmax = soldier->xposmax + SOLDIER_SPEED;
		soldier->xposmin = soldier->xposmin + SOLDIER_SPEED;
	}
	else if(soldier->lefttoright == 0){
		soldier->xposmax = soldier->xposmax - SOLDIER_SPEED;
		soldier->xposmin = soldier->xposmin - SOLDIER_SPEED;
	}
	
	/*printf("Soldier new xposmin: %d \n", soldier->xposmin);*/

}

/*void move_missile(int left_to_right, Missile *missile){*/
void move_missile(Missile *missile){

	printf("Missile old xposmin: %d \n", missile->xposmin);
	if(missile->lefttoright == 1){
		missile->xposmax = missile->xposmax + missile->speed;
		missile->xposmin = missile->xposmin + missile->speed;
	}
	else if(missile->lefttoright == 0){
		missile->xposmax = missile->xposmax - missile->speed;
		missile->xposmin = missile->xposmin - missile->speed;
	}
	
	printf("Missile new xposmin: %d \n", missile->xposmin);
}

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
If the toadd array has room (tail != null), this function will spawn a missile from the Plane
(if the missile wouldn't spawn on the outside of the map already, 
which is very rare but still needs to be checked),
the high variable is a boolean that, if true, tells the functions to spawn it from the
top wing, if false, bottom wing. The missile must be added to the array at the tail, 
and the tail moved over one, if the array is full and the tail can't be moved, 
point it to null signifying fullness.
*/
void spawn_missile(int high,Plane *from, Missile toadd[], Missile* *tail){
	int i = 0;
	if((*tail) != NULL)
	{
		if(((from->lefttoright == TRUE) && (from->xposmax <= 635)) || ((from->lefttoright == FALSE) && (from->xposmin >= 4)))
		{
			if(from->lefttoright == TRUE)
			{
				(*(*tail)).xposmin = from->xposmax + 1;
				(*(*tail)).xposmax = from->xposmax + 4;
			}
			else
			{
				(*(*tail)).xposmin = from->xposmin - 4;
				(*(*tail)).xposmax = from->xposmin - 1;
			}
	
			if(high == TRUE)
			{
				(*(*tail)).yposmin = from->yposmin + 5;
			}
			else
			{
				(*(*tail)).yposmin = from->yposmax - 5;    /* 	missiles only have ypos
													because it's only 2 pixels high	
													so the ypos represents the top pixel*/
			}
		
			(*(*tail)).lefttoright = from->lefttoright;	
			(*(*tail)).speed = (from->speed * 2);
	
	

			if((*(*tail)).xposmin == toadd[MAX_MISSILES-1].xposmin)
				(*tail) = NULL;
			else
			{
				while(((*(*tail)).xposmin != toadd[i].xposmin) || ((*(*tail)).yposmin != toadd[i].yposmin))
					i++;
				*(tail) = &(toadd[i+1]);
			}	
		}
	}
}

/*
If the toadd array has room (tail != null), this function will spawn a soldier from the side.
The soldier must be added to the array at the tail, 
and the tail moved over one up, if the array is full and the tail can't be moved, 
point it to null signifying fullness.
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
If the toadd array has room (tail != null), this function will spawn a plane from the side,
The plane must be added to the array at the tail, 
and the tail moved over one up, if the array is full and the tail can't be moved, 
point it to null signifying fullness.
This function must randomly generate a max height, and calculate min height from it,
it must check that the map is not occupied by planes at any height it wants to spawn at,
if it is, shift down until you find an available spot, if you do not find an available spot, shift up,
eventually you have to find one. 
The map's occupied y array must be updated.
*/
void spawn_plane(int left_to_right, Plane toadd[],Plane* *tail,Map *map,
			MainGame *game){
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
		if(left_to_right == TRUE)
		{
			(*(*tail)).xposmin = 1;
			(*(*tail)).xposmax = PLANEWIDTH;
		}
		else
		{
			(*(*tail)).xposmin = 607;
			(*(*tail)).xposmax = 638;
		}

		(*(*tail)).lefttoright = left_to_right;
		(*(*tail)).speed = game->planespeed;	
	
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
This function must destroy the UFO and remove a life, also updating the scoreboard.
*/

void destroyUFO(MainGame* game){
	game->player.lives = (game->player.lives)-1;
	if(game->player.lives == 0)
		game->gameover = TRUE;
	else
		respawnUFO(&(game->player), &(game->world));	
}

/*
Resets ufo's x values to the center, and its y values to the highest possible safe range in the 
occupiedy array.
*/

void respawnUFO(UFO *ufo, Map *theMap){
	ufo->xposmax = 223;
	ufo->xposmin = 192;
	ufo->yposmin = find_acceptable_y(theMap);
	ufo->yposmax = ufo->yposmin + 31;	
}

/* finds the first possible block of 16 consecutive y-coordinates that are
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
	srand((unsigned)(time(NULL)));
	j = (rand()) % (HEIGHT_OF_SKY-UFOHEIGHT);
	}
	return j;
}

/*
This must remove toremove from the array and shift everything that was ahead of it back by one, if any.
It must also adjust the tail pointer so that it points to the next available spot. (shallow copy is OK for
the shifting back, Missiles have no pointers)
*/
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

/*
This must remove toremove from the array and shift everything that was ahead of it back by one, if any.
It must also adjust the tail pointer so that it points to the next available spot. (shallow copy is OK for
the shifting back, Soldiers have no pointers)
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
}

/*
This must remove toremove from the array and shift everything that was ahead of it back by one, if any.
It must also adjust the tail pointer so that it points to the next available spot. (shallow copy is OK for
the shifting back, Planes have no pointers) this must also free the occupiedy spots in the map object that the plane
was occupying.
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
			index++;
		}
		(*tail) = &(toremovefrom[MAX_PLANES-1]);
	}
}

