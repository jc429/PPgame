#ifndef _AUDIO_
#define _AUDIO_

#include "audio_paths.h"
#include "global.h"

#include <string>

#include "SDL.h"
#include "SDL_mixer.h"

typedef struct{
	Mix_Chunk *sound;
	std::string filename;
	int used;
	int volume;
}Sound;

typedef struct{
	Mix_Music *song;
	std::string filename;
	int used;
	int volume;
}Music;

void InitAudio();

void PlaySound(Sound *s, bool looping = false);
void PlayMusic(Music *m, bool looping = true);
void StopSound(Sound *s);
void StopMusic(Music *m = NULL);

#define DEFAULT_VOLUME_MUSIC 50
#define DEFAULT_VOLUME_SOUNDS 40

void InitMusicList();
Music *LoadMusic(std::string filepath,int volume = DEFAULT_VOLUME_MUSIC);
void FreeMusic(Music *music); 
void ClearMusicList();
void InitSoundList();
Sound *LoadSound(std::string filepath,int volume = DEFAULT_VOLUME_SOUNDS);
void FreeSound(Sound *sound);
void ClearSoundList();


#endif