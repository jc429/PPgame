#include "main.h"


void InitGame();
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
	do{
		//The basic game loop:
		//read inputs
		//update game
		//draw everything
		PollEvents();

		UpdateGame();

		DrawGame();
		NextFrame();

	}while(!done);

	exit(0);
	return 0;		//just for the compiler
}

//put everything that needs to be run before the main game loop in here
void InitGame(){		
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);
	InitWindow();
	InitWorld();
	InitSpriteList();

	InitTG();		//test stuff feel free to remove


//	SDL_ShowCursor(SDL_DISABLE);	//turns off the cursor

	if(!DEBUG)
		atexit(SDL_Quit);
}

//This is how you get your inputs
void PollEvents(){
	SDL_Event e;
	while(SDL_PollEvent(&e)){
		if(e.type == SDL_QUIT){
			done = 1;
			return;
		}

	}
}

//This is where the meat of the game will be
void UpdateGame(){
	UpdateCamera();

	
}

//Put the Drawing functions for everything here 
void DrawGame(){

	TestGraphics(0);

}
