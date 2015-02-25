#ifndef _PLAYER_
#define _PLAYER_

#include "settings.h"
#include "pmath.h"
#include "input.h"
#include "sprites.h"
#include "tile.h"



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

	Sprite *sprite; 
	int frame;
	Vec2i s_offset;	

	SDL_Rect collider;
} Player;

typedef struct NonPlayer_T{
	Vec2i facing;
	Vec2i tile;				//tile we are currently standing on
	Vec2i localposition;	//current position within the tile in pixels
	Vec2i worldposition;	//current position within the world in pixels
	Vec2i tomove;			//where we're moving, in tiles
	int movespeed;
	bool moving;

	Vec2i offset;			

	Sprite *sprite; 

	SDL_Rect collider;
} NPC;

union Character{
	Player player;
	NPC npc;
};

Player *CreatePlayer(Player *p);
void UpdatePlayer(Player *p);
void DrawPlayer(Player *p);

void PlayerMovement(Player *p);
bool InputBuffered (InputNode *input, int button, int buf);
void MoveToTile(Player *p, Tile *src, Tile *dest);
void UpdateTile(Player *p);

#endif