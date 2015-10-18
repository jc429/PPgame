#ifndef _ENTITY_
#define _ENTITY_

#include "global.h"
#include "pmath.h"
#include "sprites.h"
#include "dialogue.h"

typedef enum MoveType{
	MoveType_Grid,
	MoveType_Free,
};

typedef enum EntAnim{
	ANIM_ENT_DEFAULT = 0,
};

typedef enum EntType{
	Ent_Character,
	Ent_IntObj,
};

class Entity{
public:
	int entID; //unique integer referring to this entity?
};

class OverworldEnt:public Entity{
private:
	Vec2f worldposition;	//current position within the world in pixel-sized units
public:
	Vec2i facing;
	EntType type;
	int used;
	CharData *chardata;

	//position stuff
	Vec2i tile;				//tile of the world we are currently standing on
	Vec2i tile_src;			//source tile when moving
	Vec2i tile_dest;		//dest tile when moving 
	Vec2f localposition;	//current position within the tile in pixel-sized units
	MoveType moveType;
	bool moving;
	bool movex;
	bool movey;
	
	//graphics stuff
	Animation *animlist[MAX_ANIMS][NUM_ANIM_DIRS]; //all animations this entity can have
	AnimDir direction;				//direction facing (default: south)
	int animation;				//current animation
	int numAnims;					//number of animations this entity is currently using
	bool drawn;				//were we drawn yet this frame?
	
	bool talks;			
	char dialoguepath[64];		//path to the dialogue file for this npc
	bool talking;
	bool passable;

	//functions
	virtual void Update();
	virtual void Draw();
	virtual void Talk(TextboxEX *t);
	void UpdateWorldPosition();
	Vec2f WorldPosition();
	void AddToWorld(int xpos, int ypos);
	void SetEntAnims(Sprite *spr);

};

class InteractableObject: public OverworldEnt{
public:
//	char name[32];
	Message *flavortext;
//	char* flavortext;

	void Update();
	void Draw();
	void Talk(TextboxEX *t);

	InteractableObject(int xpos, int ypos);
	~InteractableObject();
};


void InitEntList();
OverworldEnt* NewOverworldEnt();
void FreeOverworldEnt();
void ClearEntList();

/*** these functions are dumb and probably bad ***/
InteractableObject *LoadSign(int xpos, int ypos);
InteractableObject *LoadEgg(int xpos, int ypos);


#endif