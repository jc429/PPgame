#include "gamestate.h"
#include "level.h"
#include "world.h"
//#include "combat.h"
using std::vector;

GameScene *_CurrentScene;
vector<GameScene*> _SceneStack;
GameState _GameState;

extern Camera uiCamera;

Overworld::Overworld(){
	type = OVERWORLD;

	LoadLevel();

	_SceneStack.push_back(this);
}

Overworld::~Overworld(){
	_SceneStack.pop_back();
}

GameState GetCurrentState(){
	return _SceneStack.back()->type;
}
