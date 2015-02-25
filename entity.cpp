#include "entity.h"



Entity* NewEntity(){
	Entity *ent = new Entity;



	numEnts++;
	EntList[numEnts] = ent;
	return ent;
}

void FreeEntity(Entity *ent){
	for(int i = 0; i < ENTS_MAX; i++){
		if(ent==EntList[i]){
			free(EntList[i]);
			numEnts--;
		}
	}
}