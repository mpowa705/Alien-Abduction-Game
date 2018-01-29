#ifndef MODEL_H
#define MODEL_H
#define TRUE 1
#define FALSE 0
#define XPOSMIN 192
#define XPOSMAX 223
#define HEIGHT_OF_SKY 331
#define PLANEHEIGHT 24
#define PLANEWIDTH 32
#define ASCII_A 65 
#define ASCII_a 97
#define ASCII_W 87
#define ASCII_w 119
#define ASCII_D	68
#define ASCII_d 100
#define ASCII_S 83
#define ASCII_s 115
#define BOUNDARY_TOP 0
#define BOUNDARY_LEFT 0
#define BOUNDARY_RIGHT 639
#define DEFAULTPLANESPEED 3
#define SOLDIER_SPEED 1
#define SOLDIERSPEED 1
#define UFO_SPEED 12
#define UFOHEIGHT 32
#define POINTS_PER_SOLDIER 10
#define GROUND 342
#define L 0
#define R 1
#define MAX_PLANES 3
#define MAX_SOLDIERS 5
#define MAX_MISSILES 12
#define MIN_FIRE_BEAM_HEIGHT 170
#define SOLDIER_HEIGHT 11

typedef struct UFOt {
    int lives;
    int score;
    int xposmax;
    int xposmin;
    int yposmax;
    int yposmin;
	int xposold;  /*1 frame ago*/
	int xposold2; /*2frames ago*/
	int yposold;
	int yposold2;
    int BeamReady;  /*Bool*/
}UFO;

typedef struct Planet {
    int xposmax;
    int xposmin;
    int yposmax;
    int yposmin;
	int xposold;  /*1 frame ago*/
	int xposold2; /*2frames ago*/
	int yposold;
	int yposold2;
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
	int xposold;  /*1 frame ago*/
	int xposold2; /*2frames ago*/
    int lefttoright;
} Soldier;

typedef struct Mapt {
    unsigned char* base08;
    unsigned int*  base16;
    unsigned long* base32;
    int occupiedy[HEIGHT_OF_SKY];  /*used for spawning planes*/
} Map;


typedef struct Gamet {
	int gameover;
	UFO player;
	Map world;
	int beamfiring;
	int clearbeam;
	int planespeed;
	int planespawnspeed;
	Plane *tailPleft;
	Plane *tailPright;
	Plane *tailPleftold;
	Plane *tailPrightold;
	Plane *tailPleftold2;
	Plane *tailPrightold2;
	Plane planesleft[MAX_PLANES+1]; 
	Plane planesright[MAX_PLANES+1];
	Missile *tailMright;
	Missile *tailMleft;
	Missile missilesleft[12];
	Missile missilesright[12];
	Soldier *tailSright;
	Soldier *tailSleft;
	Soldier *tailSrightold;
	Soldier *tailSleftold;
	Soldier *tailSrightold2;
	Soldier *tailSleftold2;
	Soldier soldiersright[5];
	Soldier soldiersleft[5];
} MainGame;


void initializegame(MainGame *game);
void updateplanespeed(int score, MainGame *game);
void firebeam(MainGame *game, UFO *ufo, Soldier left[], Soldier right[], Soldier* *lefttail, Soldier* *righttail);
void stopfiring(MainGame *game);
int requestfirebeam(MainGame *game, UFO *ufo, Soldier left[], Soldier right[], Soldier* *lefttail, Soldier* *righttail);
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
void nextframe(MainGame* game);




#endif
