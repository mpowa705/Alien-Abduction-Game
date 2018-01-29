/*Team: Matthew Powaschuk, Brady Kirchner
COMP 2659 - Winter 2016
Prof: Paul Pospisil
Project: "Alien Abduction"
*/

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 400

void plot_pixel(unsigned char* base,int x,int y);
void horizline(unsigned char* base,int x1, int x2, int y);
void vertline(unsigned char* base,int y1, int y2, int x);
void clear(unsigned long* base,int x1, int x2, int y1, int y2);
void clearscreen(unsigned long* base);
void clearvertline(unsigned char* base,int y1, int y2, int x);
unsigned int* get_videobase();
unsigned long getbase();
void set_video_base(unsigned int* base);
void setbase(unsigned long);


void plot_bitmap08(unsigned char *base, int x, int y, unsigned char *bitmap,
			unsigned int height);
void plot_bitmap16(unsigned int *base, int x, int y, unsigned int *bitmap,
			unsigned int height);
void plot_bitmap16safecurs(unsigned int *base,unsigned long* base2, int x, int y, unsigned int *bitmap,
			unsigned int height);
void save_bitmap16safe(unsigned int *base, int x, int y, unsigned int *bitmap,
			unsigned int height);
void plot_bitmap32(unsigned long *base, int x, int y, unsigned long *bitmap,
			unsigned int height);
void set_splash(unsigned char *base, unsigned char *bitmap);