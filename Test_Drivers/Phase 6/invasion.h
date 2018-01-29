#ifndef INV_H
#define INV_H

#define PLANESPAWNTICKS 70
#define SOLDIERSPAWNTICKS 140
#define BEAMTICKS 20
#define BEAMREADYTICKS 245

char altbuffer[32256];

void swapbuffers(unsigned long oldbuffer, unsigned long nextbuffer, MainGame *game);
unsigned long pollclock();
#endif