#include "main.h"
#include <vector>
using std::vector;

void InitGame();
void ExitGame(int val = 0);
void PollEvents();
Uint16 PollKeyInputs();
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

extern OverworldCharacter *_Player;
//NPC *NPCList[MAX_NPC];
extern OverworldCharacter *CharList[MAX_CHARACTERS];

extern int framecheck;

extern TextboxEX mainTextbox; //the main dialogue box for now
//extern bool dialogue;	//are we currently talking?
//extern bool inMenu;		//are we currently in a menu?

extern GameScene *_CurrentScene;
extern vector<GameScene*> _SceneStack;
extern GameState _GameState;


extern Menu *_CurrentMenu;
//extern vector<struct Menu_T*> _OverworldMenuStack;
//Menu _MenuStack[MAX_MENUS];
//std::vector<Menu_T> MenuStack;

//void OpenPauseMenu();
//Menu *pauseMenu;

bool _Dialogue;	//are we currently talking?
Menu *quitMenu;


typedef enum Program_Mode{
	M_GAME,
	M_EDIT_MAP,
	M_EDIT_CHAR,
};

const Program_Mode _Mode = M_GAME;

int main (int argc, char* argv[]){
	done = 0;

	/*if(DEBUG_BREAK){
		DEBUG_BREAK_FUNC();
		while(true);
	}*/


	InitGame();

	if(_Mode == M_GAME){
		do{
			//The basic game loop:
			//read inputs
			//update game
			//draw everything
			PollEvents();
			UpdateGame();		
			DrawGame();	
			Graphics::NextFrame();
		}while(!done);
	}
	else if(_Mode == M_EDIT_MAP){
		do{
			done = true;
		}while(!done);
	}

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

	InitSpriteList();
	InitMusicList();
	InitSoundList();
	InitChunkList();

	InitFont();
	
	for(int i = 0; i < MAX_CHARACTERS; i++){
		CharList[i] = NULL;
	}
	
	numMenus = 0;
/////////////////////
//	InitCombat();

	InitMainTextbox(&mainTextbox,3,LoadSprite(SPATH_MAIN_TEXTBOX,160,35,1));

	LoadPauseMenu();

	quitMenu = LoadCustomMenu(2);
	SetMenuItemAction(quitMenu->items[0],ExitGame,"Exit Game");
	SetMenuItemAction(quitMenu->items[1],CancelMenu,"Return");

/////////////////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//test stuff goes between the squiggles
	
	_CurrentScene = new Overworld();
	


	LoadDialogue();

	
	//Music *m = LoadMusic("sounds/GV.mp3",40);
	//Sound *s = LoadSound("sounds/gui.wav",35);	
	//PlayMusic(m);
	//PlaySound(s);
	
	_MessageStack = NULL;
/////////////////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/*
	_Player = new NPC(2,3,"Hero");
	Message *msg;
	CreateMessage(msg,". . .",_Player); 
	GiveNPCMessage(_Player,msg);*/
	Sprite *s = LoadSprite("sprites/player.png",32,32,5,16,24);
	CharList[0]->SetEntAnims(s);
	CharList[0]->SetPlayer(true);
	
	SDL_Rect tr = {0,10,40,20};	//x and y are the offset from center
	SDL_Rect sr = {0,10,48,24};	//x and y dont matter
	InitCamera(&mainCamera,GAME_RES_X,GAME_RES_Y,true,CharList[0],&tr,&sr);

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

void ExitGame(int val){
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
	curInput = PollKeyInputs();
	InputNode *inputNode = new InputNode;
	inputNode->prev = _Inputs;//_Player->inputs;
	inputNode->input = curInput;
	_Inputs = inputNode;	
	DeleteInputNode(inputNode, INPUTS_HISTORY);

	if(GetCurrentState()==GS_OVERWORLD){
		if(DEBUG){
			if(InputPressed(PPINPUT_ESC)){			
				if(!CurrentScene()->MenuStack.empty())
					CancelMenu();
				else
					done = 1;
			}
		}else{
			if(InputPressed(PPINPUT_ESC)){			
				if(!CurrentScene()->MenuStack.empty())
					CancelMenu();
				else
					OpenMenu(quitMenu);
			}
		}
	}

}

Uint16 PollKeyInputs(){
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
		
	

	Graphics::RenderCurrentFrame();
}

/*
bool InCombat(){
	return (_SceneStack.back()->type == GS_COMBAT);
}*/
/*
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
*/
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
//	enemylist = LoadEnemyDataCFG("testfiles/enemy-test.json");
//	SetEnemies(2, enemylist.at(1)->chardata->id, enemylist.at(1)->chardata->id);
}

void Overworld::Update(){
	if(InputPressed(PPINPUT_B))
		if(!_Dialogue){
			if(this->MenuStack.empty()){
				OpenPauseMenu();
			//	ShowWallet();
			}else
				//if(this->MenuStack.back()->type == MENU_PAUSE){
					CancelMenu();
				//	HideWallet();
			//	}
		}
	UpdateWorld();

	if(!this->MenuStack.empty()){
		_CurrentMenu = this->MenuStack.back();
		UpdateMenu(this->MenuStack.back());
	}

	for(int i = 0; i < MAX_CHARACTERS; i++){
		if(CharList[i] != NULL)
			CharList[i]->Update();
	}

	
	UpdateCamera(&mainCamera);
}

void Overworld::Draw(){
	DrawWorld();
	if(DEBUG_DRAW_RECTS){
	//	DrawFacingCursor(_Player->tile+_Player->facing);
	}

	if(_Dialogue)
		mainTextbox.Draw();
	if(!this->MenuStack.empty()){
		for(int i = 0; i < (int)this->MenuStack.size(); i++){
			this->MenuStack[i]->Draw();
		}
	}


}








