#ifndef _PLAYER_
#define _PLAYER_

#include "settings.h"
#include "pmath.h"
#include "input.h"
#include "sprites.h"
#include "tile.h"
#include "entity.h"

class Character: public Entity{
public:
	//world position and movement stuff
	Vec2i facing;
	Vec2i tomove;			//where we're moving, in tiles
	int movespeed;
	bool moving;

	//conversation stuff
	bool talking;


	
};

class Player: public Character{
public:
	InputNode *inputs;
	bool movelock;

	Player();
	~Player();

	
};


class NPC: public Character{
	
public:
	NPC *CreateNPC();
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


//Player *CreatePlayer(Player *p);
void UpdatePlayer(Player *p);
void DrawPlayer(Player *p);

void PlayerMovement(Player *p);
bool InputBuffered (InputNode *input, int button, int buf);
void MoveToTile(Player *p, Tile *src, Tile *dest);
void UpdateTile(Player *p);



#endif