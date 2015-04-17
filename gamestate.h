#ifndef _GAMESTATE_
#define _GAMESTATE_

#include "sprites.h"
#include <vector>

typedef enum{
	MAIN_MENU,
	OPTIONS,
	OVERWORLD,
	COMBAT,		
	CUTSCENE,				
	DIALOGUE_OVERLAY,	//Maybe unnecessary?
	DIALOGUE_FULL,
	DATE			
}GameState;

class GameScene{
	
public:
	GameState type;

	virtual void Update(){};
	virtual void Draw(){};
};

class Overworld : public GameScene{
public:
	Overworld();
	~Overworld();
	void Update();
	void Draw();
};

class Combat : public GameScene{
public:
	Combat();
	~Combat();
	void Update();
	void Draw();
};

class CombatTransition : public GameScene{
private:
	Animation *anim, *startanim, *endanim;
	bool combat_entered;
	int timer;
public:
	CombatTransition();
	~CombatTransition();
	void Update();
	void Draw();
	void Combat();
};

void StartCombat();
void EndCombatExit();

#endif 
