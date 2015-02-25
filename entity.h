#ifndef _ENTITY_
#define _ENTITY_

#include "pmath.h"
#include "graphics.h"

typedef enum Entity_T{
	block,
	projectile

}Entity_T;

typedef struct {
	Entity_T type;		//what kind of entity is it?
	SDL_Rect bbox;		//entity's bounding box

	//graphical stuff//
	Sprite *sprite;		//the entity's sprite
	Vec2i spr_size;		//sprite tile size
	int t_per_row;		//tiles per row of the sprite sheet
	int frame;			//frame of animation
	int anim_seed;		//first frame of current animation
	int anim_length;	//length of current animation
	int facing;			//-1 if left, 1 if right
	///////////////////

	//position and movement//
	Vec2i position;		
	Vec2i velocity;		
	Vec2i acceleration;	
	/////////////////////////

}Entity;

Entity* EntList[ENTS_MAX];
Entity* NewEntity();
void FreeEntity();

int numEnts;

#endif