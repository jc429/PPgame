#ifndef _GAMESTATE_
#define _GAMESTATE_

#include "global.h"
#include "sprites.h"
#include <vector>



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


#endif 
