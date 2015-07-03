#include "player.h"

void StepOutOfTile(OverworldCharacter *c,Tile *tile){
	if(tile == NULL) return;
	tile->free = true;
	tile->contents = NULL;
}

void StepIntoTile(OverworldCharacter *c,Tile *tile){
	if(tile == NULL) return;

	tile->free = false;
	tile->contents = c;	
}

