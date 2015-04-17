#ifndef _WORLD_
#define _WORLD_

#include "tile.h"

/* 
Tile - A single Tile in the game world;
Chunk - a fixed size 2d array of Tiles (not Tile pointers) - does not have to be the same size every time, but an individual chunk's size is not dynamic.
Area - a number of chunks "stitched" together, forming whatever location in the game world. All chunks of an area are loaded at once (?)
World - The main world, handles loadind/unloading of areas. Probably loads the current Area and its neighbors 
*/
#define MAX_CHUNK_SIZE_X 32
#define MAX_CHUNK_SIZE_Y 32
#define MAX_CHUNKS 16

typedef struct Chunk_T{
	int used;
	char *path;	
	int numSprites;
	int numTiles;
	Sprite *spritelist[16];
	TileStructure *tilelist[128];

	Vec2i size;
	Tile tiles[MAX_CHUNK_SIZE_X][MAX_CHUNK_SIZE_Y];
}Chunk;

void ClearTile(Tile *t);

void InitChunkList();
Chunk *LoadChunk(char *chunkpath);
void LoadChunkCFG(Chunk *ch,char *chunkpath);
void FreeChunk(Chunk *ch);


#endif