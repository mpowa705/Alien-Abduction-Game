#ifndef RENDER_H
#define RENDER_H

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
#endif
