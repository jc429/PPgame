#ifndef _ENTITY_
#define _ENTITY_

#include "global.h"
#include "pmath.h"
#include "sprites.h"
#include "dialogue.h"

typedef enum MovementType{
	MovementType_Grid,		//completely grid-based movement, move from tile to tile (like Pokemon)
	MovementType_Free,		//completely free movement, move anywhere and everywhere
	MovementType_Platformer,	//movement with gravity, i.e. a 2D platformer
};

const MovementType _GlobalMovementType = MovementType_Platformer;


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

class OverworldEntity:public Entity{
private:
	Vec2f worldposition;	//current position within the world in pixel-sized units
public:
	Vec2i facing;
	EntType type;
	int used;
	CharData *chardata;

	//position stuff
	Vec2i tile;				//tile of the world we are currently standing on
	Vec2i tile_src;			//source tile when moving	-- Grid Movement only
	Vec2i tile_dest;		//dest tile when moving		-- Grid Movement only
	Vec2f localposition;	//current position within the tile in pixel-sized units
	MovementType MovementType;
	bool moving;
	bool movex;
	bool movey;
	bool grounded;


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
	void SetEntAnims(Sprite *spr);	//idk if this is the best way to do this

};

class InteractableObject: public OverworldEntity{
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
OverworldEntity* NewOverworldEntity();
void FreeOverworldEntity();
void ClearEntList();

/*** these functions are dumb and probably bad ***/
InteractableObject *LoadSign(int xpos, int ypos);
InteractableObject *LoadEgg(int xpos, int ypos);


#endif