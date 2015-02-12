#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "graphics.h"
#include "pmath.h"

using namespace std;

SDL_Window *sdlWindow;			//The program window
SDL_Renderer *mainRenderer;		//The main game renderer
SDL_Rect world;					//Rect representing a 2D game world
SDL_Rect mainCamera;			//Rect representing the game camera
SDL_Rect UICamera;				//Rect representing a UI camera (unused for now)

Sprite SpriteList[SPRITES_MAX];	//A table of all sprites within the game
int NumSprites;					//How many of those sprites are being used

Uint32 NOW;		/*the current time since program started*/
	

////////////Testing stuff, feel free to delete/////////////
static Sprite *testbmp;
static Sprite *blob;
static Sprite *testbg;
//////////////////////////////////////////////////////////

void InitWindow(){

	mainCamera.w = GAME_RES_X;
	mainCamera.h = GAME_RES_Y;

	int fullscreenFlag = 0;
	if(STARTFULLSCREEN != 0){
		fullscreenFlag = SDL_WINDOW_FULLSCREEN_DESKTOP;
	}
	sdlWindow = SDL_CreateWindow(GAMENAME,
                          SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,		//These set the window position on the desktop
                          WINDOW_RES_X, WINDOW_RES_Y,							//Change these in settings.h
                          fullscreenFlag | SDL_WINDOW_OPENGL);					//Don't touch these unless you know what you're doing
	SDL_Surface *icon = IMG_Load(GAMEICON);
	SDL_SetWindowIcon(sdlWindow,icon);
	SDL_FreeSurface(icon);	//MAKE SURE TO FREE EVERY SURFACE AFTER YOU'RE DONE USING IT - THIS LEAKS HELLA MEMORY IF YOU DON'T
	mainRenderer = SDL_CreateRenderer(sdlWindow, -1, 0);		//Creates the renderer so we can see graphics
	//SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");		//Makes the scaled rendering look smoother. (anti-aliasing)
	SDL_RenderSetLogicalSize(mainRenderer, GAME_RES_X, GAME_RES_Y);	//Sets the renderer's resolution
	SDL_SetRenderDrawColor(mainRenderer, 0, 30, 30, 255);			//Sets the bg color of the renderer
	SDL_RenderClear(mainRenderer);
	SDL_RenderPresent(mainRenderer);
}

void InitWorld(){
	world.w = WORLD_W;
	world.h = WORLD_H;
}

//Draws the current frame, then advances the game to the next frame
void NextFrame(){
//	Uint32 Then;
	SDL_RenderPresent(mainRenderer);	//Draws the frame to the screen
	SDL_RenderClear(mainRenderer);		//Clears the renderer to prepare for the next frame
//	Then = NOW;							/*these next few lines  are used to show how long each frame takes to update.  */
	NOW = SDL_GetTicks();				//Update the time elapsed since program opening (unused)
	/*fprintf(stdout,"Ticks passed this frame: %i\n", NOW - Then);*/
	FrameDelay(FRAMEDELAY);				//Change this in settings.h
}

//Keeps the framerate as consistent as possible
void FrameDelay(Uint32 delay){
    static Uint32 pass = 100;
    Uint32 dif;
    dif = SDL_GetTicks() - pass;
    if(dif < delay)
		SDL_Delay( delay - dif);
    pass = SDL_GetTicks();
}

//Initializes the Sprite List
void InitSpriteList(){		
	int x;
	NumSprites = 0;
	memset(SpriteList,0,sizeof(Sprite) * SPRITES_MAX);
	for(x = 0;x < SPRITES_MAX;x++)SpriteList[x].image = NULL;
}

/*Create a sprite from a file, the most common use for it.*/
Sprite *LoadSprite(char *filename,int sizex, int sizey, int fpl){
	int i;
	SDL_Surface *temp; 
	/*first search to see if the requested sprite image is already loaded*/
	for(i = 0; i < NumSprites; i++){
		if(strncmp(filename,SpriteList[i].filename,40)==0){
			SpriteList[i].used++;
			return &SpriteList[i];
		}
	}
	/*makesure we have the room for a new sprite*/ 
	if(NumSprites + 1 >= SPRITES_MAX){
		fprintf(stderr, "Maximum Sprites Reached.\n");
		exit(1);
	}
	/*if its not already in memory, then load it.*/
	NumSprites++;
	for(i = 0;i <= NumSprites;i++){
		if(!SpriteList[i].used)break;
	}
	temp = IMG_Load(filename);
	if(temp == NULL){
		fprintf(stderr,"unable to load a vital sprite: %s\n",SDL_GetError());
		exit(0);
	}
	SpriteList[i].image = SDL_CreateTextureFromSurface(mainRenderer,temp);
	SDL_FreeSurface(temp);
	/*sets a transparent color for blitting.*/
	//  SDL_SetColorKey(SpriteList[i].image, SDL_SRCCOLORKEY , SDL_MapRGB(SpriteList[i].image->format, 255,255,255));
	/*then copy the given information to the sprite*/
	strncpy_s(SpriteList[i].filename,filename,40);
	/*now sprites don't have to be 16 frames per line, but most will be.*/
	SpriteList[i].framesperline = fpl;
	SpriteList[i].w = sizex;
	SpriteList[i].h = sizey;
	SpriteList[i].used++;
	return &SpriteList[i];
}

//Draws a sprite to the renderer
void DrawSprite(Sprite* spr, Vec2i pos){
	//	if(!RectOverlap) return; /*add a check that we're within camera bounds in once there's a gameobject system*/
	SDL_Rect targetarea;
	targetarea.x = pos.x-mainCamera.x;
	targetarea.y = pos.y-mainCamera.y;
	targetarea.w = spr->w;
	targetarea.h = spr->h;
	SDL_RenderCopy(mainRenderer,spr->image,NULL,&targetarea);
	
}

//Maybe you want to move the camera during your game - Do that here
void UpdateCamera(){
	
}

//Function to test stuff - Feel free to delete
void InitTG(){
	blob = LoadSprite("sprites/blob.png",32,32,1);
	testbmp = LoadSprite("sprites/happi.png",16,16,1);
	testbg = LoadSprite("sprites/bg.png",640,480,1);
	
}

//Function to test stuff - Feel free to delete
void TestGraphics(int x){

	if(testbmp->image == NULL) return;
	if(testbg->image == NULL) return;
	SDL_Rect  targetarea;

	Vec2i a, b, c;
	a.x = 20;
	b.x = 5;
	a.y = 9;
	b.y = 8;
	c = a * b;
	
	targetarea.x = 0;
	targetarea.y = 0;
	targetarea.w = testbg->w;
	targetarea.h = testbg->h;

	SDL_RenderCopy(mainRenderer,testbg->image,NULL,&targetarea);
	
	DrawSprite(testbmp,c);
	DrawSprite(blob,a);
}
