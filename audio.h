#ifndef _AUDIO_
#define _AUDIO_
////look at attacker and VWP


#include "settings.h"

#include "SDL.h"
#include "SDL_mixer.h"

typedef struct{
	Mix_Chunk *sound;
	char filename[20];
	int used;
	int volume;
}Sound;

typedef struct{
	Mix_Music *song;
	char filename[20];
	int used;
	int volume;
}Music;

void InitAudio();

void PlaySound(Sound *s);
void PlayMusic(Music *m);

void InitMusicList();
Music *LoadMusic(char filename[20],int volume);
void FreeMusic(Music *music); 
void ClearMusicList();
void InitSoundList();
Sound *LoadSound(char filename[20],int volume);
void FreeSound(Sound *sound);
void ClearSoundList();

#endif