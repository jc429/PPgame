#include "gamestate.h"
#include "level.h"
#include "world.h"
using std::vector;

GameScene *_CurrentScene;
vector<GameScene*> _SceneStack;
GameState _GameState;



Overworld::Overworld(){
	type = OVERWORLD;

	LoadLevel();

	_SceneStack.push_back(this);
}

Overworld::~Overworld(){
	_SceneStack.pop_back();
}

