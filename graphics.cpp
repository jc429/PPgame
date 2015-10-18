#include <string.h>
#include <stdlib.h>
#include <stdio.h>


#include "graphics.h"
#include "tile.h"
#include "camera.h"
#include "player.h"

//using namespace std;

SDL_Window *sdlWindow;			//The program window
SDL_Renderer *mainRenderer;		//The main game renderer
Camera mainCamera;				//the game's overworld camera
Camera uiCamera;				//the UI camera
Camera combatCamera;			//battle camea

extern Tile *World[WORLD_W][WORLD_H];
extern OverworldCharacter *_Player;

int framecheck;


Sprite SpriteList[MAX_SPRITES];	//A table of all sprites within the game
int NumSprites;					//How many of those sprites are being used

Uint32 NOW;		/*the current time since program started*/
	

////////////Testing stuff, feel free to delete/////////////
static Sprite *testbmp;
static Sprite *blob;
static Sprite *testbg;
//////////////////////////////////////////////////////////


/////////~~~~~~~WINDOW AND RENDERER AND FRAME STUFF~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

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
	SDL_SetRenderDrawColor(mainRenderer, 0, 0, 0, 255);			//Sets the bg color of the renderer
	SDL_RenderClear(mainRenderer);
	SDL_RenderPresent(mainRenderer);
	SDL_SetWindowFullscreen(sdlWindow,fullscreenFlag);
}

//Draws the current frame
void Graphics::RenderCurrentFrame(){
	SDL_RenderPresent(mainRenderer);	//Draws the frame to the screen
	
	SDL_SetRenderDrawColor(mainRenderer, 0, 0, 0, 255);			//resets the bg color of the renderer
	SDL_RenderClear(mainRenderer);		//Clears the renderer to prepare for the next frame
}
//advances the game to the next frame
void Graphics::NextFrame(){
//	Uint32 Then;
//	Then = NOW;							/*these next few lines  are used to show how long each frame takes to update.  */
	NOW = SDL_GetTicks();				//Update the time elapsed since program opening (unused)
	/*fprintf(stdout,"Ticks passed this frame: %i\n", NOW - Then);*/
	FrameDelay(FRAMEDELAY);				//Change this in settings.h
}

//Keeps the framerate as consistent as possible
void Graphics::FrameDelay(Uint32 delay){
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

/////~~~~~~~~~~~SPRITES & ANIMATIONS~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//Initializes the Sprite List
void InitSpriteList(){		
	int x;
	NumSprites = 0;
	memset(SpriteList,0,sizeof(Sprite) * MAX_SPRITES);
	for(x = 0;x < MAX_SPRITES;x++)SpriteList[x].image = NULL;
}

/*Create a sprite from a file, the most common use for it.*/
Sprite *LoadSprite(char *filename,int sizex, int sizey, int fpl, int off_x, int off_y){
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
	strncpy(SpriteList[i].filename,filename,40);
	/*now sprites don't have to be 16 frames per line, but most will be.*/
	SpriteList[i].framesperline = fpl;
	SpriteList[i].w = sizex;
	SpriteList[i].h = sizey;
	SpriteList[i].s_offset_x = off_x;
	SpriteList[i].s_offset_y = off_y;
	SpriteList[i].used++;
	return &SpriteList[i];
}

Sprite *GetSprite(char *filename){
	for(int i = 0; i < NumSprites; i++){
		if(strncmp(filename,SpriteList[i].filename,40)==0){
			SpriteList[i].used++;
			return &SpriteList[i];
		}
	}
	printf("sprite not found :( \n");
	return NULL;
}

void FreeSprite(Sprite *spr){
	if(spr==NULL) return;
	spr->used--;
	if(spr->used <= 0){
		copy_string(spr->filename,"\0");
		if(spr->image != NULL)
			SDL_DestroyTexture(spr->image);
		spr->image = NULL;
	}
}

int AdvanceFrame(Sprite *spr, int frame){  //placeholder
	frame++;
	frame = frame%spr->framesperline;
	return frame;
}
////
Animation *LoadAnimation(Sprite *spr, int curFrame, int seed, int len, bool play, bool loop, int delay, void(*finish)()){
	Animation *anim = new Animation;
	anim->sprite = spr;
	anim->curFrame = curFrame;
	anim->seed = seed;
	anim->length = len;
	anim->playing = play;
	anim->looping = loop;
	anim->onFinish = finish;
	anim->delay = delay;

	anim->rotation = 0;
	anim->mirror.x = 1;
	anim->mirror.y = 1;

	return anim;
}

void FreeAnimation(Animation *a){
	if(a == NULL) return;
	FreeSprite(a->sprite);
	delete a;
}

void AdvanceAnimFrame(Animation *a){
	if(a->playing==0) return;
	a->curFrame++;
	if(a->curFrame >= (a->seed + a->length)){
		if(a->onFinish != NULL)
			a->onFinish;
		if(a->looping)
			a->curFrame -= a->length;
		else
			a->curFrame --;
		
	}
/*	if(a->looping!=0){
		a->curFrame++;
		if(a->curFrame >= (a->seed + a->length))
			a->curFrame -= a->length;

	}else{
		if(a->curFrame+1 < (a->length+a->seed))
			a->curFrame++;
	}*/
}

//~~~~~~~~~~~~~~YAR THERE BE DRAWIN' BELOW HERE~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//Draws a sprite to the renderer
int DrawSprite(Sprite* spr, int frame, Vec2i pos, Camera *c){
	if(spr == NULL) return 0;
	if(spr->image == NULL) return 0;

	SDL_Rect targetarea;
	SDL_Rect src;
	src.x = frame%spr->framesperline * spr->w;
    src.y = frame/spr->framesperline * spr->h;
    src.w = spr->w;
    src.h = spr->h;

	targetarea.x = (pos.x - spr->s_offset_x) - c->viewport.x;
	targetarea.y = (pos.y - spr->s_offset_y) - c->viewport.y;
	targetarea.w = src.w;
	targetarea.h = src.h;
	
//	if(RectTouch(targetarea,c->viewport))		//only draw whats actually within the camera
		SDL_RenderCopy(mainRenderer,spr->image,&src,&targetarea);

	return frame;
}

int DrawPartialSprite(Sprite* spr, int frame, SDL_Rect subrect, Vec2i pos, Camera *c){
	if(spr == NULL) return 0;
	if(spr->image == NULL) return 0;

	SDL_Rect targetarea;
	SDL_Rect src;
	src.x = (frame%spr->framesperline * spr->w) + subrect.x;
	src.y = (frame/spr->framesperline * spr->h) + subrect.y;
    src.w = subrect.w;
    src.h = subrect.h;
	if(src.x < 0){			//if we're past the left margin of the spritesheet
		src.w += src.x;		//crop the rectangle
		src.x = 0;
	}
	if(src.y < 0){			//if we're past the upper margin of the spritesheet
		src.h += src.y;		
		src.y = 0;
	}

	targetarea.x = (pos.x - spr->s_offset_x) - c->viewport.x;
	targetarea.y = (pos.y - spr->s_offset_y) - c->viewport.y;
	targetarea.w = src.w;
	targetarea.h = src.h;
	
//	if(RectTouch(targetarea,c->viewport))		//only draw whats actually within the camera
		SDL_RenderCopy(mainRenderer,spr->image,&src,&targetarea);

	return frame;

}

void DrawAnimation(Animation *anim, Vec2i pos, Camera *c){
	if(anim==NULL) return;
	if(anim->sprite==NULL) return;
	SDL_RendererFlip flip;
	SDL_Rect targetarea;
	SDL_Rect src;

	flip = SDL_FLIP_NONE;
	if(anim->mirror.x!=1)
		flip = (SDL_RendererFlip)(flip|SDL_FLIP_HORIZONTAL);
	if(anim->mirror.y!=1)
		flip = (SDL_RendererFlip)(flip|SDL_FLIP_VERTICAL);


	src.x = anim->curFrame % anim->sprite->framesperline * anim->sprite->w;
    src.y = anim->curFrame / anim->sprite->framesperline * anim->sprite->h;
	src.w = anim->sprite->w;
	src.h = anim->sprite->h;


	targetarea.x = (pos.x - anim->sprite->s_offset_x);
	targetarea.y = (pos.y - anim->sprite->s_offset_y);
	targetarea.w = src.w;
	targetarea.h = src.h;

	if(c != NULL){
		targetarea.x -= c->viewport.x;
		targetarea.y -= c->viewport.y;
	}

	if(framecheck%anim->delay == 0){
		AdvanceAnimFrame(anim);
	}
//	if(RectTouch(targetarea,c->viewport))		//only draw whats actually within the camera
	//	SDL_RenderCopy(mainRenderer,anim->sprite->image,&src,&targetarea);
	
		SDL_RenderCopyEx(mainRenderer,anim->sprite->image,&src,&targetarea,0,NULL,flip);
}
void DrawPartialAnimation(Animation *anim, SDL_Rect subrect, Vec2i pos, Camera *c){
	if(anim==NULL) return;
	if(anim->sprite==NULL) return;
	SDL_RendererFlip flip;
	SDL_Rect targetarea;
	SDL_Rect src;

	flip = SDL_FLIP_NONE;
	if(anim->mirror.x!=1)
		flip = (SDL_RendererFlip)(flip|SDL_FLIP_HORIZONTAL);
	if(anim->mirror.y!=1)
		flip = (SDL_RendererFlip)(flip|SDL_FLIP_VERTICAL);


	src.x = (anim->curFrame % anim->sprite->framesperline * anim->sprite->w) + subrect.x;
	src.y = (anim->curFrame / anim->sprite->framesperline * anim->sprite->h) + subrect.y;
	src.w = subrect.w;
	src.h = subrect.h;
	if(src.x < 0){			//if we're past the left margin of the spritesheet
		src.w += src.x;		//crop the rectangle
		src.x = 0;
	}
	if(src.y < 0){			//if we're past the upper margin of the spritesheet
		src.h += src.y;		
		src.y = 0;
	}
	
	targetarea.x = (pos.x - anim->sprite->s_offset_x);
	targetarea.y = (pos.y - anim->sprite->s_offset_y);
	targetarea.w = src.w;
	targetarea.h = src.h;

	if(c != NULL){
		targetarea.x -= c->viewport.x;
		targetarea.y -= c->viewport.y;
	}

	if(framecheck%anim->delay == 0){
		AdvanceAnimFrame(anim);
	}
//	if(RectTouch(targetarea,c->viewport))		//only draw whats actually within the camera
	//	SDL_RenderCopy(mainRenderer,anim->sprite->image,&src,&targetarea);
	
		SDL_RenderCopyEx(mainRenderer,anim->sprite->image,&src,&targetarea,0,NULL,flip);
}
void DrawBGAnimation(Animation *anim,Camera *c){	//draws a tiled bg across the back of the screen
	if(anim==NULL) return;
	if(anim->sprite==NULL) return;
	SDL_RendererFlip flip;
	SDL_Rect targetarea;
	SDL_Rect src;

	flip = SDL_FLIP_NONE;
	if(anim->mirror.x!=1)
		flip = (SDL_RendererFlip)(flip|SDL_FLIP_HORIZONTAL);
	if(anim->mirror.y!=1)
		flip = (SDL_RendererFlip)(flip|SDL_FLIP_VERTICAL);
	
	src.x = anim->curFrame % anim->sprite->framesperline * anim->sprite->w;
    src.y = anim->curFrame / anim->sprite->framesperline * anim->sprite->h;
	src.w = anim->sprite->w;
	src.h = anim->sprite->h;
	targetarea.w = src.w;
	targetarea.h = src.h;
	Vec2i tilepos(0,0);
	while(tilepos.y < GAME_RES_Y){
		while(tilepos.x < GAME_RES_X){

			//draw the animation frame here

			targetarea.x = (tilepos.x - anim->sprite->s_offset_x);
			targetarea.y = (tilepos.y - anim->sprite->s_offset_y);
			if(c != NULL){
				targetarea.x -= c->viewport.x;
				targetarea.y -= c->viewport.y;
			}
			SDL_RenderCopyEx(mainRenderer,anim->sprite->image,&src,&targetarea,0,NULL,flip);
			tilepos.x += anim->sprite->w;
		}
		tilepos.x = 0;
		tilepos.y += anim->sprite->h;
	}
	if(framecheck%anim->delay == 0){
		AdvanceAnimFrame(anim);
	}
}
void DrawAnimFrame(Animation *anim, Vec2i pos, Camera *c){	//identical to drawanimation but does not advance the frame
	if(anim==NULL) return;
	if(anim->sprite==NULL) return;
	SDL_RendererFlip flip;
	SDL_Rect targetarea;
	SDL_Rect src;

	flip = SDL_FLIP_NONE;
	if(anim->mirror.x!=1)
		flip = (SDL_RendererFlip)(flip|SDL_FLIP_HORIZONTAL);
	if(anim->mirror.y!=1)
		flip = (SDL_RendererFlip)(flip|SDL_FLIP_VERTICAL);


	src.x = anim->curFrame % anim->sprite->framesperline * anim->sprite->w;
    src.y = anim->curFrame / anim->sprite->framesperline * anim->sprite->h;
	src.w = anim->sprite->w;
	src.h = anim->sprite->h;


	targetarea.x = (pos.x - anim->sprite->s_offset_x);
	targetarea.y = (pos.y - anim->sprite->s_offset_y);
	targetarea.w = src.w;
	targetarea.h = src.h;

	if(c != NULL){
		targetarea.x -= c->viewport.x;
		targetarea.y -= c->viewport.y;
	}

	SDL_RenderCopyEx(mainRenderer,anim->sprite->image,&src,&targetarea,0,NULL,flip);
}

void DrawRect(SDL_Rect rect, Camera *c, Uint32 color){
	rect.x -= c->viewport.x;
	rect.y -= c->viewport.y;
	SDL_SetRenderDrawColor(mainRenderer, (color & COL_MASK_R) >> R_SHIFT , (color & COL_MASK_G) >> G_SHIFT, (color & COL_MASK_B) >> B_SHIFT, 255);
	SDL_RenderDrawRect(mainRenderer, &rect);
}

void DrawRectFill(SDL_Rect rect, Camera *c, Uint32 color){
	rect.x -= c->viewport.x;
	rect.y -= c->viewport.y;
	SDL_SetRenderDrawColor(mainRenderer, (color & COL_MASK_R) >> R_SHIFT , (color & COL_MASK_G) >> G_SHIFT, (color & COL_MASK_B) >> B_SHIFT, 255);
	SDL_RenderFillRect(mainRenderer, &rect);
}

void DrawTile(Vec2i pos){
	SDL_Rect tile = {pos.x*TILE_W,pos.y*TILE_H,TILE_W,TILE_H}; // Just some random rect

	tile.y -= World[pos.x][pos.y]->structure->height<<2;
	DrawRect(tile,&mainCamera);
}

void DrawFacingCursor(Vec2i pos){
	SDL_Rect tile = {pos.x*TILE_W+0.4*TILE_W,pos.y*TILE_H+0.4*TILE_H,TILE_W*0.2,TILE_H*0.2}; // Just some random rect
	if(World[pos.x][pos.y] != NULL)
		tile.y -= World[pos.x][pos.y]->structure->height<<2;
	DrawRect(tile,&mainCamera);
}


void DrawPanel(SDL_Rect rect, Sprite *spr){		//takes a spritesheet (3 tiles x 3 tiles) and draws the specified rect using it
//	DrawRect(rect,&uiCamera);

	Vec2i tile_size;		//the size of the sprite tiles
	SDL_Rect subrect;		//used when only part of the sprite may end up being drawn
	Vec2i loc;
	SetVec2i(tile_size,spr->w,spr->h);
	SetVec2i(loc,rect.x,rect.y);
	SetRect(subrect,0,0,tile_size.x,tile_size.y);	

	//top left corner
	loc = loc - tile_size;
	DrawSprite(spr,0,loc,&uiCamera);

	//top edge
	loc.x += tile_size.x;
	while((loc.x) < (rect.x + rect.w)){		
		if((loc.x + tile_size.x) > (rect.x + rect.w)){		//check that the current column fits - adjust otherwise
			subrect.w = (rect.x + rect.w) - loc.x;
		}
		DrawPartialSprite(spr,1,subrect,loc,&uiCamera);
		loc.x += subrect.w;
	}
	subrect.w = tile_size.x;		//reset subrect width for the next row

	//top right corner
	DrawSprite(spr,2,loc,&uiCamera);
	loc.x = rect.x - tile_size.x;
	loc.y += tile_size.y;

	while(loc.y < (rect.y + rect.h)){
		if((loc.y + tile_size.y) > (rect.y + rect.h)){		//check that the current row fits - adjust otherwise
			subrect.h = (rect.y + rect.h) - loc.y;
		}
		//left edge
		DrawPartialSprite(spr,3,subrect,loc,&uiCamera);
		loc.x += tile_size.x;
		while((loc.x) < (rect.x + rect.w)){		
			if((loc.x + tile_size.x) > (rect.x + rect.w)){	//check that the current column fits - adjust otherwise
				subrect.w = (rect.x + rect.w) - loc.x;
			}
			DrawPartialSprite(spr,4,subrect,loc,&uiCamera);
			loc.x += subrect.w;
		}
		subrect.w = tile_size.x;		//reset subrect width for the next row
		DrawPartialSprite(spr,5,subrect,loc,&uiCamera);
		loc.x = rect.x - tile_size.x;
		loc.y += subrect.h;
	}
	subrect.h = tile_size.y;		//reset subrect height for the next row

	DrawSprite(spr,6,loc,&uiCamera);
	loc.x += tile_size.x;
	while((loc.x) < (rect.x + rect.w)){		
		if((loc.x + tile_size.x) > (rect.x + rect.w)){		//check that the current column fits - adjust otherwise
			subrect.w = (rect.x + rect.w) - loc.x;
		}
		DrawPartialSprite(spr,7,subrect,loc,&uiCamera);
		loc.x += subrect.w;
	}
	DrawSprite(spr,8,loc,&uiCamera);

}

//////			World Drawing

void DrawWorld(){	//Draws the world row by row. Limitations: Entities walking south are drawn beneath the tile they're walking into
	int row;
	static OverworldEnt *ents_drawn[MAX_ENTS];
	static int num_ents;

	for(int i = 0; i < MAX_ENTS; i++){
		ents_drawn[i] = NULL;
	}
	num_ents = 0;

	int currentLayer;
	int numLayers = 1;

	for(row = 0; row < WORLD_H; row++){
		for(currentLayer = 0; currentLayer < numLayers; currentLayer++){
		//	DrawRow(currentRow,currentLayer);
			for(int col = 0; col < WORLD_W; col++){
				if (World[col][row] != NULL){
					if(World[col][row]->structure->spritesheet == NULL) continue;
					Vec2i loc(col*TILE_W,row*TILE_H);
					loc = World[col][row]->position;
					//draw the base height tile
					World[col][row]->structure->baseframe = DrawSprite(World[col][row]->structure->spritesheet,World[col][row]->structure->baseframe,loc,&mainCamera);
					loc.y -= World[col][row]->structure->height<<2;
					//draw the ground height tile
					World[col][row]->structure->floorframe = DrawSprite(World[col][row]->structure->spritesheet,World[col][row]->structure->floorframe,loc,&mainCamera);
					loc.y += TILE_H;
					World[col][row]->structure->wallframe = DrawSprite(World[col][row]->structure->spritesheet,World[col][row]->structure->wallframe,loc,&mainCamera);
					
			
				}
			}
			//draw tile contents after drawing the row
			for(int col = 0; col < WORLD_W; col++){
				if (World[col][row] != NULL){
					if(World[col][row]->contents!=NULL){
						OverworldEnt *contents = World[col][row]->contents;
						if(contents == NULL) continue;

						Vec2i pos;
						SetVec2i(pos,col,row);

						if(DEBUG_DRAW_RECTS)
							DrawTile(pos);

						bool drawn = false;
						for(int i = 0; i < num_ents; i++){
							if(contents == ents_drawn[i])	//if the tile's contents were already drawn
								drawn = true;
						}
						if(drawn) continue;
						
						//this is gross but idk a better way to do this
						if((World[col][row+1] != NULL)&&(World[col][row+1]->contents == contents))
							continue;	//basically if this same entity is also on a tile south of us, don't draw it yet
						if(col > 0)
							if((World[col-1][row+1] != NULL)&&(World[col-1][row+1]->contents == contents))
								continue;	//also check for diagonals obviously
						if((World[col+1][row+1] != NULL)&&(World[col+1][row+1]->contents == contents))
							continue;

						if(row > 0){
							if((World[col][row-1] != NULL)&&(World[col][row-1]->contents != NULL)&&(World[col][row-1]->contents != contents)){	//if there's something behind us
								//continue;
								World[col][row-1]->contents->Draw();
								ents_drawn[num_ents] = World[col][row-1]->contents;
								num_ents++;
							}
						}

						contents->Draw();
						ents_drawn[num_ents] = contents;
						num_ents++;
						

					}
				}
			}
		}
	}
}

/*
void DrawWorld2(){	//draws the game in stacked layers based on their height. doesn't work.
	int row;
	static OverworldEnt *ents_drawn[MAX_ENTS];
	static int num_ents;

	for(int i = 0; i < MAX_ENTS; i++){
		ents_drawn[i] = NULL;
	}
	num_ents = 0;

	int currentLayer = 0;
	int numLayers = 10;	//the number of different heights tiles can have 

	//Draw the Base tiles first. This ensures the world has no gaping holes.
	for(row = 0; row < WORLD_H; row++){
		for(int col = 0; col < WORLD_W; col++){
			if (World[col][row] == NULL) continue;
			if(World[col][row]->structure->spritesheet == NULL) continue;
			//draw the base height tile
			Vec2i loc = World[col][row]->position;
			World[col][row]->structure->baseframe = DrawSprite(World[col][row]->structure->spritesheet,World[col][row]->structure->baseframe,loc,&mainCamera);	
			// I thiiiiink walls should also be drawn at this point? idfk
			loc.y -= World[col][row]->structure->height<<2;
			loc.y += TILE_H;
			World[col][row]->structure->wallframe = DrawSprite(World[col][row]->structure->spritesheet,World[col][row]->structure->wallframe,loc,&mainCamera);
					
		}
	}


	
	for(currentLayer = 0; currentLayer < numLayers; currentLayer++){
		for(row = 0; row < WORLD_H; row++){
			for(int col = 0; col < WORLD_W; col++){
				//draw all tiles on a layer
				if (World[col][row] != NULL){
					if(World[col][row]->structure->spritesheet == NULL) continue;
					Vec2i loc = {col*TILE_W,row*TILE_H};
					loc = World[col][row]->position;
					if(World[col][row]->structure->height != currentLayer)
						continue;
					loc.y -= World[col][row]->structure->height<<2;
					//draw the ground height tile
					World[col][row]->structure->floorframe = DrawSprite(World[col][row]->structure->spritesheet,World[col][row]->structure->floorframe,loc,&mainCamera);
					loc.y += TILE_H;
				//	World[col][row]->structure->wallframe = DrawSprite(World[col][row]->structure->spritesheet,World[col][row]->structure->wallframe,loc,&mainCamera);
					
			
				}
			}
			//draw tile contents on that layer
			for(int col = 0; col < WORLD_W; col++){
				if (World[col][row] != NULL){
					if(World[col][row]->contents!=NULL){
						if(World[col][row]->structure->height != currentLayer)
							continue;
						OverworldEnt *contents = World[col][row]->contents;
						bool drawn = false;
						for(int i = 0; i < num_ents; i++){
							if(contents == ents_drawn[i])
								drawn = true;
						}
						if(drawn) continue;
						
						contents->Draw();
						ents_drawn[num_ents] = contents;
						num_ents++;
						
					}
				}
			}
		}
	}
}
*/

void DrawRow(int row, int layer){
	return;

/*
	for(int col = 0; col < WORLD_W; col++){
		if (World[col][row] != NULL){
			Vec2i loc = {col*TILE_W,row*TILE_H};
			loc = World[col][row]->position;
			loc.y -= World[col][row]->structure->height<<2;
			//draw the tile
			if(World[col][row]->structure->lowerspr != NULL){
				World[col][row]->structure->lowerframe = DrawSprite(World[col][row]->structure->lowerspr,World[col][row]->structure->lowerframe,loc,&mainCamera);
			}
			
			
		}
	}
	//draw tile contents after drawing the row
	for(int col = 0; col < WORLD_W; col++){
		static OverworldEnt *last_drawn = NULL;
		if (World[col][row] != NULL){
			if(World[col][row]->contents!=NULL){
				OverworldEnt *contents = World[col][row]->contents;
				if(contents != last_drawn){
					contents->Draw();
					last_drawn = contents;
				}
			}
		}
	}*/
}






/////////////////////////////////////////////////////////////////////////////////////////////////
//soon to be unused
void DrawTilesLower(){
/*	for(int i = 0; i < WORLD_W; i++){
		for(int j = 0; j < WORLD_H; j++){
			if (World[i][j] != NULL){
				if(World[i][j]->lowerspr != NULL){
					fprintf(stdout,"Tile %i, %i   ",i,j);
					fprintf(stdout,World[i][j]->lowerspr->filename);
					fprintf(stdout,"   Position: %i, %i \n",World[i][j]->position.x,World[i][j]->position.y);
					Vec2i loc = {i*TILE_W,j*TILE_H};
					loc = World[i][j]->position;
					loc.y -= World[i][j]->height<<3;
//					fprintf(stdout,"%i, %i \n",loc.x,loc.y);
					World[i][j]->lowerframe = DrawSprite(World[i][j]->lowerspr,World[i][j]->lowerframe,loc,&mainCamera);
					if(World[i][j]->contents!=NULL){
						OverworldEnt *contents = World[i][j]->contents;
						DrawAnimation(contents->animlist[contents->animation][contents->direction],contents->worldposition-contents->s_offset,&mainCamera);
					}
				}
			}
		}
	}*/
}
//soon to be unused
void DrawTilesUpper(){
	/*for(int i = 0; i < WORLD_W; i++){
		for(int j = 0; j < WORLD_H; j++){
			if (World[i][j] != NULL){
				if(World[i][j]->upperspr != NULL)
					World[i][j]->upperframe = DrawSprite(World[i][j]->upperspr,World[i][j]->upperframe,World[i][j]->position,&mainCamera);
//				if(DEBUG&&World[i][j]->free==false)
//					DrawSprite(World[i][j]->debugFill,World[i][j]->position,&mainCamera);
			}
		}
	}*/
}
/////////////////////////////////////////////////////////////////////////////////////////////////////

