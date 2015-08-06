#ifndef _CHARACTER_
#define _CHARACTER_


#include "global.h"
#include "pmath.h"
#include "input.h"
#include "sprites.h"
#include "tile.h"
#include "chardata.h"
#include "entity.h"


typedef enum CharAnim{
	ANIM_CHAR_IDLE = 0,
	ANIM_CHAR_WALK = 1,
};

class OverworldCharacter: public OverworldEnt{
private:	
	bool isPlayer;

public:
	//world position and movement stuff
	Vec2i facing;
	Vec2i tomove;			//where we're moving, in tiles
	float movespeed;
	bool movelock;

	//animation stuff 
//	CharAnim animation;

	//other
	OverworldCharacter(int xpos, int ypos, char *entName);
	void Movement();
	void Update();
	void Draw();

	void SetPlayer(bool p);
	bool IsPlayer(){
		return isPlayer;
	}
};

void StepOutOfTile(OverworldCharacter *c,Tile *tile);
void StepIntoTile(OverworldCharacter *c,Tile *tile);
void MoveToTile(OverworldCharacter *c, Tile *src, Tile *dest);
void UpdateTile(OverworldCharacter *c);
void UpdateDirection(OverworldCharacter *c);

void TransferPlayer(OverworldCharacter *src, OverworldCharacter *dest);

#endif