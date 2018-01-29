#ifndef MUSIC_H
#define MUSIC_H
#define SONG_LENGTH 120

typedef struct SONGt {
    unsigned int *notePtr;
    unsigned int *durationPtr;
    unsigned long timeElapsed;
}SONG;

void start_music(SONG *theSong, unsigned int* *current_note, unsigned int* *note_duration, unsigned int MUSIC[]);
void update_music(unsigned long time_elapsed, SONG *theSong, unsigned int* *current_note, unsigned int* *note_duration, unsigned int MUSIC[]);

#endif