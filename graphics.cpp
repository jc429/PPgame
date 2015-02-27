#include <string.h>
#include <stdlib.h>
#include <stdio.h>


#include "graphics.h"
#include "tile.h"
#include "camera.h"
#include "player.h"

using namespace std;

SDL_Window *sdlWindow;			//The program window
SDL_Renderer *mainRenderer;		//The main game renderer
Camera mainCamera;				//the game's overworld camera
Camera uiCamera;				//the UI camera

extern Tile *World[WORLD_W][WORLD_H];
extern Player *player;

int framecheck;


Sprite SpriteList[MAX_SPRITES];	//A table of all sprites within the game
int NumSprites;					//How many of those sprites are being used

Uint32 NOW;		/*the current time since program started*/
	

////////////Testing stuff, feel free to delete/////////////
static Sprite *testbmp;
static Sprite *blob;
static Sprite *testbg;
//////////////////////////////////////////////////////////

void InitWindow(){
	
	mainCamera.viewport.w = GAME_RES_X;
	mainCamera.viewport.h = GAME_RES_Y;
	uiCamera.viewport.w = GAME_RES_X;
	uiCamera.viewport.h = GAME_RES_Y;

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
	SDL_SetRenderDrawColor(mainRenderer, 250, 230, 230, 255);			//Sets the bg color of the renderer
	SDL_RenderClear(mainRenderer);
	SDL_RenderPresent(mainRenderer);
	SDL_SetWindowFullscreen(sdlWindow,fullscreenFlag);
}
/*
void InitWorld(){
	world.w = WORLD_W*TILE_W;
	world.h = WORLD_H*TILE_H;

	for(int i = 0; i < WORLD_W; i++){
		for(int j = 0; j < WORLD_H; j++){
			World[i][j] = (Tile*)malloc(sizeof(Tile));
			World[i][j]->position.x = i*TILE_W;
			World[i][j]->position.y = j*TILE_H;
			World[i][j]->lowerspr = LoadSprite("sprites/grasstile.png",32,32,1);
			World[i][j]->upperspr = NULL;
			World[i][j]->height = 0;
			if((i != 0)&&(j != 0)&&(i+1 != WORLD_W)&&(j+1 != WORLD_H))
				World[i][j]->free = true;
			else{
				World[i][j]->free = false;
				World[i][j]->upperspr = LoadSprite("sprites/shade.png",32,32,1);
			}
	//		World[i][j]->lowerspr = (Sprite*)malloc(sizeof(Sprite));
		//	World[i][j]->upperspr = (Sprite*)malloc(sizeof(Sprite));
			
			World[i][j]->debugFill = LoadSprite("sprites/shade.png",32,32,1);
		}
	}
	World[2][6]->free = false;
	World[2][6]->upperspr = LoadSprite("sprites/shade.png",32,32,1);

	World[5][2]->height = 1;
	World[6][2]->height = 2;
	World[7][2]->height = 3;
	World[5][2]->upperspr = LoadSprite("sprites/shade.png",32,32,1);
	World[6][2]->upperspr = LoadSprite("sprites/shade.png",32,32,1);
	World[7][2]->upperspr = LoadSprite("sprites/shade.png",32,32,1);
}*/

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
/*
//For updating the frame of an entity's sprite 
void UpdateFrame(Entity *e){	
	e->frame += 1;
	if(e->frame >= (e->anim_length+e->anim_seed)){
	//	if(f->grounded && *f->inputs == 00000000)
			//ChangeState(f,IDLE);
	}
	if(e->frame >= (e->anim_length+e->anim_seed)){
		e->frame = ((e->frame-e->anim_seed)%e->anim_length)+e->anim_seed;
	}
}
*/

//Initializes the Sprite List
void InitSpriteList(){		
	int x;
	NumSprites = 0;
	memset(SpriteList,0,sizeof(Sprite) * MAX_SPRITES);
	for(x = 0;x < MAX_SPRITES;x++)SpriteList[x].image = NULL;
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
	if(NumSprites + 1 >= MAX_SPRITES){
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
		exit(-1);
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
int DrawSprite(Sprite* spr, int frame, Vec2i pos, Camera *c){
	if(spr==NULL) return 0;
	//	if(!RectOverlap) return; /*add a check that we're within camera bounds in once there's a gameobject system*/
	SDL_Rect targetarea;
	SDL_Rect src;
	src.x = frame%spr->framesperline * spr->w;
    src.y = frame/spr->framesperline * spr->h;
    src.w = spr->w;
    src.h = spr->h;

	targetarea.x = pos.x-c->viewport.x;
	targetarea.y = pos.y-c->viewport.y;
	targetarea.w = spr->w;
	targetarea.h = spr->h;

	

	SDL_RenderCopy(mainRenderer,spr->image,&src,&targetarea);
	
	
	if(framecheck == 1){
		frame = AdvanceFrame(spr,frame);
	}
	return frame;
}

int AdvanceFrame(Sprite *spr, int frame){
	frame++;
	frame = frame%spr->framesperline;
	return frame;
}

void DrawTile(Vec2i pos){
	SDL_Rect tile = {pos.x*TILE_W,pos.y*TILE_H,TILE_W,TILE_H}; // Just some random rect
	tile.x -= mainCamera.viewport.x;
	tile.y -= mainCamera.viewport.y;
	SDL_RenderDrawRect(mainRenderer, &tile);
}

void DrawCursor(Vec2i pos){
	SDL_Rect tile = {pos.x*TILE_W+0.4*TILE_W,pos.y*TILE_H+0.4*TILE_H,TILE_W*0.2,TILE_H*0.2}; // Just some random rect
	tile.x -= mainCamera.viewport.x;
	tile.y -= mainCamera.viewport.y;
	SDL_RenderDrawRect(mainRenderer, &tile);
}
/*
void DrawWorld(){
	int currentRow;
	int currentLayer;
	int numLayers = 2;
	for(currentRow = 0; currentRow < WORLD_H; currentRow++){
		for(currentLayer = 0; currentLayer < numLayers; currentLayer++){
			DrawRow(currentRow,currentLayer);
			if(currentLayer==0){
				if((!player->tomove.y==1)&&(player->tile.y == currentRow)){
					DrawPlayer(player);
				}
				else if((player->tomove.y==1)&&(player->tile.y+1 == currentRow)){
					DrawPlayer(player);
				}
			}
		}
	}
}


void DrawRow(int row, int layer){
	for(int col = 0; col < WORLD_W; col++){
		if (World[col][row] != NULL){
			Vec2i loc = {col*TILE_W,row*TILE_H};
			loc = World[col][row]->position;
			if(layer == 0){
				if(World[col][row]->lowerspr != NULL){
					World[col][row]->lowerframe = DrawSprite(World[col][row]->lowerspr,World[col][row]->lowerframe,loc,&mainCamera);
				}
			}else if(layer ==1){
				if(World[col][row]->upperspr != NULL){
					World[col][row]->upperframe = DrawSprite(World[col][row]->upperspr,World[col][row]->upperframe,loc,&mainCamera);
				}
			}
		}
	}
}

*/




/////////////////////////////////////////////////////////////////////////////////////////////////
//soon to be unused
void DrawTilesLower(){
	for(int i = 0; i < WORLD_W; i++){
		for(int j = 0; j < WORLD_H; j++){
			if (World[i][j] != NULL){
				if(World[i][j]->lowerspr != NULL){
			/*		fprintf(stdout,"Tile %i, %i   ",i,j);
					fprintf(stdout,World[i][j]->lowerspr->filename);
					fprintf(stdout,"   Position: %i, %i \n",World[i][j]->position.x,World[i][j]->position.y);*/
					Vec2i loc = {i*TILE_W,j*TILE_H};
					loc = World[i][j]->position;
//					fprintf(stdout,"%i, %i \n",loc.x,loc.y);
					World[i][j]->lowerframe = DrawSprite(World[i][j]->lowerspr,World[i][j]->lowerframe,loc,&mainCamera);
				}
			}
		}
	}
}
//soon to be unused
void DrawTilesUpper(){
	for(int i = 0; i < WORLD_W; i++){
		for(int j = 0; j < WORLD_H; j++){
			if (World[i][j] != NULL){
				if(World[i][j]->upperspr != NULL)
					World[i][j]->upperframe = DrawSprite(World[i][j]->upperspr,World[i][j]->upperframe,World[i][j]->position,&mainCamera);
//				if(DEBUG&&World[i][j]->free==false)
//					DrawSprite(World[i][j]->debugFill,World[i][j]->position,&mainCamera);
			}
		}
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////

