#include "graphics.h"
 

SDL_Window *sdlWindow;
SDL_Renderer *mainRenderer;
SDL_Rect mainCamera;
SDL_Rect UICamera;

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



void DrawSprite(){

}


void UpdateCamera(){

	//mainCamera.y++;
}


void TestGraphics(int x){
	
	SDL_Surface *testbmp;
	testbmp = IMG_Load("sprites/happi.png");
	if(testbmp == NULL) return;
	SDL_Rect  targetarea;
	targetarea.x = 10+mainCamera.x;
	targetarea.y = 20+mainCamera.y;
	targetarea.w = testbmp->w;
	targetarea.h = testbmp->h;
	SDL_Texture *sdlTexture = SDL_CreateTextureFromSurface(mainRenderer, testbmp);
	
	SDL_RenderCopy(mainRenderer,sdlTexture,NULL,&targetarea);

}
