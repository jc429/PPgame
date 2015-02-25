#include "main.h"


void InitGame();
void PollEvents();
Uint8 PollInputs();
void UpdateGame();
void DrawGame();

bool done;
bool pause;

extern SDL_Window *sdlWindow;
extern SDL_Renderer *mainRenderer;
extern Camera mainCamera;
extern Camera uiCamera;

//This and last frame's inputs
Uint8 curInput;		//LRUDABXY
Uint8 prevInput;	

Tile* World[WORLD_W][WORLD_H];

Player *player;

extern int framecheck;

extern Textbox testbox; //the main dialogue box for now
extern bool dialogue;	//are we currently talking?

int main (int argc, char* argv[]){
	done = 0;
	InitGame();
	GameState = OVERWORLD;
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
	InitSpriteList();
	InitFont();

	/////////////////
	InitTextbox(&testbox);
//	InitTG();		//test stuff feel free to remove

	Level *level = new Level;
	if(LoadCFG(level,"cfg/test.txt")!=0)
		InitWorld();

	//////////////////

	player = CreatePlayer(player);
	mainCamera.target = player;
	mainCamera.trackPlayer = true;

//	SDL_ShowCursor(SDL_DISABLE);	//turns off the cursor

	if(!DEBUG){
		atexit(SDL_Quit);
		atexit(TTF_Quit);
	}

	GameState = MAIN_MENU;
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
	curInput = PollInputs();
	InputNode *inputNode = new InputNode;
	inputNode->prev = player->inputs;
	inputNode->input = curInput;
	player->inputs = inputNode;

	if((inputNode->input&1<<3)&&(~inputNode->prev->input & 1<<3)){
		dialogue = Toggle(dialogue);
		player->talking = Toggle(player->talking);
		SetMessage("Hellooooooo!\0",&testbox);
	}
}

Uint8 PollInputs(){
	Uint8 inputs = 00000000;
	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	if (keys[SDL_SCANCODE_ESCAPE])
		done = 1;

	if(keys[INPUT_PAUSE])
		pause = 1;

	if(keys[INPUT_LEFT])
		inputs |= 1<<7;
	if(keys[INPUT_RIGHT])
		inputs |= 1<<6;
	if(keys[INPUT_UP])
		inputs |= 1<<5;
	if(keys[INPUT_DOWN])
		inputs |= 1<<4;
	if(keys[INPUT_A])
		inputs |= 1<<3;
	if(keys[INPUT_B])
		inputs |= 1<<2;
	if(keys[INPUT_X])
		inputs |= 1<<1;
	if(keys[INPUT_Y])
		inputs |= 1;
	return inputs;
}

//This is where the meat of the game will be
void UpdateGame(){
	UpdatePlayer(player);
	
	UpdateCamera(&mainCamera);
}

//Put the Drawing functions for everything here 
void DrawGame(){
	framecheck+=1;
	if(framecheck>FRAMESPERDRAW)
		framecheck=0;

	switch(GameState){
	case OVERWORLD:
		DrawTilesLower();
		DrawPlayer(player);
		DrawTilesUpper();
		if(dialogue)
			DrawTextbox(&testbox);

		break;
	}
}
