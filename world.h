#ifndef _WORLD_
#define _WORLD_

#include "global.h"
#include "tile.h"

/* 
Tile - A single Tile in the game world;
Chunk - a fixed size 2d array of Tiles (not Tile pointers) - does not have to be the same size every time, but an individual chunk's size is not dynamic.
Area - a number of chunks "stitched" together, forming whatever location in the game world. All chunks of an area are loaded at once (?)
World - The main world, handles loadind/unloading of areas. Probably loads the current Area and its neighbors 
*/


typedef struct Chunk_T{
	int used;			//how many areas the chunk is currently being used in
	char *path;			//file path to the chunk - one chunk per file max 
	int numSprites;		//number of different spritesheets (tilemaps) in the chunk - max 16 
	int numTiles;		//number of unique tiles in the chunk - max 128
	Sprite *spritelist[16];
	TileStructure *tilelist[128];

	Vec2i location;	//location within the area (in tiles)

	Vec2i size;		//size in tiles
	Tile tiles[MAX_CHUNK_SIZE_X][MAX_CHUNK_SIZE_Y];
}Chunk;

void ClearTile(Tile *t);

void InitChunkList();
Chunk *LoadChunk(char *chunkpath, Vec2i location);
void LoadChunkCFG(Chunk *ch,char *chunkpath);
void FreeChunk(Chunk *ch);

typedef struct Area_T{
	int area_id;		//unique identifier
	char *path;			//filepath to the area's config
	int numchunks;		//how many chunks are being used
	Chunk *chunklist[MAX_CHUNKS];

}Area;

Area *LoadArea(char *areapath);
int LoadAreaCFG(Area *area, char *areapath, char chunkpaths[MAX_CHUNKS][40], Vec2i chunklocs[MAX_CHUNKS]);

void InitWorld();
void UpdateWorld();


void LoadLevel();

#endif