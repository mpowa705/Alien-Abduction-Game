#ifndef MODEL_H
#define MODEL_H

#define UFOSPEED 2
#define SOLDIERSPEED 3
#define TRUE 1
#define FALSE 0
#define HEIGHT_OF_SKY 334
#define MAX_LEFT_PLANES 3
#define MAX_RIGHT_PLANES 3


typedef struct UFOt {
    int lives;
    int score;
    int xposmax;
    int xposmin;
    int yposmax;
    int yposmin;
    int BeamReady;  /*Bool*/
}UFO;

typedef struct Planet {
    int xposmax;
    int xposmin;
    int yposmax;
    int yposmin;
    int speed;
    int lefttoright;
} Plane;

typedef struct Missilet {
    int xposmax;
    int xposmin;
    int yposmax;
	int yposmin;
    int speed;
    int lefttoright;
} Missile;

typedef struct Soldiert {
    int xposmax;
    int xposmin;
    int lefttoright;
} Soldier;

typedef struct Mapt {
    char* base08;
    int*  base16;
    long* base32;
    int occupiedy[HEIGHT_OF_SKY];  /*used for spawning planes*/
} Map;


typedef struct Gamet {
	int gameover;
	UFO player;
	Map world;
	int beamfiring;
	int planespeed;
	int planespawnspeed;
	Plane *tailPleft;
	Plane *tailPright;
	Plane planesleft[MAX_LEFT_PLANES]; 
	Plane planesright[MAX_RIGHT_PLANES];
	Missile *tailMright;
	Missile *tailMleft;
	Missile missilesleft[12];
	Missile missilesright[12];
	Soldier *tailSright;
	Soldier *tailSleft;
	Soldier soldiersright[5];
	Soldier soldiersleft[5];
} MainGame;


void initializegame(MainGame *game);
void updateplanespeed(int score, MainGame *game);
void firebeam(MainGame *game, UFO *ufo, Soldier left[], Soldier right[], Soldier* *lefttail, Soldier* *righttail);
void stopfiring(MainGame *game);
void requestfirebeam(MainGame *game, UFO *ufo, Soldier left[], Soldier right[], Soldier* *lefttail, Soldier* *righttail);
void requestmoveufo(int direction, MainGame *game, UFO *ufo, Map *map);
void requestmoveplanes(MainGame *game, UFO *ufo, Map *map);
void requestmovesoldiers(MainGame *game, UFO *ufo);
void requestmovemissiles(MainGame *game, UFO *ufo);
int check_collision_ufoVSsoldier_horiz(int direction, MainGame *game, UFO *ufo);
int check_collision_ufoVSsoldier_vert(MainGame *game, UFO *ufo);
int check_collision_ufoVSplane_vert(int direction, MainGame *game, UFO *ufo, Map *map);
int check_collision_ufoVSplane_horiz(int direction, MainGame *game, UFO *ufo, Map *map);
int check_collision_ufoVSmissile_horiz(int direction, MainGame *game, UFO *ufo);
int check_collision_ufoVSmissile_vert(int direction, MainGame *game, UFO *ufo);

#endif
