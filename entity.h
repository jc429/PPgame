#ifndef _ENTITY_
#define _ENTITY_

#include "settings.h"
#include "pmath.h"
#include "sprites.h"
#include "dialogue.h"

#define NUM_ANIM_DIRS 8

typedef enum AnimDir{
	ANIM_DIR_S = 0,
	ANIM_DIR_SW = 1,
	ANIM_DIR_W = 2,
	ANIM_DIR_NW = 3,
	ANIM_DIR_N = 4,
	ANIM_DIR_NE = 5,
	ANIM_DIR_E = 6,
	ANIM_DIR_SE = 7,
};

typedef enum EntAnim{
	ANIM_ENT_DEFAULT = 0,
};

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
	Animation *animlist[MAX_ANIMS][NUM_ANIM_DIRS]; //all animations this entity can have
	AnimDir direction;				//direction facing (default: south)
	int animation;				//current animation
	int numAnims;					//number of animations this entity is currently using

	bool talks;
	bool passable;

	virtual void Update();
	virtual void Talk(Textbox *t);

};

class InteractableObject: public Entity{
public:
	Message *flavortext;
//	char* flavortext;

	void Update();
	void Talk(Textbox *t);

	InteractableObject(int xpos, int ypos);
	~InteractableObject();
};



void InitEntList();
Entity* NewEntity();
void FreeEntity();
void ClearEntList();

void AddToWorld(Entity *e, int xpos, int ypos);

#endif