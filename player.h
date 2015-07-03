#ifndef _PLAYER_
#define _PLAYER_

#include "global.h"
#include "pmath.h"
#include "input.h"
#include "sprites.h"
#include "tile.h"
#include "entity.h"
#include "chardata.h"

typedef enum CharAnim{
	ANIM_CHAR_IDLE = 0,
	ANIM_CHAR_WALK = 1,
};

class OverworldCharacter: public OverworldEnt{
public:
	//world position and movement stuff
	Vec2i facing;
	Vec2i tomove;			//where we're moving, in tiles
	int movespeed;
	bool moving;

	//animation stuff 
//	CharAnim animation;


};

class Player: public OverworldCharacter{
public:
	InputNode *inputs;
	bool movelock;
	CharData *chardata;

	Player();
	~Player();
	
	void Update();
	void Draw();
};

class NPC: public OverworldCharacter{
public:
	char name[32];
	bool rotates; //does this NPC turn when they speak?
	Message *msg;	//any dialogue they have gets stored here 
	int actiontimer;	//counts down to 0 then does an something (maybe) 


	NPC (int xpos, int ypos, char *entName = NULL);
	~NPC();

	void Update();
	void Draw();
	void Talk(Textbox *t);
};

/*   keeping for reference 
typedef struct Player_T{
	InputNode *inputs;

	Vec2i facing;
	Vec2i tile;				//tile we are currently standing on
	Vec2i localposition;	//current position within the tile in pixels
	Vec2i worldposition;	//current position within the world in pixels
	Vec2i tomove;			//where we're moving, in tiles
	int movespeed;
	bool moving;


	bool talking;

//	Sprite *sprite; 
//	int frame;
	Vec2i s_offset;	
	Animation *animlist[MAX_ANIMS]; //all animations this entity can have
	int animation;					//current animation
	int numAnims;					//number of animations this entity is currently using

	SDL_Rect collider;
} Player;*/


NPC **LoadEntitiesCFG(char *path);



void UpdatePlayer(Player *p);
void DrawPlayer(Player *p);

void PlayerMovement(Player *p);

void StepOutOfTile(OverworldCharacter *c,Tile *tile);
void StepIntoTile(OverworldCharacter *c,Tile *tile);
void MoveToTile(OverworldCharacter *c, Tile *src, Tile *dest);
void UpdateTile(OverworldCharacter *c);
void UpdateDirection(OverworldCharacter *c);

void GiveNPCMessage(NPC *npc, Message *msg);

#endif