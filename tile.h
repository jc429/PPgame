#ifndef _TILE_
#define _TILE_
#include "pmath.h"
#include "sprites.h"

//typedef union Character;

typedef struct Tile_T{
	Vec2i position;		//world position
	bool free;				//is the tile unoccupied?
	Sprite *lowerspr;		//sprite below player
	Sprite *upperspr;		//sprite above player
//	Character *contents;	//character in the tile
	int lowerframe;			//frame of animation we're on
	int upperframe;			//frame of animation we're on

	Sprite *debugFill;		//just shows whether a sprite is occupied or not
}Tile;

Tile* LoadTile(bool fr, Sprite *lower, Sprite *upper);
void DrawTilesLower();
void DrawTilesUpper();

#endif