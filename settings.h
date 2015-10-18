#ifndef _SETTINGS_
#define _SETTINGS_


#include "debug.h"

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif



#define START_MODE GS_OVERWORLD

//music toggles
#define SOUNDS_ON 1
#define MUSIC_ON 1



//Window stuff
#define GAMENAME "Words. Words. Words!"				// Title of your game window
#define GAMEICON "img/bookicon.ico"	// path to a .ico file (might not have to be .ico idk try it)
#define STARTFULLSCREEN 0			// Starts the game in fullscreen if on

//GAME_RES is for the in-game resolution, WINDOW_RES is for the resolution of the actual window
#define GAME_RES_X 160
#define GAME_RES_Y 120
#define WINDOW_RES_X 640
#define WINDOW_RES_Y 480

#define MAX_CHUNK_SIZE_X 32		//maximum width (in tiles) of a single chunk
#define MAX_CHUNK_SIZE_Y 32		//maximum height (in tiles) of a single chunk
#define MAX_CHUNKS 16		//maximum chunks that can be loaded at one time

//the size of the currently game world, in tiles. Has to be big to handle very long or very tall areas
#define WORLD_W 512
#define WORLD_H	512

//the size of the game tiles (in pixels) in the world
#define TILE_W 16
#define TILE_H 16

//the maximum number of different sprites that can be loaded into the game
#define MAX_SPRITES 255
//the max animations per entity
#define MAX_ANIMS 32
//max different tiles
#define MAX_TILES 255
//the maximum number of different sprites that can be loaded into the game
#define MAX_ENTS 255
//max characters loaded at once
#define MAX_CHARACTERS 255
//max party members (4?)
#define MAX_PARTY_COMBAT 4
//max enemies per battle
#define MAX_ENEMIES 5
//max techniques per char
#define MAX_TECHS 8

//the amount of delay between one frame and the next, in milliseconds
#define FRAMEDELAY 20 // working based on 20	// 50 = 20FPS, 33 ~= 30FPS, 16 ~= 60FPS
#define FRAMESPERDRAW	6				//how many game frames to wait before updating player animations (for smooth movement but less sprites needed)

//Gameplay stuff
#define INPUTS_HISTORY 30			//number of frames of inputs to save (unused prob)
#define INPUT_BUFFER 5				//number of frames to buffer a movement input (must be smaller than inputs history)
#define MAX_MENUS 6
#define MAX_MENU_ITEMS 12
#define MAX_PROMPT_CHOICES 6
#define LOC_DEFAULT_PROMPT_X 250
#define LOC_DEFAULT_PROMPT_Y 100

//text stuff
#define FONT_RES 48
#define DEFAULT_KERNING 0

#define TEXTAREA_W 160
#define TEXTAREA_H 35
#define TEXTAREA_INSET 4
#define LINE_LENGTH 40		//max chars per textbox line
#define LINE_COUNT 4		//max lines per textbox
#define MAX_PARAGRAPH_SIZE 255	//max size a single block of text is allowed to be
#define TEXT_SPEED 1		//how many frames to wait before advancing another character in the text box

//audio
#define MAX_MUSIC 8
#define MAX_SOUNDS 64
#define CHANNELS 16



#endif