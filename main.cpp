#include "main.h"
#include <vector>
using std::vector;

void InitGame();
void ExitGame();
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

extern vector<CombatEnt*> enemylist;
extern int CombatPartyIDs[MAX_PARTY_COMBAT];
extern int EnemyIDs[MAX_ENEMIES];

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

	InitItemTable();
	InitInventory();
	InitParty();
	
	numMenus = 0;
/////////////////////
	LoadWallet();
	InitCombat();

	InitMainTextbox(&mainTextbox,4,40,LoadSprite(SPATH_MAIN_TEXTBOX,320,80,1));
	pauseMenu = LoadPauseMenu();

/////////////////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//test stuff goes between the squiggles
	
	_CurrentScene = new Overworld();
	
	AddMoney(6000);
	AddItem(300);
	AddItem(600);
	AddItem(300);
	AddItem(600);
	AddItem(601);
	AddItem(10);
	AddItem(301);


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

void ExitGame(){
	done = 1;
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
		if(InputPressed(PPINPUT_ESC)){
			if(_GameState == INVENTORY){
				CloseInventory();
			}else if(_GameState == COMBAT){
				ExitCombat();
			}else{
				if(!_MenuStack.empty())
					CancelMenu();
				else
					done = 1;
			}
			
		}

	}

}

Uint16 PollInputs(){
	Uint16 inputs = 0;
	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	if(DEBUG)
		if (keys[SDL_SCANCODE_F4])
			ExitGame();

	if(DEBUG){		//debug inputs
	/*	if(keys[SDL_SCANCODE_M])
			inputs |= 1<<15;
		if(keys[SDL_SCANCODE_N]){
			inputs |= 1<<14;		
		}*/
		if(keys[SDL_SCANCODE_ESCAPE])
			inputs |= PPINPUT_ESC;
	}

	if(keys[PPKEY_PAUSE]){
		pause = 1;
	}
	if(keys[PPKEY_START])
		inputs |= PPINPUT_START;

	if(keys[PPKEY_LEFT]||keys[PPKEY_LEFT2])
		inputs |= PPINPUT_LEFT;
	if(keys[PPKEY_RIGHT]||keys[PPKEY_RIGHT2])
		inputs |= PPINPUT_RIGHT;
	if(keys[PPKEY_UP]||keys[PPKEY_UP2])
		inputs |= PPINPUT_UP;
	if(keys[PPKEY_DOWN]||keys[PPKEY_DOWN2])
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
	//only go to combat if enemies are loaded
	for(int i = 0; i < MAX_ENEMIES; i++){
		if(EnemyIDs[i] != 0){
			break;
		}
		if(i + 1 == MAX_ENEMIES)
			return;
	}
	_CurrentScene = new CombatTransition();
}

void ExitCombat(){
	StopMusic();
	_SceneStack.pop_back();
}

void LoadLevel(){
//	Chunk *ch = LoadChunk("testfiles/chunk2.json");
	Area *a = LoadArea("testfiles/area1.json");

	for(int x = 0; x < a->numchunks; x++){
		Chunk *ch = a->chunklist[x];
		Vec2i off = ch->location;
		for(int i = 0; i < ch->size.x; i++){
			for(int j = 0; j < ch->size.y; j++){
				World[i+off.x][j+off.y] = &ch->tiles[i][j];
			}
		}
	}
	/*
	char *lvpath = "testfiles/map.txt";
	Level *level = new Level;
	if((LoadCFG(level,lvpath)!=0)||(FORCE_DEBUG_LEVEL))
		InitWorld();*/
	enemylist = LoadEnemyDataCFG("testfiles/enemy-test.json");
	SetEnemies(2, enemylist.at(1)->chardata->id, enemylist.at(1)->chardata->id);
}

void Overworld::Update(){
	if(InputPressed(PPINPUT_B))
		if(!_Dialogue){
			if(_MenuStack.empty()){
				OpenPauseMenu();
				ShowWallet();
			}else
				if(_MenuStack.back()->type == MENU_PAUSE){
					CancelMenu();
					HideWallet();
				}
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
	DrawWorld();
	if(DEBUG_DRAW_RECTS){
		DrawFacingCursor(_Player->tile+_Player->facing);
	}

	if(_Dialogue)
		DrawTextbox(&mainTextbox);
	if(!_MenuStack.empty()){
		for(int i = 0; i < (int)_MenuStack.size(); i++){
			DrawMenu(_MenuStack[i]);
		}
	}

	DrawWallet();
}







void StartCombat(){
	_CurrentScene = new Combat();
}

void OpenInventory(){
	UpdateInvText();
	_CurrentScene = new InventoryPage();
}

void CloseInventory(){
	_SceneStack.pop_back();
}

void OpenParty(){
	_CurrentScene = new PartyView();
}

void CloseParty(){
	_SceneStack.pop_back();
}

Menu *LoadPauseMenu(){
	char* names[6] =  {"Party ","Items ","FIGHT ","Save  ","Options","Exit "};
	Menu *m = LoadMenu(MENU_PAUSE);
	for(int i = 0; i < m->numItems; i++){
		SetMenuItemAction(m->items[i],CancelMenu,names[i]);
	}
	SetMenuItemAction(m->items[0],OpenParty);
	SetMenuItemAction(m->items[1],OpenInventory);
	SetMenuItemAction(m->items[2],EnterCombat);
	return m;
}

void OpenPauseMenu(){
	OpenMenu(pauseMenu);
}


//move this prob
void FightBoss(){
	SetEnemies(1,enemylist.at(0)->chardata->id);
	EnterCombat();
}