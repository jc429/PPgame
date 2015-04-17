#include "player.h"

void StepOutOfTile(Character *c,Tile *tile){
	tile->free = true;
	tile->contents = NULL;
}

void StepIntoTile(Character *c,Tile *tile){
	tile->free = false;
	tile->contents = c;
	
}

