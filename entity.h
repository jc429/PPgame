#ifndef _ENTITY_
#define _ENTITY_

#include "settings.h"

typedef enum EntType{
	Ent_Player,
	Ent_NPC,
	Ent_Sign,
	Ent_Enemy,
};


typedef struct Entity_T{
	EntType type;
	int used;

}Entity;

void InitEntList();
Entity* NewEntity();
void FreeEntity();
void ClearEntList();


#endif