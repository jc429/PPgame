#include "main.h"
#include <vector>
using std::vector;

void InitGame();
void PollEvents();
Uint16 PollInputs();
void UpdateGame();
void DrawGame();

bool done;
bool pause;

extern SDL_Window *sdlWindow;
extern SDL_Renderer *mainRenderer;
extern Camera mainCamera;
extern Camera uiCamera;

//This and last frame's inputs
Uint16 curInput;		//00000000LRUDABXY
Uint16 prevInput;	
InputNode *_Inputs;

Tile* World[WORLD_W][WORLD_H];

Player *_Player;
NPC *NPCList[MAX_NPC];

extern int framecheck;

extern Textbox mainTextbox; //the main dialogue box for now
//extern bool dialogue;	//are we currently talking?
//extern bool inMenu;		//are we currently in a menu?

extern GameScene *_CurrentScene;
extern vector<GameScene*> _SceneStack;
extern GameState _GameState;


extern Menu *_CurrentMenu;
extern vector<struct Menu_T*> _MenuStack;
//Menu _MenuStack[MAX_MENUS];
//std::vector<Menu_T> MenuStack;

void OpenPauseMenu();
Menu *pauseMenu;

bool _Dialogue;	//are we currently talking?

int main (int argc, char* argv[]){
	done = 0;

	/*if(DEBUG_BREAK){
		DEBUG_BREAK_FUNC();
		while(true);
	}*/


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
	InitChunkList();


	numMenus = 0;
/////////////////////
	
	InitCombat();

	InitMainTextbox(&mainTextbox,4,40,LoadSprite(SPATH_MAIN_TEXTBOX,320,80,1));
	pauseMenu = LoadPauseMenu();

/////////////////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//test stuff goes between the squiggles
	

	What(); 

	_CurrentScene = new Overworld();
	

	LoadDialogue();


	LoadCombatBG();
	
	//Music *m = LoadMusic("sounds/GV.mp3",40);
	//Sound *s = LoadSound("sounds/gui.wav",35);	
	//PlayMusic(m);
	//PlaySound(s);

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
	inputNode->prev = _Inputs;//_Player->inputs;
	inputNode->input = curInput;
	_Inputs = inputNode;	
	DeleteInputNode(inputNode, INPUTS_HISTORY);

	if(DEBUG){
		if(_Inputs->input & 1<<14){
			if(_GameState == OVERWORLD){
				EnterCombat();
			}
		}
		if(_Inputs->input & 1<<15){
			if(_GameState == COMBAT){
				ExitCombat();
			}
		}
	}

}

Uint16 PollInputs(){
	Uint16 inputs = 0;
	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	if(DEBUG)
		if (keys[SDL_SCANCODE_ESCAPE])
			done = 1;

	if(DEBUG){		//debug inputs
		if(keys[SDL_SCANCODE_M])
			inputs |= 1<<15;
		if(keys[SDL_SCANCODE_N]){
			inputs |= 1<<14;		
		}
	}

	if(keys[PPKEY_PAUSE]){
		pause = 1;
	}
	if(keys[PPKEY_START])
		inputs |= PPINPUT_START;

	if(keys[PPKEY_LEFT])
		inputs |= PPINPUT_LEFT;
	if(keys[PPKEY_RIGHT])
		inputs |= PPINPUT_RIGHT;
	if(keys[PPKEY_UP])
		inputs |= PPINPUT_UP;
	if(keys[PPKEY_DOWN])
		inputs |= PPINPUT_DOWN;
	if(keys[PPKEY_A])
		inputs |= PPINPUT_A;
	if(keys[PPKEY_B])
		inputs |= PPINPUT_B;
	if(keys[PPKEY_X])
		inputs |= PPINPUT_X;
	if(keys[PPKEY_Y])
		inputs |= PPINPUT_Y;
	return inputs;
}

//This is where the meat of the game will be
void UpdateGame(){
	if(_GamePause)
		return;
	//
	if(!_SceneStack.empty()){
		_GameState = _SceneStack.back()->type;
		_SceneStack.back()->Update();
	}
	//
	
	


	/*int i;
	for(i = MAX_MENUS-1; i >= 0; i--)
		if(_MenuStack[i].used&&_MenuStack[i].active)	//only update the highest active menu, i.e. the one on top
			break;
	if(i>=0)											//dont update a menu that doesn't exist
		UpdateMenu(&_MenuStack[i]);
		*/
}

//Put the Drawing functions for everything here 
void DrawGame(){
	if(_GamePause)
		return;
	if(_DrawPause) {
		return;
	}
	framecheck+=1;
	framecheck %= 360; //reset the framecheck after 10 seconds of drawing because what animation would ever play slower than that
/*	if(framecheck>FRAMESPERDRAW)
		framecheck=0;*/

	if(!_SceneStack.empty()){
		for(int i = 0; i < (int)_SceneStack.size(); i++){
			_SceneStack[i]->Draw();

		}
	}
		
	
	

	RenderCurrentFrame();
}


bool InCombat(){
	return (_SceneStack.back()->type == COMBAT);
}

void EnterCombat(){
	_CurrentScene = new CombatTransition();
}

void ExitCombat(){
	StopMusic();
	_SceneStack.pop_back();
}

void LoadLevel(){
	Chunk *ch = LoadChunk("testfiles/chunk1.json");
	for(int i = 0; i < ch->size.x; i++){
		for(int j = 0; j < ch->size.y; j++){
			World[i][j] = &ch->tiles[i][j];
		}
	}
	/*
	char *lvpath = "testfiles/map.txt";
	Level *level = new Level;
	if((LoadCFG(level,lvpath)!=0)||(FORCE_DEBUG_LEVEL))
		InitWorld();*/
}

void Overworld::Update(){
	if(InputPressed(PPINPUT_B))
		if(!_Dialogue){
			if(_MenuStack.empty())
				OpenPauseMenu();
			else
				if(_MenuStack.back()->type == MENU_PAUSE)
					CancelMenu();

		}
	UpdateWorld();
	if(!_MenuStack.empty()){
		_CurrentMenu = _MenuStack.back();
		UpdateMenu(_MenuStack.back());
	}else{
		UpdatePlayer(_Player);
	}
	UpdateCamera(&mainCamera);
	
}

void Overworld::Draw(){
	//DrawWorld();
	DrawWorld();
	if(DEBUG_DRAW_RECTS){
		DrawFacingCursor(_Player->tile+_Player->facing);
	}

	DrawOverworldUI();
	if(_Dialogue)
		DrawTextbox(&mainTextbox);
	if(!_MenuStack.empty()){
		for(int i = 0; i < (int)_MenuStack.size(); i++){
			DrawMenu(_MenuStack[i]);
		}
	}
}

Combat::Combat(){
	type = COMBAT;
	_SceneStack.push_back(this);
	SetupCombat();
}

Combat::~Combat(){
	_SceneStack.pop_back();
}

void Combat::Update(){
	UpdateCombat();
	
}

void Combat::Draw(){
	DrawCombatBG();
	DrawEnemies();
	DrawAllies();
	DrawCombatUI();
}

CombatTransition::CombatTransition(){
	type = CUTSCENE;
	Sprite *spr = LoadSprite(SPATH_COMBAT_ENTER,320,240,1);
	startanim = LoadAnimation(spr,0,0,10,1,0,3);
	spr = LoadSprite(SPATH_COMBAT_EXIT,320,240,1);
	endanim = LoadAnimation(spr,0,0,10,1,0,3);
	anim = startanim;
	timer = 58;
	this->combat_entered = false;
	_SceneStack.push_back(this);
}

CombatTransition::~CombatTransition(){
	_SceneStack.pop_back();
}

void CombatTransition::Update(){
	if(timer > 0)
		timer--;
	if(timer == 29){
		this->combat_entered = true;
		StartCombat();
	}
	if(timer == 28)
		anim = endanim;
	if(timer == 0){
		if(combat_entered)
			delete(this);
	}
}

void CombatTransition::Draw(){
	Vec2i loc = {0,0};
	DrawAnimation(this->anim,loc,&uiCamera);
}

void StartCombat(){
	_CurrentScene = new Combat();
}

Menu *LoadPauseMenu(){
	char* names[6] =  {"Party ","Items ","FIGHT ","Save  ","Options","Exit "};
	Menu *m = LoadMenu(MENU_PAUSE);
	for(int i = 0; i < m->numItems; i++){
		SetMenuItemAction(m->items[i],CancelMenu,names[i]);
	}
	SetMenuItemAction(m->items[2],LaunchCombat);
	return m;
}

void OpenPauseMenu(){
	OpenMenu(pauseMenu);
}

void LaunchCombat(){
	_CurrentScene = new CombatTransition();
}