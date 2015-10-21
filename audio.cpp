#include "audio.h"
#include <stdio.h>
#include <stdlib.h>
#include "pstring.h"
#include <string.h>

Music MusicList[MAX_MUSIC];
Sound SoundList[MAX_SOUNDS];
int numSounds;
int numSongs;



void InitAudio(){
	int flags=/*MIX_INIT_OGG|MIX_INIT_MOD|*/MIX_INIT_MP3;
	int initted=Mix_Init(flags);
	if((initted&flags) != flags) {
		printf("Mix_Init: Failed to init required ogg and mod support!\n");
		printf("Mix_Init: %s\n", Mix_GetError());
		// handle error
	}

	if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY,AUDIO_U16SYS,2,1024/*this affects how immediately sfx plays*/) == -1){
		fprintf(stderr, "Unable to init SDL Mixer: %s\n", Mix_GetError());
		exit(1);
	}
	if(!DEBUG)
		atexit(Mix_CloseAudio);
	if(Mix_AllocateChannels(CHANNELS) != CHANNELS){
		fprintf(stderr, "Unable to allocate enough channels: %s\n", Mix_GetError());
		exit(1);
	}
	Mix_VolumeMusic(MIX_MAX_VOLUME);
}

void PlaySound(Sound *s, bool looping){
	if(s==NULL) return;
	if(DEBUG && DEBUG_MUTE) return;
	if(SOUNDS_ON){
		int ch = -1;	// -1 finds the first available channel
		int loopcount = (looping ? -1 : 0);
		Mix_PlayChannel(ch,s->sound,loopcount);
	}
}

void PlayMusic(Music *m, bool looping){
	if(m==NULL) return;
	if(DEBUG && DEBUG_MUTE) return;
	if(MUSIC_ON){
		int loopcount = (looping ? -1 : 0);
		Mix_PlayMusic(m->song,loopcount);
	}
}

void StopSound(Sound *s){
	
}

void StopMusic(Music *m){
	if(Mix_PlayingMusic())
		Mix_HaltMusic();
}

void InitMusicList(){
	int i;
	for(i = 0;i < MAX_MUSIC;i++){
		MusicList[i].song = NULL;
		//copy_string(SoundList[i].filename,"\0");
		MusicList[i].used = 0;
		MusicList[i].volume = 0;
	}
}

Music *LoadMusic(std::string filepath,int volume){
	int i;
	/*first search to see if the requested sound is already loaded*/
	for(i = 0; i < MAX_MUSIC; i++){
		if((filepath == MusicList[i].filename)&&(MusicList[i].used >= 1)){
			MusicList[i].used++;
			return &MusicList[i];
		}
	}
	/*makesure we have the room for a new sound*/
	if(numSongs + 1 >= MAX_MUSIC)
	{
		fprintf(stderr, "Maximum Music Tracks Reached.\n");
		exit(1);
	}
	/*if its not already in memory, then load it.*/
	numSongs++;
	for(i = 0;i < MAX_MUSIC;i++){
		if(!MusicList[i].used)
			break;
	}
	MusicList[i].song = Mix_LoadMUS(filepath.c_str());
	if(MusicList[i].song == NULL){
		fprintf(stderr, "FAILED TO LOAD A VITAL SOUND: %s\n", filepath);
//		exit(4);
	}
	MusicList[i].filename = filepath;
	MusicList[i].volume = volume;
	MusicList[i].used = 1;
	Mix_VolumeMusic(volume);
	return &MusicList[i];
}

void FreeMusic(Music *music){
	if(music->used > 1){
		music->used--;
		return;
	}
	numSounds--;
	music->used--;
	Mix_FreeMusic(music->song);
	music->song = NULL;
}

void ClearMusicList(){
	int i;
	for(i = 0;i < MAX_MUSIC;i++){
		if(MusicList[i].song != NULL){
			Mix_FreeMusic(MusicList[i].song);
			MusicList[i].song = NULL;
		}
		MusicList[i].filename = "";
		MusicList[i].used = 0;
		MusicList[i].volume = 0;
	}
	numSongs = 0;
}

void InitSoundList(){
	int i;
	for(i = 0;i < MAX_SOUNDS;i++){
		SoundList[i].sound = NULL;
		SoundList[i].filename = "";
		SoundList[i].used = 0;
		SoundList[i].volume = 0;
	}
	numSounds = 0;
}

Sound *LoadSound(std::string filepath,int volume){
	int i;
	/*first search to see if the requested sound is already loaded*/
	for(i = 0; i < MAX_SOUNDS; i++){
		if((filepath == SoundList[i].filename)&&(SoundList[i].used >= 1)){

			SoundList[i].used++;
			return &SoundList[i];
		}
	}
	/*makesure we have the room for a new sound*/
	if(numSounds + 1 >= MAX_SOUNDS)
	{
		fprintf(stderr, "Maximum Sound clips Reached.\n");
		exit(1);
	}
	/*if its not already in memory, then load it.*/
	numSounds++;
	for(i = 0;i < MAX_SOUNDS;i++){
		if(!SoundList[i].used)
			break;
	}
	SoundList[i].sound = Mix_LoadWAV(filepath.c_str());
	if(SoundList[i].sound == NULL){
		fprintf(stderr, "FAILED TO LOAD A VITAL SOUND.\n");
		exit(1);
	}
	SoundList[i].filename = filepath;
	SoundList[i].volume = volume;
	SoundList[i].used = 1;
	Mix_VolumeChunk(SoundList[i].sound,volume);
	return &SoundList[i];
}

void FreeSound(Sound *sound){
	if(sound->used > 1){
		sound->used--;
		return;
	}
	numSounds--;
	sound->used--;
	Mix_FreeChunk(sound->sound);
	sound->sound = NULL;
}

void ClearSoundList(){
	int i;
	for(i = 0;i < MAX_SOUNDS;i++){
		if(SoundList[i].sound != NULL){
			Mix_FreeChunk(SoundList[i].sound);
			SoundList[i].sound = NULL;
		}
		SoundList[i].filename = "";
		SoundList[i].used = 0;
		SoundList[i].volume = 0;
	}
	numSounds = 0;
}