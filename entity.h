#ifndef _ENTITY_
#define _ENTITY_

#include "global.h"
#include "pmath.h"
#include "sprites.h"
#include "dialogue.h"



typedef enum EntAnim{
	ANIM_ENT_DEFAULT = 0,
};

typedef enum EntType{
	Ent_Player,
	Ent_NPC,
	Ent_IntObj,
	Ent_Enemy,
};

class Entity{
public:
	int entID; //unique integer referring to this entity?
};

class OverworldEnt:public Entity{
public:
	EntType type;
	int used;
	CharData *chardata;

	//position stuff
	Vec2i tile;				//tile we are currently standing on
	Vec2i tile_src;			//source tile when moving
	Vec2i tile_dest;		//dest tile when moving 
	Vec2i localposition;	//current position within the tile in pixels
	Vec2i worldposition;	//current position within the world in pixels
	
	//graphics stuff
//	Vec2i s_offset;	
	Animation *animlist[MAX_ANIMS][NUM_ANIM_DIRS]; //all animations this entity can have
	AnimDir direction;				//direction facing (default: south)
	int animation;				//current animation
	int numAnims;					//number of animations this entity is currently using
	
	bool talks;			
	char dialoguepath[64];		//path to the dialogue file for this npc
	bool talking;
	bool passable;

	virtual void Update();
	virtual void Draw();
	virtual void Talk(Textbox *t);

};

class InteractableObject: public OverworldEnt{
public:
//	char name[32];
	Message *flavortext;
//	char* flavortext;

	void Update();
	void Draw();
	void Talk(Textbox *t);

	InteractableObject(int xpos, int ypos);
	~InteractableObject();
};


InteractableObject *LoadSign(int xpos, int ypos);
InteractableObject *LoadEgg(int xpos, int ypos);

void InitEntList();
OverworldEnt* NewOverworldEnt();
void FreeOverworldEnt();
void ClearEntList();

void AddToWorld(OverworldEnt *e, int xpos, int ypos);

#endif