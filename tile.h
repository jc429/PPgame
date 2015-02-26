#ifndef _TILE_
#define _TILE_
#include "pmath.h"
#include "sprites.h"
#include "entity.h"


typedef struct Tile_T{
	Vec2i position;			//world position
	bool free;				//is the tile unoccupied?

	Entity * contents;		//who or what is in this tile?

	int height;				/*entities can only move to a tile with a height of their current tile +/- 1, 
							this will make for some really cool 3d-ish things i hope*/

	//Graphics
	Sprite *lowerspr;		//sprite below player
	Sprite *upperspr;		//sprite above player
	int lowerframe;			//frame of animation we're on
	int upperframe;			//frame of animation we're on

	Sprite *debugFill;		//just shows whether a sprite is occupied or not
}Tile;

Tile* LoadTile(bool fr, Sprite *lower, Sprite *upper);
void DrawTilesLower();
void DrawTilesUpper();
void DrawRow(int row, int layer);


#endif