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
InputNode *_Inputs;

Tile* World[WORLD_W][WORLD_H];

Player *_Player;
NPC *NPCList[MAX_NPC];

extern int framecheck;

extern Textbox mainTextbox; //the main dialogue box for now
//extern bool dialogue;	//are we currently talking?
//extern bool inMenu;		//are we currently in a menu?

Menu _MenuStack[MAX_MENUS];


Menu *testMenu;


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
	if(SDL_Init(SDL_INIT_EVERYTHING))
		exit(-1);
	IMG_Init(IMG_INIT_PNG);
	InitAudio();
	InitWindow();
	InitFont();

	InitSpriteList();
	InitMusicList();
	InitSoundList();

	numMenus = 0;
/////////////////////

	Sprite *mainTextSpr = LoadSprite("sprites/textbox.png",320,80,1);
	InitMainTextbox(&mainTextbox,4,40,mainTextSpr);

/////////////////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//test stuff feel free to remove
	char *lvpath = "cfg/test.txt";
	Level *level = new Level;
	if((LoadCFG(level,lvpath)!=0)||(FORCE_DEBUG_LEVEL))
		InitWorld();

	Vec2i menuloc = {250,10};
//	testMenu = OpenMenuYesNo(menuloc,CancelMenu,CancelMenu);


/*
	Entity *e = NewEntity();
	e->talks = 1;
	World[5][5]->contents = e;
	*/
	InteractableObject *sign = new InteractableObject(5,5);
	InteractableObject *sign2 = new InteractableObject(11,3);
	sign->flavortext = new Message;
	sign->flavortext->text = "Hey there I'm a shitty sign!                Smoke weed every day";
	
	Vec2i promptloc;
	promptloc.x = LOC_DEFAULT_PROMPT_X;
	promptloc.y = LOC_DEFAULT_PROMPT_Y;

	CreateMessage(sign2->flavortext,"Where are you from?");
	SetPrompt(sign2->flavortext,MENU_YES_NO,promptloc);
	SetAnswers(sign2->flavortext,2,CancelMenu,CancelMenu);
	
	World[11][3]->contents = sign2;
	World[11][3]->free = sign2->passable;
	World[5][5]->contents = sign;
	World[5][5]->free = sign->passable;

	LoadCombatBG();
	
	//Music *m = LoadMusic("sounds/GV.mp3",40);
	//Sound *s = LoadSound("sounds/gui.wav",35);	
	//PlayMusic(m);
	//PlaySound(s);

	//////////////////
/////////////////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	_Player = new Player;
	mainCamera.target = _Player;
	mainCamera.trackPlayer = true;

	_MessageStack = NULL;
//	SDL_ShowCursor(SDL_DISABLE);	//turns off the cursor

	if(!DEBUG){
		atexit(SDL_Quit);
		atexit(TTF_Quit);
	}

	if(DEBUG)
		_GameState = DEBUG_START_MODE;
	else
		_GameState = START_MODE;
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
	inputNode->prev = _Player->inputs;
	inputNode->input = curInput;
	_Inputs = inputNode;
	_Player->inputs = inputNode;

	if((inputNode->input&PPINPUT_A)&&!(inputNode->prev->input & PPINPUT_A)){
		if(_Dialogue || _Player->talking){
			if(mainTextbox.donewriting){
				_Player->talking = false;
				_Dialogue = false;
			}
		}
		else{
			if(World[_Player->tile.x+_Player->facing.x][_Player->tile.y+_Player->facing.y]!=NULL){
				if(World[_Player->tile.x+_Player->facing.x][_Player->tile.y+_Player->facing.y]->contents!=NULL){
					if(World[_Player->tile.x+_Player->facing.x][_Player->tile.y+_Player->facing.y]->contents->talks){
						if(!_Player->talking && !_Dialogue){
							_Dialogue = true;
							_Player->talking = true;
							World[_Player->tile.x+_Player->facing.x][_Player->tile.y+_Player->facing.y]->contents->Talk(&mainTextbox);
						}
					}
				}
			}
		}
	}
}

Uint8 PollInputs(){
	Uint8 inputs = 00000000;
	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	if (keys[SDL_SCANCODE_ESCAPE])
		done = 1;

	if(DEBUG){		//debug inputs
		if(keys[SDL_SCANCODE_M]&&_GameState==COMBAT)
			_GameState = OVERWORLD;
		if(keys[SDL_SCANCODE_N]){
			if(_GameState==OVERWORLD){
				EnterCombat();
				_GameState = COMBAT;
			}
		}
		if(keys[SDL_SCANCODE_L])
			DecrementCursor(testMenu);
		if(keys[SDL_SCANCODE_K])
			IncrementCursor(testMenu);
	}

	if(keys[PPKEY_PAUSE])
		pause = 1;

	if(keys[PPKEY_LEFT])
		inputs |= 1<<7;
	if(keys[PPKEY_RIGHT])
		inputs |= 1<<6;
	if(keys[PPKEY_UP])
		inputs |= 1<<5;
	if(keys[PPKEY_DOWN])
		inputs |= 1<<4;
	if(keys[PPKEY_A])
		inputs |= 1<<3;
	if(keys[PPKEY_B])
		inputs |= 1<<2;
	if(keys[PPKEY_X])
		inputs |= 1<<1;
	if(keys[PPKEY_Y])
		inputs |= 1;
	return inputs;
}

//This is where the meat of the game will be
void UpdateGame(){
	if(_GamePause)
		return;
	switch(_GameState){
	case OVERWORLD:
		UpdatePlayer(_Player);
		UpdateCamera(&mainCamera);
		break;
	case COMBAT:
		UpdateCombat();
		break;
	}


	int i;
	for(i = MAX_MENUS-1; i >= 0; i--)
		if(_MenuStack[i].used&&_MenuStack[i].active)	//only update the highest active menu, i.e. the one on top
			break;
	if(i>=0)											//dont update a menu that doesn't exist
		UpdateMenu(&_MenuStack[i]);
}

//Put the Drawing functions for everything here 
void DrawGame(){
	if(_GamePause)
		return;
	if(_DrawPause) {
		//fprintf(stdout,"%i \n",_DrawPause);
		return;
	}
	framecheck+=1;
	if(framecheck>FRAMESPERDRAW)
		framecheck=0;

	switch(_GameState){
	case OVERWORLD:
	//	DrawWorld();
		DrawTilesLower();
		DrawPlayer(_Player);
		DrawTilesUpper();
		DrawOverworldUI();
		if(_Dialogue)
			DrawTextbox(&mainTextbox);
//		if(_InMenu)

 			for(int i = 0; i < MAX_MENUS; i++){
				if(_MenuStack[i].used)
					if(_MenuStack[i].active)
						DrawMenu(&_MenuStack[i]);
			}
		break;

	case COMBAT:
		DrawCombatBG();
		DrawEnemies();
		DrawAllies();
		DrawCombatUI();
		break;
	}

	RenderCurrentFrame();
}
