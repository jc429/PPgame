#ifndef _GAMESTATE_
#define _GAMESTATE_

#include "global.h"
#include "sprites.h"
#include "menu.h"
#include <vector>



class GameScene{
public:
	GameState type;
	vector<Menu*> MenuStack;

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

class StatusScreen : public GameScene{
private:
	Animation *bg;
	vector<UIElement*> UIcontents;
public:
	StatusScreen();
	~StatusScreen();
	void Update();
	void Draw();
};


GameScene *CurrentScene();
GameState GetCurrentState();

#endif 
