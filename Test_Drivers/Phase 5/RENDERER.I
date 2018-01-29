# 1 "renderer.c" 1
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
	int clearbeam;
	int planespeed;
	int planespawnspeed;
	Plane *tailPleft;
	Plane *tailPright;
	Plane *tailPleftold;
	Plane *tailPrightold;
	Plane *tailPleftold2;
	Plane *tailPrightold2;
	Plane planesleft[3 +1];
	Plane planesright[3 +1];
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
# 1 "renderer.c" 2
# 1 "./rast.h" 1



void plot_pixel(unsigned char* base,int x,int y);
void horizline(unsigned char* base,int x1, int x2, int y);
void vertline(unsigned char* base,int y1, int y2, int x);
void clear(unsigned long* base,int x1, int x2, int y1, int y2);
void clearscreen(unsigned long* base);

void plot_bitmap08(unsigned char *base, int x, int y, unsigned char *bitmap,
			unsigned int height);
void plot_bitmap16(unsigned int *base, int x, int y, unsigned int *bitmap,
			unsigned int height);
void plot_bitmap32(unsigned long *base, int x, int y, unsigned long *bitmap,
			unsigned int height);
# 2 "renderer.c" 2
# 1 "./render.h" 1



void renderfirst(MainGame game);
void render(MainGame game);
void render_soldiers(unsigned char* base, Soldier toshow[],Soldier* *tail);
void clearchanging(MainGame game);
void clearsoldiers(MainGame game);
void clearplanes(MainGame game);
void render_planes(unsigned long* base, Plane toshow[],Plane* *tail);
void renderscore(int score, unsigned char* base);
void renderlives(int lives, unsigned int* base);
void renderbeam(UFO ufo, unsigned char* base);
void clearbeam(UFO ufo, unsigned long* base);
# 3 "renderer.c" 2
# 1 "./events.h" 1
# 1 "./model.h" 1
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
# 4 "renderer.c" 2
# 1 "./bitmaps.h" 1


 unsigned long PLANE_RIGHT[24] = {
		0x0007E000,
		0x00060000,
		0x00050000,
		0x00048000,
		0x00047800,
		0x00044000,
		0x20042000,
		0x30041000,
		0x28080FC0,
		0x27F00830,
		0x400007EC,
		0xC0000003,
		0x6000001C,
		0x17F007E0,
		0x18080800,
		0x10041000,
		0x00042000,
		0x00044000,
		0x00047800,
		0x00048000,
		0x00048000,
		0x00050000,
		0x00060000,
		0x0007E000
};

 unsigned long PLANE_LEFT[24] = {
		0x0007E000,
		0x00006000,
		0x0000A000,
		0x00012000,
		0x001E2000,
		0x00022000,
		0x00042004,
		0x0008200C,
		0x03F01014,
		0x0C100FE4,
		0x37E00002,
		0xC0000003,
		0x38000006,
		0x07E00FE8,
		0x00101018,
		0x00082008,
		0x00042000,
		0x00022000,
		0x001E2000,
		0x00012000,
		0x00012000,
		0x0000A000,
		0x00006000,
		0x0007E000
};
	unsigned long THEUFO[32] = {
		0x0007E000,
		0x00181800,
		0x00600600,
		0x00881100,
		0x01081080,
		0x02042040,
		0x04024020,
		0x081FF810,
		0x10200408,
		0x20466204,
		0x20400204,
		0x4043C202,
		0x40242402,
		0x80100801,
		0xFFFFFFFF,
		0x80000001,
		0x80000001,
		0x40181802,
		0x7824241E,
		0x26242464,
		0x21181884,
		0x10C18308,
		0x10224408,
		0x08124810,
		0x08099010,
		0x0F0420F0,
		0x08842110,
		0x0FE427F0,
		0x04581A20,
		0x02866140,
		0x02818140,
		0x01000080
	};

	unsigned int UFO_16[16] =
	{
		0x03E0,
		0x0810,
		0x1248,
		0x2184,
		0x43E2,
		0x4812,
		0x8811,
		0x8421,
		0xFFFF,
		0x8001,
		0x4C32,
		0x4DB2,
		0x2184,
		0x3C3C,
		0x2BD4,
		0x1008
	};



	unsigned char TINYUFO[8] =
	{
		0x3C,
		0x42,
		0x99,
		0x99,
		0xFF,
		0x42,
		0x24,
		0x5A
	};


	unsigned char SOLDIER_RIGHT[12] =
	{
		0x99,
		0x99,
		0x52,
		0x3C,
		0x18,
		0x18,
		0x18,
		0x18,
		0x1E,
		0x12,
		0xF2,
		0x83
	};

	unsigned char SOLDIER_LEFT[12] =
	{
		0x99,
		0x99,
		0x4A,
		0x3C,
		0x18,
		0x18,
		0x18,
		0x18,
		0x78,
		0x48,
		0x4F,
		0xC1
	};

	const unsigned char font[] =
	{
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x08,0x08,0x08,0x08,0x08,0x00,0x08,0x00,
	0x14,0x14,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x24,0x7E,0x24,0x24,0x7E,0x24,0x00,
	0x10,0x7C,0x50,0x7C,0x14,0x7C,0x10,0x00,
	0x00,0x62,0x64,0x08,0x10,0x26,0x46,0x00,
	0x00,0x10,0x28,0x10,0x2A,0x44,0x3A,0x00,
	0x00,0x08,0x10,0x00,0x00,0x00,0x00,0x00,
	0x00,0x08,0x10,0x10,0x10,0x10,0x08,0x00,
	0x00,0x10,0x08,0x08,0x08,0x08,0x10,0x00,
	0x00,0x00,0x28,0x10,0x7C,0x10,0x28,0x00,
	0x00,0x00,0x10,0x10,0x7C,0x10,0x10,0x00,
	0x00,0x00,0x00,0x00,0x00,0x08,0x08,0x10,
	0x00,0x00,0x00,0x00,0x7C,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x00,
	0x00,0x00,0x04,0x08,0x10,0x20,0x40,0x00,
	0x00,0x78,0x8C,0x94,0xA4,0xC4,0x78,0x00,
	0x00,0x60,0xA0,0x20,0x20,0x20,0xF8,0x00,
	0x00,0x78,0x84,0x04,0x78,0x80,0xFC,0x00,
	0x00,0x78,0x84,0x18,0x04,0x84,0x78,0x00,
	0x00,0x10,0x30,0x50,0x90,0xFC,0x10,0x00,
	0x00,0xFC,0x80,0xF8,0x04,0x84,0x78,0x00,
	0x00,0x78,0x80,0xF8,0x84,0x84,0x78,0x00,
	0x00,0xFC,0x04,0x08,0x10,0x20,0x20,0x00,
	0x00,0x78,0x84,0x78,0x84,0x84,0x78,0x00,
0x00,0x78,0x84,0x84,0x7C,0x04,0x78,0x00,
0x00,0x00,0x00,0x10,0x00,0x00,0x10,0x00,
0x00,0x00,0x10,0x00,0x00,0x10,0x10,0x20,
0x00,0x00,0x08,0x10,0x20,0x10,0x08,0x00,
0x00,0x00,0x00,0x7C,0x00,0x7C,0x00,0x00,
0x00,0x00,0x20,0x10,0x08,0x10,0x20,0x00,
0x00,0x3C,0x42,0x04,0x08,0x00,0x08,0x00,

0x00,0x3C,0x4A,0x56,0x5E,0x40,0x3C,0x00,
0x00,0x78,0x84,0x84,0xFC,0x84,0x84,0x00,
0x00,0xF8,0x84,0xF8,0x84,0x84,0xF8,0x00,
0x00,0x78,0x84,0x80,0x80,0x84,0x78,0x00,
0x00,0xF0,0x88,0x84,0x84,0x88,0xF0,0x00,
0x00,0xFC,0x80,0xF8,0x80,0x80,0xFC,0x00,
0x00,0xFC,0x80,0xF8,0x80,0x80,0x80,0x00,
0x00,0x78,0x84,0x80,0x9C,0x84,0x78,0x00,
0x00,0x84,0x84,0xFC,0x84,0x84,0x84,0x00,
0x00,0x7C,0x10,0x10,0x10,0x10,0x7C,0x00,
0x00,0x04,0x04,0x04,0x84,0x84,0x78,0x00,
0x00,0x88,0x90,0xE0,0x90,0x88,0x84,0x00,
0x00,0x80,0x80,0x80,0x80,0x80,0xFC,0x00,
0x00,0x84,0xCC,0xB4,0x84,0x84,0x84,0x00,
0x00,0x84,0xC4,0xA4,0x94,0x8C,0x84,0x00,
0x00,0x78,0x84,0x84,0x84,0x84,0x78,0x00,

0x00,0xF8,0x84,0x84,0xF8,0x80,0x80,0x00,
0x00,0x78,0x84,0x84,0xA4,0x94,0x78,0x00,
0x00,0xF8,0x84,0x84,0xF8,0x88,0x84,0x00,
0x00,0x78,0x80,0x78,0x04,0x84,0x78,0x00,
0x00,0xFE,0x10,0x10,0x10,0x10,0x10,0x00,
0x00,0x84,0x84,0x84,0x84,0x84,0x78,0x00,
0x00,0x84,0x84,0x84,0x84,0x48,0x30,0x00,
0x00,0x84,0x84,0x84,0x84,0xB4,0x48,0x00,
0x00,0x84,0x48,0x30,0x30,0x48,0x84,0x00,
0x00,0x82,0x44,0x28,0x10,0x10,0x10,0x00,
0x00,0xFC,0x08,0x10,0x20,0x40,0xFC,0x00,
0x00,0x38,0x20,0x20,0x20,0x20,0x38,0x00,
0x00,0x00,0x40,0x20,0x10,0x08,0x04,0x00,
0x00,0x38,0x08,0x08,0x08,0x08,0x38,0x00,
0x00,0x10,0x28,0x44,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,

0x3C,0x42,0x99,0xA1,0xA1,0x99,0x42,0x3C,
0x00,0x00,0x38,0x04,0x3C,0x44,0x3C,0x00,
0x00,0x40,0x40,0x78,0x44,0x44,0x78,0x00,
0x00,0x00,0x1C,0x20,0x20,0x20,0x1C,0x00,
0x00,0x04,0x04,0x3C,0x44,0x44,0x3C,0x00,
0x00,0x00,0x38,0x44,0x78,0x40,0x3C,0x00,
0x00,0x0C,0x10,0x18,0x10,0x10,0x10,0x00,
0x00,0x00,0x3E,0x42,0x42,0x3E,0x02,0x3C,
0x00,0x40,0x40,0x78,0x44,0x44,0x44,0x00,
0x00,0x08,0x00,0x18,0x08,0x08,0x1C,0x00,
0x00,0x04,0x00,0x04,0x04,0x04,0x24,0x18,
0x00,0x40,0x50,0x60,0x60,0x50,0x48,0x00,
0x00,0x10,0x10,0x10,0x10,0x10,0x0C,0x00,
0x00,0x00,0x68,0x54,0x54,0x54,0x54,0x00,
0x00,0x00,0x78,0x44,0x44,0x44,0x44,0x00,
0x00,0x00,0x38,0x44,0x44,0x44,0x38,0x00,

0x00,0x00,0x78,0x44,0x44,0x78,0x40,0x40,
0x00,0x00,0x3C,0x44,0x44,0x3C,0x04,0x06,
0x00,0x00,0x1C,0x20,0x20,0x20,0x20,0x00,
0x00,0x00,0x38,0x40,0x38,0x04,0x78,0x00,
0x00,0x10,0x38,0x10,0x10,0x10,0x0C,0x00,
0x00,0x00,0x44,0x44,0x44,0x44,0x38,0x00,
0x00,0x00,0x44,0x44,0x28,0x28,0x10,0x00,
0x00,0x00,0x44,0x54,0x54,0x54,0x28,0x00,
0x00,0x00,0x44,0x28,0x10,0x28,0x44,0x00,
0x00,0x00,0x44,0x44,0x44,0x3C,0x04,0x38,
0x00,0x00,0x7C,0x08,0x10,0x20,0x7C,0x00,
0x00,0x1C,0x10,0x60,0x10,0x10,0x1C,0x00,
0x00,0x10,0x10,0x10,0x10,0x10,0x10,0x00,
0x00,0x70,0x10,0x0C,0x10,0x10,0x70,0x00,
0x00,0x14,0x28,0x00,0x00,0x00,0x00,0x00
};
# 5 "renderer.c" 2
# 1 "C:/c68/include/stdio.h" 1
# 1 "C:/c68/include/compiler.h" 1
# 11 "C:/c68/include/stdio.h" 2
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
# 6 "renderer.c" 2
# 1 "C:/c68/include/osbind.h" 1
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
# 7 "renderer.c" 2



void render(MainGame game)
{
	clearchanging(game);
	plot_bitmap32(game.world.base32, game.player.xposmin, (game.player.yposmin + 51), THEUFO,
		32);
	render_soldiers(game.world.base08, game.soldiersright,&(game.tailSright));
	render_soldiers(game.world.base08, game.soldiersleft,&(game.tailSleft));

	render_planes(game.world.base32, game.planesright,&(game.tailPright));
	render_planes(game.world.base32, game.planesleft,&(game.tailPleft));
	renderscore(game.player.score,game.world.base08);
	renderlives(game.player.lives,game.world.base16);
	if(game.beamfiring == 1 )
	{
		renderbeam(game.player, game.world.base08);
	}
	else if(game.clearbeam == 1 )
	{
		clearbeam(game.player,game.world.base32);
	}

}

void clearbeam(UFO ufo, unsigned long* base){
		clear(base,ufo.xposold,ufo.xposold+31,ufo.yposold+51,342 +49);
}
void renderbeam(UFO ufo, unsigned char* base)
{
	vertline(base,ufo.yposmax+51,342 +50,ufo.xposmin);
	vertline(base,ufo.yposmax+51,342 +50,ufo.xposmax);
}

void renderfirst(MainGame game){
	clearscreen(game.world.base32);
	horizline(game.world.base08,0,149,0);
	horizline(game.world.base08,0,149,48);
	vertline(game.world.base08,0, 48, 149);
	vertline(game.world.base08,0, 48, 0);
	vertline(game.world.base08,49, 399, 0);
	vertline(game.world.base08,49, 399, 639);
	horizline(game.world.base08,0,639,50);
	horizline(game.world.base08,0,639,392);
	horizline(game.world.base08,0,639,399);
	plot_bitmap08(game.world.base08,4,4,font+(8*48),8);
	plot_bitmap08(game.world.base08,14,4,font+(8*44),8);
	plot_bitmap08(game.world.base08,24,4,font+(8*33),8);
	plot_bitmap08(game.world.base08,34,4,font+(8*57),8);
	plot_bitmap08(game.world.base08,44,4,font+(8*37),8);
	plot_bitmap08(game.world.base08,54,4,font+(8*50),8);
	plot_bitmap08(game.world.base08,70,4,font+(8*17),8);
	plot_bitmap08(game.world.base08,4,20,font+(8*51),8);
	plot_bitmap08(game.world.base08,14,20,font+(8*35),8);
	plot_bitmap08(game.world.base08,24,20,font+(8*47),8);
	plot_bitmap08(game.world.base08,34,20,font+(8*50),8);
	plot_bitmap08(game.world.base08,44,20,font+(8*37),8);
	plot_bitmap08(game.world.base08,54,20,font+(8*26),8);
	plot_bitmap08(game.world.base08,4,36,font+(8*44),8);
	plot_bitmap08(game.world.base08,14,36,font+(8*41),8);
	plot_bitmap08(game.world.base08,24,36,font+(8*54),8);
	plot_bitmap08(game.world.base08,34,36,font+(8*37),8);
	plot_bitmap08(game.world.base08,44,36,font+(8*51),8);
	plot_bitmap08(game.world.base08,54,36,font+(8*26),8);
	plot_bitmap32(game.world.base32, game.player.xposmin, (game.player.yposmin + 51), THEUFO,
		32);
	render_soldiers(game.world.base08, game.soldiersright,&(game.tailSright));
	render_soldiers(game.world.base08, game.soldiersleft,&(game.tailSleft));
	render_planes(game.world.base32, game.planesright,&(game.tailPright));
	render_planes(game.world.base32, game.planesleft,&(game.tailPleft));
	renderscore(game.player.score,game.world.base08);
	renderlives(game.player.lives,game.world.base16);
}

void renderlives(int lives, unsigned int* base)
{
	int i = 0;
	while(i < lives)
	{
		plot_bitmap16(base,70+(i * 20),32,UFO_16,16);
		i++;
	}
}

void renderscore(int score, unsigned char* base)
{
	int toprint = 0;
	int i = 0;
	if(score == 0)
		plot_bitmap08(base,100-(9*i),20,font+(8*(toprint+16)),8);
	else if(score < 10000)
	{
		while(score >= 10)
		{
			toprint = score % 10;
			score = score / 10;
			plot_bitmap08(base,100-(9*i),20,font+(8*(toprint+16)),8);
			i++;
		}
		toprint = score % 10;
		score = score / 10;
		plot_bitmap08(base,100-(9*i),20,font+(8*(toprint+16)),8);
	}
}

void clearchanging(MainGame game)
{
	clear(game.world.base32,70,125,32,47);
	clear(game.world.base32,73,107,20,27);
	clear(game.world.base32,game.player.xposold,game.player.xposold+31,game.player.yposold+51,game.player.yposold+31+51);
	clearsoldiers(game);
	clearplanes(game);
}

void clearplanes(MainGame game)
{
	int i = 0;

	if(game.tailPrightold != ((void *)0) )
	{
		while((game.tailPrightold->yposold != game.planesright[i].yposold))
		{
			clear(game.world.base32,game.planesright[i].xposold,game.planesright[i].xposold+31,
			game.planesright[i].yposold +51,game.planesright[i].yposold+23+51);
			i++;
		}
	}
	else
	{
		while(i < 3 )
		{
			clear(game.world.base32,game.planesright[i].xposold,game.planesright[i].xposold+31,
			game.planesright[i].yposold +51,game.planesright[i].yposold+23+51);
			i++;
		}
	}
	i = 0;
	if(game.tailPleftold != ((void *)0) )
	{
		while((game.tailPleftold->yposold != game.planesleft[i].yposold))
		{
			clear(game.world.base32,game.planesleft[i].xposold,game.planesleft[i].xposold+31,
			game.planesleft[i].yposold +51,game.planesleft[i].yposold+23+51);
			i++;
		}
	}
	else
	{
		while(i < 3 )
		{
			clear(game.world.base32,game.planesleft[i].xposold,game.planesleft[i].xposold+31,
			game.planesleft[i].yposold +51,game.planesleft[i].yposold+23+51);
			i++;
		}
	}
}
void clearsoldiers(MainGame game)
{
	int i = 0;
	if(game.tailSrightold != ((void *)0) )
	{
		while((game.tailSrightold->xposold != game.soldiersright[i].xposold))
		{
			clear(game.world.base32,game.soldiersright[i].xposold,game.soldiersright[i].xposold+7,381,391);
			i++;
		}
	}
	else
	{
		while(i<5 )
		{
			clear(game.world.base32,game.soldiersright[i].xposold,game.soldiersright[i].xposold+7,381,391);
			i++;
		}
	}
	i = 0;
	if(game.tailSleftold != ((void *)0) )
	{
		while((game.tailSleftold->xposold != game.soldiersleft[i].xposold))
		{
			clear(game.world.base32,game.soldiersleft[i].xposold,game.soldiersleft[i].xposold+7,381,391);
			i++;
		}
	}
	else
	{
		while(i<5 )
		{
			clear(game.world.base32,game.soldiersleft[i].xposold,game.soldiersleft[i].xposold+7,381,391);
			i++;
		}
	}
}

void render_planes(unsigned long* base, Plane toshow[],Plane* *tail)
{
	int i = 0;
	if(*(tail) != ((void *)0) )
	{
		while((*(*tail)).yposmin != toshow[i].yposmin)
		{
			if(toshow[i].lefttoright == 1 )
			{
				plot_bitmap32(base,toshow[i].xposmin,toshow[i].yposmin+51,PLANE_RIGHT,24);
				i = i+1;
			}
			else
			{
				plot_bitmap32(base,toshow[i].xposmin,toshow[i].yposmin+51,PLANE_LEFT,24);
				i = i+1;
			}
		}
	}
	else
	{
		while(i < 3 )
		{
		if(toshow[i].lefttoright == 1 )
			{
				plot_bitmap32(base,toshow[i].xposmin,toshow[i].yposmin+51,PLANE_RIGHT,24);
				i = i+1;
			}
			else
			{
				plot_bitmap32(base,toshow[i].xposmin,toshow[i].yposmin+51,PLANE_LEFT,24);
				i = i+1;
			}
		}
	}
}

void render_soldiers(unsigned char* base, Soldier toshow[],Soldier* *tail)
{
	int i = 0;
	if(*(tail) != ((void *)0) )
	{
		while((*(*tail)).xposmin != toshow[i].xposmin)
		{
			if(toshow[i].lefttoright == 1 )
			{
			plot_bitmap08(base,toshow[i].xposmin,381,SOLDIER_RIGHT,12);
			i = i+1;
			}
			else
			{
			plot_bitmap08(base,toshow[i].xposmin,381,SOLDIER_LEFT,12);
			i = i+1;
			}
		}
	}
	else
	{
		while(i < 5 )
		{
			if(toshow[i].lefttoright == 1 )
			{
			plot_bitmap08(base,toshow[i].xposmin,381,SOLDIER_RIGHT,12);
			i = i+1;
			}
			else
			{
			plot_bitmap08(base,toshow[i].xposmin,381,SOLDIER_LEFT,12);
			i = i+1;
			}
		}
	}
}
