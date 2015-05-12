#include "gamestate.h"
#include "level.h"
#include "world.h"
#include "combat.h"
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

InventoryPage::InventoryPage(){
	type = INVENTORY;
	_SceneStack.push_back(this);
}

InventoryPage::~InventoryPage(){
	_SceneStack.pop_back();
}

PartyView::PartyView(){
	type = PARTY;
	_SceneStack.push_back(this);
}

PartyView::~PartyView(){
	_SceneStack.pop_back();
}

Combat::Combat(){
	type = COMBAT;
	_SceneStack.push_back(this);
	SetupCombat();
}

Combat::~Combat(){
	_SceneStack.pop_back();
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

GameState GetCurrentState(){
	return _SceneStack.back()->type;
}
