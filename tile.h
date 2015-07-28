#ifndef _TILE_
#define _TILE_
#include "pmath.h"
#include "sprites.h"
#include "entity.h"

typedef struct Tile_Structure_T{
//static stuff that is the same across all copies of a certain tile
	int id;

	bool solid;				//self-explanatory i hope
	int height;				/*entities can only move to a tile with a height of their current tile +/- 1, 
							this will make for some really cool 3d-ish things i hope*/
	//Graphics
	Sprite *spritesheet;	//sheet the tile uses 
	int baseframe;			//frame of sheet drawn at height 0
	int floorframe;			//frame of sheet drawn at the tile's height
	int wallframe;			//frame drawn on the side of the sprite
	
//	Sprite *lowerspr;		//sprite below player - depreciate this
//	int lowerframe;			//frame of animation we're on - depreciate this
//	int upperframe;			//frame of animation we're on - depreciate this
}TileStructure;

typedef struct Tile_T{
	TileStructure *structure;
//stuff that is unique to an individual tile
	Vec2i position;			//world position

//changing stuff	
	bool free;				//is the tile unoccupied?
	OverworldEnt *contents;		//who or what is in this tile?
	class OverworldCharacter *character;	//if it's a who put it here
}Tile;

Tile* LoadTile(bool fr, int ht, Sprite *sheet,  int baseframe, int floorframe, int wallframe = 0);
void DrawTilesLower(); //depreciated
void DrawTilesUpper(); //depreciated 

void DrawWorld();
//void DrawWorld2();
void DrawRow(int row, int layer);

bool CheckTileAvailable(Tile *t);
bool CheckTileHeights(Tile *t1, Tile *t2);
bool CheckContentsTalk(Tile *t);

#endif