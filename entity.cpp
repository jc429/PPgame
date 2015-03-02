#include "entity.h"
#include <stdio.h>

Entity EntList[MAX_ENTS];
int numEnts;

void InitEntList(){
	int i;
	for(i = 0;i < MAX_ENTS;i++){

	}
}

Entity *NewEntity(){
	int i;
	if(numEnts+1 >= MAX_ENTS){
		fprintf(stderr, "Too many Ents!\n");
		//exit(1);
		return NULL;
	}
	numEnts++;
	for(i = 0;i < MAX_ENTS;i++){
		if(!EntList[i].used)
			break;
	}
	EntList[i].used++;
	return &EntList[i];
}

void FreeEntity(Entity *ent){
	if(ent->used > 1) {
		ent->used--;
		return;
	}
	numEnts--;
	ent->used--;
	ent = NULL;
}

void ClearEntList(){
	int i;
	for(i = 0;i < MAX_ENTS;i++){

	}
	numEnts = 0;
}