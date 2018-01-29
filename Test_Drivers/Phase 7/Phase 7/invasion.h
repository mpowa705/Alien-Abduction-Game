#ifndef INV_H
#define INV_H
#include"model.h"

#define PLANESPAWNTICKS 70
#define SOLDIERSPAWNTICKS 140
#define BEAMTICKS 20
#define BEAMREADYTICKS 245

void swapbuffers(unsigned long oldbuffer, unsigned long nextbuffer, MainGame *game);
unsigned long pollclock();
#endif