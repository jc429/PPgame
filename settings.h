
//debug mode toggle
#define DEBUG 1						//PROTIP: wrap any code you want to execute in if(DEBUG){<code>} and set this to 1 or 0 as necessary  

#define FORCE_DEBUG_LEVEL 0

#define DEBUG_START_MODE COMBAT
/*{
	MAIN_MENU,
	OPTIONS,
	OVERWORLD,
	COMBAT,				
	DIALOGUE_OVERLAY,	//Maybe unnecessary?
	DIALOGUE_FULL,
	DATE			
}*/

#define START_MODE OVERWORLD

//music toggles
#define SOUNDS_ON 1
#define MUSIC_ON 1



//Window stuff
#define GAMENAME "Bone Me Up, Scotty"				// Title of your game window
#define GAMEICON "img/fieyerrr.ico"	// path to a .ico file (might not have to be .ico idk try it)
#define STARTFULLSCREEN 0			// Starts the game in fullscreen if on

//GAME_RES is for the in-game resolution, WINDOW_RES is for the resolution of the actual window
#define GAME_RES_X 320
#define GAME_RES_Y 240
#define WINDOW_RES_X 640
#define WINDOW_RES_Y 480

//the size of the game world, in tiles for now. this doesn't really affect anything yet.
#define WORLD_W 32
#define WORLD_H	24

//the size of the game tiles in the world
#define TILE_W 32
#define TILE_H 32

//the maximum number of different sprites that can be loaded into the game
#define MAX_SPRITES 255
//max different tiles
#define MAX_TILES 255
//the maximum number of different sprites that can be loaded into the game
#define MAX_ENTS 255

//the amount of delay between one frame and the next, in milliseconds
#define FRAMEDELAY 16 //33				// 50 = 20FPS, 33 ~= 30FPS, 16 ~= 60FPS
#define FRAMESPERDRAW	15				//how many game frames to wait before updating player animations (for smooth movement but less sprites needed)

//Gameplay stuff
#define INPUTS_HISTORY 60			//number of frames of inputs to save (unused prob)
#define INPUT_BUFFER 10				//number of frames to buffer a movement input (must be smaller than inputs history)

//text stuff
#define FONT_RES 48
#define TEXTAREA_W 320
#define TEXTAREA_H 80
#define TEXTAREA_INSET 4
#define LINE_LENGTH 40		//num chars per textbox line
#define LINE_COUNT 4		//num lines per textbox
#define TEXT_SPEED 1		//how many frames to wait before advancing another character in the text box

//audio
#define MAX_MUSIC 8
#define MAX_SOUNDS 64
#define CHANNELS 16


//Game inputs 
#define PPKEY_LEFT SDL_SCANCODE_A
#define PPKEY_RIGHT SDL_SCANCODE_D
#define PPKEY_UP SDL_SCANCODE_W
#define PPKEY_DOWN SDL_SCANCODE_S
#define PPKEY_A SDL_SCANCODE_SPACE
#define PPKEY_B SDL_SCANCODE_LSHIFT
#define PPKEY_X SDL_SCANCODE_RSHIFT
#define PPKEY_Y SDL_SCANCODE_RETURN
#define PPKEY_PAUSE SDL_SCANCODE_P

#define PPINPUT_LEFT	1<<7
#define PPINPUT_RIGHT	1<<6
#define PPINPUT_UP		1<<5
#define PPINPUT_DOWN	1<<4
#define PPINPUT_A		1<<3
#define PPINPUT_B		1<<2
#define PPINPUT_X		1<<1
#define PPINPUT_Y		1
#define PPINPUT_FACE_ANY PPINPUT_A|PPINPUT_B|PPINPUT_X|PPINPUT_Y
#define PPINPUT_DIR_ANY PPINPUT_UP|PPINPUT_DOWN|PPINPUT_LEFT|PPINPUT_RIGHT