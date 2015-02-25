
//debug mode toggle
#define DEBUG 1						//PROTIP: wrap any code you want to execute in if(DEBUG){<code>} and set this to 1 or 0 as necessary  

//Window stuff
#define GAMENAME "pow"				// Title of your game window
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
#define ENTS_MAX 255

//the amount of delay between one frame and the next, in milliseconds
#define FRAMEDELAY 16 //33				// 50 = 20FPS, 33 ~= 30FPS, 16 ~= 60FPS
#define FRAMESPERDRAW	15				//how many game frames to wait before updating player animations (for smooth movement but less sprites needed)

//Gameplay stuff
#define INPUTS_HISTORY 60			//number of frames of inputs to save (unused prob)
#define INPUT_BUFFER 10				//number of frames to buffer a movement input (must be smaller than inputs history)

//Game inputs 
#define INPUT_LEFT SDL_SCANCODE_A
#define INPUT_RIGHT SDL_SCANCODE_D
#define INPUT_UP SDL_SCANCODE_W
#define INPUT_DOWN SDL_SCANCODE_S
#define INPUT_A SDL_SCANCODE_SPACE
#define INPUT_B SDL_SCANCODE_LSHIFT
#define INPUT_X SDL_SCANCODE_RSHIFT
#define INPUT_Y SDL_SCANCODE_RETURN
#define INPUT_PAUSE SDL_SCANCODE_P

//text stuff
#define FONT_RES 48
#define TEXTAREA_W 320
#define TEXTAREA_H 80
#define TEXTAREA_INSET 4