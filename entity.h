#ifndef _ENTITY_
#define _ENTITY_

#include "settings.h"
#include "pmath.h"
#include "sprites.h"
#include "dialogue.h"

typedef enum EntType{
	Ent_Player,
	Ent_NPC,
	Ent_IntObj,
	Ent_Enemy,
};

/*
typedef struct Entity_T{
	EntType type;
	int used;
	
	bool talks;

}Entity;*/

class Entity{
public:
	EntType type;
	int used;

	//position stuff
	Vec2i tile;				//tile we are currently standing on
	Vec2i localposition;	//current position within the tile in pixels
	Vec2i worldposition;	//current position within the world in pixels
	
	//graphics stuff
	Vec2i s_offset;	
	Animation *animlist[MAX_ANIMS]; //all animations this entity can have
	int animation;					//current animation
	int numAnims;					//number of animations this entity is currently using

	bool talks;
	bool passable;

	virtual void Talk(Textbox *t);
};

class InteractableObject: public Entity{
public:
	Message *flavortext;
//	char* flavortext;

	 void Talk(Textbox *t);

	InteractableObject(int xpos, int ypos);
	~InteractableObject();
};



void InitEntList();
Entity* NewEntity();
void FreeEntity();
void ClearEntList();


#endif