#include "main.h"


void InitGame();
void InitPtrs();
void PollEvents();
void UpdateGame();
void DrawGame();

bool done;

extern SDL_Window *sdlWindow;
extern SDL_Renderer *mainRenderer;
extern SDL_Rect mainCamera;
extern SDL_Rect UICamera;


int main (int argc, char* argv[]){
	done = 0;
	InitGame();
	InitTG();
	do{
		//poll inputs
		//update game
		//draw everything
		PollEvents();

		UpdateGame();

		DrawGame();
		NextFrame();

	}while(!done);
	exit(0);
	return 0;
}

void InitGame(){
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);
	InitPtrs();
	InitWindow();
	InitWorld();
	InitSpriteList();


//	SDL_ShowCursor(SDL_DISABLE);
//	atexit(SDL_Quit);
}

void InitPtrs(){
	sdlWindow = NULL;
	mainRenderer = NULL;
}

void PollEvents(){
	SDL_Event e;
	while(SDL_PollEvent(&e)){
		if(e.type == SDL_QUIT){
			done = 1;
			return;
		}
	}
}

void UpdateGame(){
	UpdateCamera();

	
}

void DrawGame(){

	TestGraphics(0);

}
