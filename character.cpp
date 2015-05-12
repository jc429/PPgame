#include "player.h"

void StepOutOfTile(OverworldCharacter *c,Tile *tile){
	tile->free = true;
	tile->contents = NULL;
}

void StepIntoTile(OverworldCharacter *c,Tile *tile){
	tile->free = false;
	tile->contents = c;
	
}

