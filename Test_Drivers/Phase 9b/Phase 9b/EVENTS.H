#ifndef EVENTS_H
#define EVENTS_H
#include"model.h"

int get_char_input();
void destroyUFO(MainGame *game);
void respawnUFO(UFO *ufo, Map *theMap);
int find_acceptable_y(Map *theMap);
void spawn_missile(int high, Plane *from, Missile toadd[], Missile* *tail);
void despawn_missile(Missile toremove, Missile toremovefrom[], Missile* *tail);
void spawn_soldier(int left_to_right, Soldier toadd[],Soldier* *tail);
void despawn_soldier(Soldier toremove, Soldier toremovefrom[],Soldier* *tail);
void spawn_plane(int left_to_right, Plane toadd[],Plane* *tail,Map *map,
	int speed);
void despawn_plane(Plane toremove,Plane toremovefrom[],Plane* *tail,Map *map);
void move_ufo(unsigned char direction, UFO *ufo);
void move_plane(Plane *plane);
void move_soldier(Soldier *soldier);
void move_missile(Missile *missile);

#define XPOSMIN 192
#define XPOSMAX 208
#define HEIGHT_OF_SKY 331
#define PLANEHEIGHT 24
#define PLANEWIDTH 32

#endif