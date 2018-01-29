# 1 "model.c" 1
# 1 "./events.h" 1
# 1 "./model.h" 1
# 36 "./model.h"
typedef struct UFOt {
    int lives;
    int score;
    int xposmax;
    int xposmin;
    int yposmax;
    int yposmin;
	int xposold;
	int xposold2;
	int yposold;
	int yposold2;
    int BeamReady;
}UFO;

typedef struct Planet {
    int xposmax;
    int xposmin;
    int yposmax;
    int yposmin;
	int xposold;
	int xposold2;
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
	int xposold;
	int xposold2;
    int lefttoright;
} Soldier;

typedef struct Mapt {
    unsigned char* base08;
    unsigned int*  base16;
    unsigned long* base32;
    int occupiedy[331 ];
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
	Plane *tailPleftold;
	Plane *tailPrightold;
	Plane *tailPleftold2;
	Plane *tailPrightold2;
	Plane planesleft[3 ];
	Plane planesright[3 ];
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
void nextframe(MainGame* game);
# 3 "./events.h" 2


int get_char_input();
void destroyUFO(MainGame *game);
void respawnUFO(UFO *ufo, Map *theMap);
int find_acceptable_y(Map *theMap);
void spawn_missile(int high, Plane *from, Missile toadd[], Missile* *tail);
void despawn_missile(Missile toremove, Missile toremovefrom[], Missile* *tail);
void spawn_soldier(int left_to_right, Soldier toadd[],Soldier* *tail);
void despawn_soldier(Soldier toremove, Soldier toremovefrom[],Soldier* *tail);
void spawn_plane(int left_to_right, Plane toadd[],Plane* *tail,Map *map,
	MainGame *game);
void despawn_plane(Plane toremove,Plane toremovefrom[],Plane* *tail,Map *map);
void move_ufo(int direction, UFO *ufo);
void move_plane(Plane *plane);
void move_soldier(Soldier *soldier);
void move_missile(Missile *missile);
# 1 "model.c" 2
# 1 "./model.h" 1
# 2 "model.c" 2
# 1 "C:/c68/include/osbind.h" 1
# 1 "C:/c68/include/compiler.h" 1
# 9 "C:/c68/include/osbind.h" 2
# 1 "C:/c68/include/ostruct.h" 1
# 22 "C:/c68/include/ostruct.h"
typedef struct {
    long b_free;
    long b_total;
    long b_secsiz;
    long b_clsiz;
} _DISKINFO;


typedef struct {
  short time;
  short date;
} _DOSTIME;


typedef struct
{
        unsigned char maxlen;
        unsigned char actuallen;
        char    buffer[255];
} _CCONLINE;
# 48 "C:/c68/include/ostruct.h"
typedef struct _dta {
    char 	    dta_buf[21];
    char            dta_attribute;
    unsigned short  dta_time;
    unsigned short  dta_date;
    long            dta_size;
    char            dta_name[14];
} _DTA;
# 98 "C:/c68/include/ostruct.h"
typedef struct {
  short recsiz;
  short clsiz;
  short clsizb;
  short rdlen;
  short fsiz;
  short fatrec;
  short datrec;
  short numcl;
  short bflags;
} _BPB;




typedef struct _md {
    struct _md	*md_next;
    long	 md_start;
    long	 md_length;
    long	 md_owner;
} _MD;


typedef struct {
    _MD *mp_free;
    _MD *mp_used;
    _MD *mp_rover;
} _MPB;
# 141 "C:/c68/include/ostruct.h"
typedef struct {
    char    *ibuf;
    short   ibufsiz;
    volatile short   ibufhd;
    volatile short   ibuftl;
    short   ibuflow;
    short   ibufhi;
} _IOREC;


typedef struct {
	char	topmode;
	char	buttons;
	char	xparam;
	char	yparam;
	short	xmax;
	short	ymax;
	short	xstart;
	short	ystart;
} _PARAM;


typedef struct {
    void    (*midivec)	(void) ;
    void    (*vkbderr)	(void) ;
    void    (*vmiderr)	(void) ;
    void    (*statvec)	(void *) ;
    void    (*mousevec)	(void *) ;
    void    (*clockvec)	(void *) ;
    void    (*joyvec)	(void *) ;
    long    (*midisys)	(void) ;
    long    (*ikbdsys)	(void) ;
    char    kbstate;
} _KBDVECS;


typedef struct {
    void *unshift;
    void *shift;
    void *caps;
} _KEYTAB;


typedef struct
{
        void    *pb_scrptr;
        int     pb_offset;
        int     pb_width;
        int     pb_height;
        int     pb_left;
        int     pb_right;
        int     pb_screz;
        int     pb_prrez;
        void    *pb_colptr;
        int     pb_prtype;
        int     pb_prport;
        void    *pb_mask;
} _PBDEF;
# 17 "C:/c68/include/osbind.h" 2
# 33 "C:/c68/include/osbind.h"
 long _trap_1_w		(short) ;
 long _trap_1_ww	(short,short) ;
 long _trap_1_wl	(short,long) ;
 long _trap_1_wlw	(short,long,short) ;
 long _trap_1_wwll	(short,short,long,long) ;
 long _trap_1_wlww	(short,long,short,short) ;
 long _trap_1_www	(short,short,short) ;
 long _trap_1_wll	(short,long,long) ;
 long _trap_1_wwlll	(short,short,long,long,long) ;
 long _trap_1_wwwll	(short,short,short,long,long) ;
 long _trap_13_wl	(short,long) ;
 long _trap_13_w	(short) ;
 long _trap_13_ww	(short,short) ;
 long _trap_13_www	(short,short,short) ;
 long _trap_13_wwlwww	(short,short,long,short,short,short) ;
 long _trap_13_wwl	(short,short,long) ;
 long _trap_14_wwl	(short,short,long) ;
 long _trap_14_wwll	(short,short,long,long) ;
 long _trap_14_ww	(short,short) ;
 long _trap_14_w	(short) ;
 long _trap_14_wllw	(short,long,long,short) ;
 long _trap_14_wl	(short,long) ;
 long _trap_14_www	(short,short,short) ;
 long _trap_14_wllwwwww (short,long,long,short,short,short,short,short) ;
 long _trap_14_wllwwwwlw (short,long,long,short,short,short,short,long,short) ;
 long _trap_14_wllwwwwwlw (short,long,long,short,short,short,short,short,long,short) ;
 long _trap_14_wwwwwww	(short,short,short,short,short,short,short) ;
 long _trap_14_wlll	(short,long,long,long) ;
 long _trap_14_wllww	(short,long,long,short,short) ;
 long _trap_14_wwwwl	(short,short,short,short,long) ;
 long _trap_14_wwwl	(short,short,short,long) ;
 long _trap_14_wlwlw	(short,long,short,long,short) ;
# 103 "C:/c68/include/osbind.h"
 long _trap_1_ 	(short,...) ;
 long _trap_14_ 	(short,...) ;
 long _trap_13_ 	(short,...) ;
# 3 "model.c" 2
# 1 "C:/c68/include/stdio.h" 1
# 20 "C:/c68/include/stdio.h"
typedef unsigned long  size_t;
# 69 "C:/c68/include/stdio.h"
typedef	struct
	{
	long		_cnt;
	unsigned char	*_ptr;
	unsigned char	*_base;
	unsigned int	_flag;
	int		_file;
	long		_bsiz;
	unsigned char	_ch;
	}
	FILE;


typedef unsigned long fpos_t;
# 92 "C:/c68/include/stdio.h"
extern	FILE	_iob[];
# 108 "C:/c68/include/stdio.h"
 char *	ctermid	(char *) ;




 int	remove	(const char *) ;
 int	rename	(const char *, const char *) ;
 char *	tmpnam	(char *) ;
 FILE *	tmpfile	(void) ;

 int	fclose	(FILE *) ;
 int	fflush	(FILE *) ;

 FILE *	fopen	(const char *, const char *) ;
 FILE *	freopen	(const char *, const char *, FILE *) ;

 void	setbuf	(FILE *, char *) ;
 int	setvbuf	(FILE *, char *, int, size_t) ;
# 132 "C:/c68/include/stdio.h"
 int  fscanf  (FILE *, const char *, ...) ;
 int  scanf   (const char *, ...) ;
 int  sscanf  (const char *, const char *, ...) ;


 int	fprintf	(FILE *, const char *, ...) ;
 int	printf	(const char *, ...) ;
 int	sprintf	(char *, const char *, ...) ;

 int 	vfprintf (FILE *, const char *, char * ) ;
 int 	vprintf	 (const char *, char * ) ;
 int 	vsprintf (char *, const char *, char * ) ;

 int	vscanf  (const char *, char * ) ;
 int	vfscanf (FILE *, const char *, char * ) ;
 int	vsscanf	(const char *, const char *, char * ) ;


 int	fgetc	(FILE *) ;
 char	*fgets	(char *, int, FILE *) ;
 char	*gets	(char *) ;
 int	fputc	(int c, FILE *) ;
 int	fputs	(const char *, FILE *) ;
 int	puts	(const char *) ;

 size_t	fread	(void *, size_t, size_t, FILE *) ;
 size_t	fwrite	(const void *, size_t, size_t, FILE *) ;

 int	fgetpos	(FILE *, fpos_t *) ;
 int	fsetpos	(FILE *, fpos_t *) ;

 int	fseek	(FILE *, long, int) ;
 long	ftell	(FILE *) ;
 void	rewind	(FILE *) ;

 void	perror	(const char *) ;


 FILE	*fdopen	(int, const char *) ;


 FILE *	fopenp	(const char *, const char *) ;
 int 	fungetc	(int, FILE *) ;
 int	pclose	(FILE *) ;
 FILE *	popen	(const char *, const char *) ;
 void	setlinebuf	(FILE *) ;

 void	_binmode	(int) ;
 long 	getl	(FILE *) ;
 long 	putl	(long, FILE *) ;
 short 	getw	(FILE *) ;
 short 	putw	(short, FILE *) ;
 void	_getbuf	(FILE *fp) ;
# 192 "C:/c68/include/stdio.h"
 int	_filbuf	(FILE *) ;
# 4 "model.c" 2


void initializegame(MainGame *game)
{
	int i = 0;
	game->gameover = 0 ;
	game->world.base08 = (void*)_trap_14_w((short)0x2) ;
	game->world.base16 = (void*)_trap_14_w((short)0x2) ;
	game->world.base32 = (void*)_trap_14_w((short)0x2) ;
	game->player.lives = 3;
	game->player.score = 0;
	game->player.yposmin = 176;
	game->player.yposmax = 207;
	game->player.xposmin = 185;
	game->player.xposmax = 216;
	game->player.xposold = 185;
	game->player.xposold2 = 185;
	game->player.yposold = 176;
	game->player.yposold2 = 176;
	game->player.BeamReady = 1 ;
	game->planespeed = 3 ;

	game->planespawnspeed = 5;
	game->beamfiring = 0 ;
	game->player.lives = 3;


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


	while(i < 331 )
	{
		game->world.occupiedy[i] = 0 ;

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
	while((game->tailPright->yposmin != game->planesright[i].yposmin))
	{
		game->planesright[i].xposold2 = game->planesright[i].xposold;
		game->planesright[i].xposold = game->planesright[i].xposmin;
		game->planesright[i].yposold2 = game->planesright[i].yposold;
		game->planesright[i].yposold = game->planesright[i].yposmin;
		i++;
	}
	i = 0;
	while((game->tailPleft->yposmin != game->planesleft[i].yposmin))
	{
		game->planesleft[i].xposold2 = game->planesleft[i].xposold;
		game->planesleft[i].xposold = game->planesleft[i].xposmin;
		game->planesleft[i].yposold2 = game->planesleft[i].yposold;
		game->planesleft[i].yposold = game->planesleft[i].yposmin;
		i++;
	}
	i = 0;
	while((game->tailSright->xposmin != game->soldiersright[i].xposmin))
	{
		game->soldiersright[i].xposold2 = game->soldiersright[i].xposold;
		game->soldiersright[i].xposold = game->soldiersright[i].xposmin;
		i++;
	}
	i = 0;
	while((game->tailSleft->xposmin != game->soldiersleft[i].xposmin))
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
# 120 "model.c"
void requestmoveufo(int direction, MainGame *game, UFO *ufo, Map *map){
	int invalidMove = 0;
	int removeSleftcounter = 0;
	int removeSrightcounter = 0;
	int i=0;
	int soldierHit = 0;
	Soldier *toremoveSleft[5 ];
	Soldier *toremoveSright[5 ];

	if(!(game->beamfiring)){

		if(direction == 87  || direction == 119 ){
			if(!(ufo->yposmin < (0 +12 ))){
				invalidMove = check_collision_ufoVSplane_vert(direction, game, ufo, map);

				if(invalidMove == 0)
					invalidMove = check_collision_ufoVSmissile_vert(direction, game, ufo);

			}
			else invalidMove = 1;
		}

		else if(direction == 65  || direction == 97 ){
			if(!(ufo->xposmin < (0 +12 ))){
				invalidMove = check_collision_ufoVSplane_horiz(direction, game, ufo, map);

				if(invalidMove == 0)
					invalidMove = check_collision_ufoVSmissile_horiz(direction, game, ufo);

				if(invalidMove == 0 && ufo->yposmax >= 342 -11 ){

					invalidMove = check_collision_ufoVSsoldier_horiz(direction, game, ufo);
				}
			}
			else invalidMove = 1;
		}

		else if(direction == 68  || direction == 100 ){
			if(!(ufo->xposmax > (639 -12 ))){
				invalidMove = check_collision_ufoVSplane_horiz(direction, game, ufo, map);

				if(invalidMove == 0)
					invalidMove = check_collision_ufoVSmissile_horiz(direction, game, ufo);

				if(invalidMove == 0 && ufo->yposmax >= 342 -11 ){

					invalidMove = check_collision_ufoVSsoldier_horiz(direction, game, ufo);
				}
			}
			else invalidMove = 1;
		}

		else if(direction == 83  || direction == 115 ){
			if(ufo->yposmax >= (342 -12 )){
				destroyUFO(game);

				invalidMove = 1;
			}
			else{
				invalidMove = check_collision_ufoVSplane_vert(direction, game, ufo, map);

				if(invalidMove == 0)
					invalidMove = check_collision_ufoVSmissile_vert(direction, game, ufo);

				if((invalidMove == 0 && ufo->yposmax >= 342 -11 -12 ) &&
						(ufo->yposmax < 342 -11 )){
					invalidMove = check_collision_ufoVSsoldier_vert(game, ufo);
				}
			}
		}

		if(invalidMove == 0)
			move_ufo(direction, ufo);
	}
}
# 203 "model.c"
void requestmoveplanes(MainGame *game, UFO *ufo, Map *map){
	int i = 0;
	int mustDespawn = 0;
	int planeHit = 0;
	Plane *toremovePleft[3 ];
	Plane *toremovePright[3 ];
	int removePleftcounter = 0;
	int removePrightcounter = 0;

	while((game->tailPleft != &(game->planesleft[i])) && i < 3 ){
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

			toremovePleft[removePleftcounter] = &(game->planesleft[i]);
			removePleftcounter++;
		}
		else{
			move_plane(&(game->planesleft[i]));
		}
		i++;
	}
	i = 0;
	while((game->tailPright != &(game->planesright[i])) && i < 3 ){
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


int check_collision_ufoVSmissile_vert(int direction, MainGame *game, UFO *ufo){
	int missileHit = 0;
	int i = 0;

	while(missileHit == 0 && ((game->tailMleft) != &(game->missilesleft[i])) && i < 12 ){
		if((ufo->xposmax <= (game->missilesleft[i]).xposmax && ufo->xposmax >= (game->missilesleft[i]).xposmin) ||
				(ufo->xposmin >= (game->missilesleft[i]).xposmin && ufo->xposmin <= (game->missilesleft[i]).xposmax)){
			if(((direction == 87  || direction == 119 ) &&
				(ufo->yposmin <= ((game->missilesleft[i]).yposmax)+12 ))
				||
				((direction == 83  || direction == 115 ) &&
				(ufo->yposmax >= ((game->missilesleft[i]).yposmin)-12 )))
			{

				missileHit = 1;
				despawn_missile(game->missilesleft[i], game->missilesleft, &(game->tailMleft));
				destroyUFO(game);
			}
		}
		i++;
	}
	i = 0;
	while(missileHit == 0 && ((game->tailMright) != &(game->missilesright[i])) && i < 12 ){
		if((ufo->xposmax <= (game->missilesright[i]).xposmax && ufo->xposmax >= (game->missilesright[i]).xposmin) ||
				(ufo->xposmin >= (game->missilesright[i]).xposmin && ufo->xposmin <= (game->missilesright[i]).xposmax)){
			if(((direction == 87  || direction == 119 ) &&
				(ufo->yposmin <= ((game->missilesright[i]).yposmax)+12 ))
				||
				((direction == 83  || direction == 115 ) &&
				(ufo->yposmax >= ((game->missilesright[i]).yposmin)-12 )))
			{

				missileHit = 1;
				despawn_missile(game->missilesright[i], game->missilesright, &(game->tailMright));
				destroyUFO(game);
			}
		}
	    i++;
	}
	return missileHit;
}


int check_collision_ufoVSmissile_horiz(int direction, MainGame *game, UFO *ufo){

	int missileHit = 0;
	int i = 0;

	while(missileHit == 0 && ((game->tailMleft) != &(game->missilesleft[i])) && i < 12 ){
		if(((direction == 68  || direction == 100 ) &&
			(((game->missilesleft[i]).yposmax >= ufo->yposmin && (game->missilesleft[i]).yposmax <= ufo->yposmax) ||
			((game->missilesleft[i]).yposmin >= ufo->yposmin && (game->missilesleft[i]).yposmin <= ufo->yposmax))
			&&
			(game->missilesleft[i]).xposmin <= ufo->xposmax+12  &&
			(game->missilesleft[i]).xposmax > ufo->xposmax)
			||
			((direction == 65  || direction == 97 ) &&
			(game->missilesleft[i]).xposmax >= ufo->xposmin-12  &&
			(game->missilesleft[i]).xposmin < ufo->xposmin))
		{

			missileHit = 1;
			despawn_missile(game->missilesleft[i], game->missilesleft, &(game->tailMleft));
		}
		i++;
	}
	i=0;
	missileHit = 0;
	while(missileHit == 0 && (game->tailMright) != &(game->missilesright[i]) && i < 12 ){
		if(((direction == 68  || direction == 100 ) &&
			(((game->missilesright[i]).yposmax >= ufo->yposmin && (game->missilesright[i]).yposmax <= ufo->yposmax) ||
			((game->missilesright[i]).yposmin >= ufo->yposmin && (game->missilesright[i]).yposmin <= ufo->yposmax))
			&&
			(game->missilesright[i]).xposmin <= ufo->xposmax+12  &&
			(game->missilesright[i]).xposmax > ufo->xposmax)
			||
			((direction == 65  || direction == 97 ) &&
			(game->missilesright[i]).xposmax >= ufo->xposmin-12  &&
			(game->missilesright[i]).xposmin < ufo->xposmin))
		{

			missileHit = 1;
			despawn_missile(game->missilesright[i], game->missilesright, &(game->tailMright));
		}
		i++;
	}
	return missileHit;
}


int check_collision_ufoVSplane_vert(int direction, MainGame *game, UFO *ufo, Map *map){
	int planeHit = 0;
	int i = 0;

	while(planeHit == 0 && ((game->tailPleft) != &(game->planesleft[i])) && i < 3 ){
		if((ufo->xposmax <= (game->planesleft[i]).xposmax && ufo->xposmax >= (game->planesleft[i]).xposmin) ||
				(ufo->xposmin >= (game->planesleft[i]).xposmin && ufo->xposmin <= (game->planesleft[i]).xposmax)){
			if(((direction == 87  || direction == 119 ) &&
				(ufo->yposmin <= ((game->planesleft[i]).yposmax)+12 ))
				||
				((direction == 83  || direction == 115 ) &&
				(ufo->yposmax >= ((game->planesleft[i]).yposmin)-12 )))
			{

				planeHit = 1;
				despawn_plane(game->planesleft[i], game->planesleft, &(game->tailPleft), map);
				destroyUFO(game);
			}
		}
		i++;
	}
	i = 0;
	while(planeHit == 0 && ((game->tailPright) != &(game->planesright[i])) && i < 3 ){
		if((ufo->xposmax <= (game->planesright[i]).xposmax && ufo->xposmax >= (game->planesright[i]).xposmin) ||
				(ufo->xposmin >= (game->planesright[i]).xposmin && ufo->xposmin <= (game->planesright[i]).xposmax)){
			if(((direction == 87  || direction == 119 ) &&
				(ufo->yposmin <= ((game->planesright[i]).yposmax)+12 ))
				||
				((direction == 83  || direction == 115 ) &&
				(ufo->yposmax >= ((game->planesright[i]).yposmin)-12 )))
			{

				planeHit = 1;
				despawn_plane(game->planesright[i], game->planesright, &(game->tailPright), map);
				destroyUFO(game);
			}
		}
		i++;
	}
	return planeHit;
}


int check_collision_ufoVSplane_horiz(int direction, MainGame *game, UFO *ufo, Map *map){
	int planeHit = 0;
	int i = 0;

	while(planeHit == 0 && ((game->tailPleft) != &(game->planesleft[i])) && i < 3 ){
		if(((direction == 68  || direction == 100 ) &&
			(((game->planesleft[i]).yposmax >= ufo->yposmin && (game->planesleft[i]).yposmax <= ufo->yposmax) ||
			((game->planesleft[i]).yposmin >= ufo->yposmin && (game->planesleft[i]).yposmin <= ufo->yposmax))
			&&
			(game->planesleft[i]).xposmin <= ufo->xposmax+12  &&
			(game->planesleft[i]).xposmax > ufo->xposmax)
			||
			((direction == 65  || direction == 97 ) &&
			(game->planesleft[i]).xposmax >= ufo->xposmin-12  &&
			(game->planesleft[i]).xposmin < ufo->xposmin))
		{

			planeHit = 1;
			despawn_plane(game->planesleft[i], game->planesleft, &(game->tailPleft), map);
		}
		i++;
	}
	i=0;
	planeHit = 0;
	while(planeHit == 0 && (game->tailPright) != &(game->planesright[i]) && i < 3 ){
		if(((direction == 68  || direction == 100 ) &&
			(((game->planesright[i]).yposmax >= ufo->yposmin && (game->planesright[i]).yposmax <= ufo->yposmax) ||
			((game->planesright[i]).yposmin >= ufo->yposmin && (game->planesright[i]).yposmin <= ufo->yposmax))
			&&
			(game->planesright[i]).xposmin <= ufo->xposmax+12  &&
			(game->planesright[i]).xposmax > ufo->xposmax)
			||
			((direction == 65  || direction == 97 ) &&
			(game->planesright[i]).xposmax >= ufo->xposmin-12  &&
			(game->planesright[i]).xposmin < ufo->xposmin))
		{

			planeHit = 1;
			despawn_plane(game->planesright[i], game->planesright, &(game->tailPright), map);
		}
		i++;
	}
	return planeHit;
}


int check_collision_ufoVSsoldier_horiz(int direction, MainGame *game, UFO *ufo){
	int soldierHit = 0;
	int i = 0;

	while(soldierHit == 0 && ((game->tailSleft) != &(game->soldiersleft[i])) && i < 5 ){
		if(((direction == 68  || direction == 100 ) &&
			(game->soldiersleft[i]).xposmin <= ufo->xposmax+12  &&
			(game->soldiersleft[i]).xposmax > ufo->xposmax)
			||
			((direction == 65  || direction == 97 ) &&
			(game->soldiersleft[i]).xposmax >= ufo->xposmin-12  &&
			(game->soldiersleft[i]).xposmin < ufo->xposmin))
		{

			soldierHit = 1;
			despawn_soldier(game->soldiersleft[i], game->soldiersleft, &(game->tailSleft));
		}
		i++;
	}
	i=0;
	soldierHit = 0;
	while(soldierHit == 0 && (game->tailSright) != &(game->soldiersright[i]) && i < 5 ){
		if(((direction == 68  || direction == 100 ) &&
			(game->soldiersright[i]).xposmin <= ufo->xposmax+12  &&
			(game->soldiersright[i]).xposmax > ufo->xposmax)
			||
			((direction == 65  || direction == 97 ) &&
			(game->soldiersright[i]).xposmax >= ufo->xposmin-12  &&
			(game->soldiersright[i]).xposmin < ufo->xposmin))
		{

			soldierHit = 1;
			despawn_soldier(game->soldiersright[i], game->soldiersright, &(game->tailSright));
		}
		i++;
	}
	return 0;
}


int check_collision_ufoVSsoldier_vert(MainGame *game, UFO *ufo){
	int removeSleftcounter = 0;
	int removeSrightcounter = 0;
	int mustDespawn = 0;
	Soldier *toremoveSleft[5 ];
	Soldier *toremoveSright[5 ];
	int i = 0;

	while((game->tailSleft) != &(game->soldiersleft[i]) && i < 5 ){
		if(((game->soldiersleft[i]).xposmin >= ufo->xposmin &&
			(game->soldiersleft[i]).xposmin <= ufo->xposmax)
			|| (game->soldiersleft[i]).xposmax <= ufo->xposmax &&
			(game->soldiersleft[i]).xposmax >= ufo->xposmin)
		{
			mustDespawn = 1;

			toremoveSleft[removeSleftcounter] = &(game->soldiersleft[i]);
			removeSleftcounter = removeSleftcounter + 1;
		}
		i++;
	}
	i=0;
	while((game->tailSright) != &(game->soldiersright[i]) && i < 5 ){
		if(((game->soldiersright[i]).xposmin >= ufo->xposmin &&
			(game->soldiersright[i]).xposmin <= ufo->xposmax)
			|| (game->soldiersright[i]).xposmax <= ufo->xposmax &&
			(game->soldiersright[i]).xposmax >= ufo->xposmin)
		{
			mustDespawn = 1;

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
# 546 "model.c"
void requestmovesoldiers(MainGame *game, UFO *ufo){
	int i = 0;
	int mustDespawn = 0;
	Soldier *toremoveSleft[5 ];
	Soldier *toremoveSright[5 ];
	int removeSleftcounter = 0;
	int removeSrightcounter = 0;

	if(game->beamfiring == 0){
		while((game->tailSleft != &(game->soldiersleft[i])) && i < 5 ){
			if(game->soldiersleft[i].xposmin <= 0+1 ){
				mustDespawn = 1;
				toremoveSleft[removeSleftcounter] = &(game->soldiersleft[i]);
				removeSleftcounter++;
			}
			else if(ufo->yposmax >= 342 -11  &&
					(game->soldiersleft[i]).xposmin <= ufo->xposmax+1  &&
					(game->soldiersleft[i]).xposmax > ufo->xposmax)
			{
				mustDespawn = 1;

				toremoveSleft[removeSleftcounter] = &(game->soldiersleft[i]);
				removeSleftcounter++;
			}
			else{
				move_soldier(&(game->soldiersleft[i]));
			}
			i++;
		}
		i = 0;
		while((game->tailSright != &(game->soldiersright[i])) && i < 5 ){
			if(game->soldiersright[i].xposmax >= 639-1 ){

				mustDespawn =1;
				toremoveSright[removeSrightcounter] = &(game->soldiersright[i]);
				removeSrightcounter++;
			}
			else if(ufo->yposmax >= 342 -11  &&
					(game->soldiersright[i]).xposmax >= ufo->xposmin-1  &&
					(game->soldiersright[i]).xposmin < ufo->xposmin)
			{
				mustDespawn = 1;

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
# 613 "model.c"
void requestmovemissiles(MainGame *game, UFO *ufo){
	int i = 0;
	int	mustDespawn = 0;
	int missileHit = 0;
	Missile *toremoveMleft[12 ];
	Missile *toremoveMright[12 ];
	int removeMleftcounter = 0;
	int removeMrightcounter = 0;

	while((game->tailMleft != &(game->missilesleft[i])) && i < 12 ){
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

			toremoveMleft[removeMleftcounter] = &(game->missilesleft[i]);
			removeMleftcounter++;
		}
		else{
			move_missile(&(game->missilesleft[i]));
		}
		i++;
	}
	i = 0;
	while((game->tailMright != &(game->missilesright[i])) && i < 12 ){
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
# 688 "model.c"
void requestfirebeam(MainGame *game, UFO *ufo, Soldier left[], Soldier right[], Soldier* *lefttail, Soldier* *righttail){
	int i=0;

	if(ufo->yposmin <= 170 ){
		if(ufo->BeamReady){
			firebeam(game, ufo, left, right, lefttail, righttail);
			ufo->BeamReady = 0 ;
		}
	}
}





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
# 720 "model.c"
void firebeam(MainGame *game, UFO *ufo, Soldier left[], Soldier right[], Soldier* *lefttail, Soldier* *righttail){
	int i=0;
	int removeleftcounter = 0;
	int removerightcounter = 0;
	int	mustDespawn = 0;
	Soldier *toremoveleft[5 ];
	Soldier *toremoveright[5 ];

	game->beamfiring = 1 ;
	while(*(lefttail) != &(left[i]) && i < 5 ){
		if(left[i].xposmin >= ufo->xposmin
			&& left[i].xposmax <= ufo->xposmax){
			mustDespawn = 1;

			toremoveleft[removeleftcounter] = &(left[i]);
			removeleftcounter = removeleftcounter + 1;
			ufo->score = ufo->score + 10 ;
		}
		i++;
	}
	i=0;
	while(*(righttail) != &(right[i]) && i < 5 ){
		if(right[i].xposmin >= ufo->xposmin
			&& right[i].xposmax <= ufo->xposmax){
			mustDespawn = 1;

			toremoveright[removerightcounter] = &(right[i]);
			removerightcounter = removerightcounter + 1;
			ufo->score = ufo->score + 10 ;
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
# 768 "model.c"
void stopfiring(MainGame *game){
	game->beamfiring = 0 ;
}
