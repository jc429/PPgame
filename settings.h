
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

//the size of the game world, in pixels for now. this doesn't really affect anything yet.
#define WORLD_W 1280
#define WORLD_H	960

//the maximum number of different sprites that can be loaded into the game
#define SPRITES_MAX 255

//the amount of delay between one frame and the next, in milliseconds
#define FRAMEDELAY 50				// 50 = 20FPS, 33 ~= 30FPS, 16 ~= 60FPS
