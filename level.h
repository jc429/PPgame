#ifndef _LEVEL_
#define _LEVEL_
#include "graphics.h"
#include "tile.h"
#include <stdio.h>
#include <string.h>

typedef struct Level_T{
	int width, height;
	int numsprites, numtiles;
	Sprite *sprites[MAX_SPRITES];
	Tile *tiles[MAX_TILES];
}Level;


void LoadLevel();

int LoadCFG(Level *l, char* path);
int LoadLevelDef(Level *l, FILE *fileptr, char *buf);
int LoadSpriteDef(Level *l,FILE *fileptr,char *buf);
int LoadTileDef(Level *l,FILE *fileptr,char *buf);
int LoadMapDef(Level *l,FILE *fileptr,char *buf);
int SaveCFG(Level *l, char* path);



#endif