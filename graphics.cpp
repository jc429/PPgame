#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "graphics.h"
#include "pmath.h"

using namespace std;

SDL_Window *sdlWindow;
SDL_Renderer *mainRenderer;
SDL_Rect world;
SDL_Rect mainCamera;
SDL_Rect UICamera;

Sprite SpriteList[SPRITES_MAX];
int NumSprites;

Uint32 NOW;		/*the current time since program started*/
	

void InitWindow(){

	mainCamera.w = GAME_RES_X;
	mainCamera.h = GAME_RES_Y;

	int fullscreenFlag = 0;
	if(STARTFULLSCREEN != 0){
		fullscreenFlag = SDL_WINDOW_FULLSCREEN_DESKTOP;
	}
	sdlWindow = SDL_CreateWindow(GAMENAME,
                          SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                          WINDOW_RES_X, WINDOW_RES_Y,
                          fullscreenFlag | SDL_WINDOW_OPENGL);
	mainRenderer = SDL_CreateRenderer(sdlWindow, -1, 0);
	//SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");  // make the scaled rendering look smoother. (anti-aliasing)
	SDL_RenderSetLogicalSize(mainRenderer, GAME_RES_X, GAME_RES_Y);
	SDL_SetRenderDrawColor(mainRenderer, 0, 30, 30, 255);
	SDL_RenderClear(mainRenderer);
	SDL_RenderPresent(mainRenderer);
}

void InitWorld(){
	world.w = WORLD_W;
	world.h = WORLD_H;
}

void NextFrame(){
	Uint32 Then;
	SDL_RenderPresent(mainRenderer);						/*and then update the screen*/
	SDL_RenderClear(mainRenderer);
	Then = NOW;									/*these next few lines  are used to show how long each frame takes to update.  */
	NOW = SDL_GetTicks();
	/*fprintf(stdout,"Ticks passed this frame: %i\n", NOW - Then);*/
	FrameDelay(FRAMEDELAY); /* 33 will make your frame rate about 30 frames per second.  If you want 60 fps then set it to about 17*/
}

void FrameDelay(Uint32 delay){
    static Uint32 pass = 100;
    Uint32 dif;
    dif = SDL_GetTicks() - pass;
    if(dif < delay)SDL_Delay( delay - dif);
    pass = SDL_GetTicks();
}

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
	
  
  /*first search to see if the requested sprite image is alreday loaded*/
  //////////////////////////////////////////////FIX THIS
  for(i = 0; i < NumSprites; i++)
  {
    if(strncmp(filename,SpriteList[i].filename,40)==0)
    {
      SpriteList[i].used++;
      return &SpriteList[i];
    }
  }
  /*makesure we have the room for a new sprite*/ 
  if(NumSprites + 1 >= SPRITES_MAX)
  {
        fprintf(stderr, "Maximum Sprites Reached.\n");
        exit(1);
  }
  /*if its not already in memory, then load it.*/
  NumSprites++;
  for(i = 0;i <= NumSprites;i++)
  {
    if(!SpriteList[i].used)break;
  }
  temp = IMG_Load(filename);
  if(temp == NULL)
  {
    fprintf(stderr,"unable to load a vital sprite: %s\n",SDL_GetError());
    exit(0);
  }
  SpriteList[i].image = temp; //SDL_DisplayFormatAlpha(temp);
//  SDL_FreeSurface(temp);
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

void DrawSprite(Sprite* spr, Vec2i pos){
	SDL_Rect targetarea;
	targetarea.x = pos.x-mainCamera.x;
	targetarea.y = pos.y-mainCamera.y;
	targetarea.w = spr->w;
	targetarea.h = spr->h;
	SDL_Texture *texture = SDL_CreateTextureFromSurface(mainRenderer, spr->image);
	SDL_RenderCopy(mainRenderer,texture,NULL,&targetarea);
}


void UpdateCamera(){
}

void TestGraphics(int x){
	
	static Sprite *testbmp;
	static Sprite *testbg;
	testbmp = LoadSprite("sprites/happi.png",16,16,1);
	testbg = LoadSprite("sprites/bg.png",640,480,1);
	if(testbmp->image == NULL) return;
	if(testbg->image == NULL) return;
	SDL_Rect  targetarea;

	SDL_Surface *s = IMG_Load("sprites/happi.png");

	SDL_Texture *sdlTexture = SDL_CreateTextureFromSurface(mainRenderer, s);
	SDL_Texture *sdlTexture2 = SDL_CreateTextureFromSurface(mainRenderer, testbg->image);
	//SDL_Texture *sdlTexture2 = SDL_CreateTextureFromSurface(mainRenderer, testbg->image);

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
	
	SDL_RenderCopy(mainRenderer,sdlTexture2,NULL,&targetarea);
	
	targetarea.x = c.x;
	targetarea.y = c.y;
	targetarea.w = 16;
	targetarea.h = 16;
//	SDL_RenderCopy(mainRenderer,sdlTexture,NULL,&targetarea);	
	DrawSprite(testbmp,c);
}
