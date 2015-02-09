#include "main.h"


void InitGame();
void PollEvents();
void UpdateGame();
void DrawGame();

bool done;

int c;

extern SDL_Window *sdlWindow;
extern SDL_Renderer *mainRenderer;
extern SDL_Rect mainCamera;
extern SDL_Rect UICamera;


int main (int argc, char* argv[]){
	done = 0;
	InitGame();
	
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
	//atexit(SDL_Quit);


	InitWindow();

}

void PollEvents(){
	SDL_Event e;
	while(SDL_PollEvent(&e)){
		if(e.type == SDL_QUIT)
			done = 1;
	}
}

void UpdateGame(){
	UpdateCamera();
}

void DrawGame(){
	c++;
	mainCamera.x++;
	TestGraphics(1);
}

