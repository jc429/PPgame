#include "entity.h"
#include "tile.h"

#include <stdio.h>

Entity EntList[MAX_ENTS];
int numEnts;

extern Tile* World[WORLD_W][WORLD_H];


void Entity::Update(){
	return;
}
void Entity::Talk(Textbox *t){
	return;
}

void AddToWorld(Entity *e, int xpos, int ypos){
	World[xpos][ypos]->contents = e;
	World[xpos][ypos]->free = e->passable;
}

InteractableObject::InteractableObject(int xpos, int ypos){
	talks = true;
	passable = false;
	flavortext = new Message;

	//Position
	tile.x = xpos;
	tile.y = ypos;
	localposition.x = TILE_W>>1;
	localposition.y = TILE_H>>1;
	worldposition.x = tile.x*TILE_W+localposition.x;
	worldposition.y = tile.y*TILE_H+localposition.y;

	//Graphics
	numAnims = 0;
	direction = (AnimDir)0;
	animation = (EntAnim)0;
	for(int i = 0; i < MAX_ANIMS; i++)
		for(int j = 0; j < NUM_ANIM_DIRS; j++)
			animlist[i][j]=NULL;
	//////////////////////////////////////////////////////////////////
	Sprite *s = LoadSprite("sprites/sign.png",32,32,1);
	animlist[0][0] = LoadAnimation(s,0,0,1,1,1);
	numAnims = 1;
	////////////////////////////////////////////////////////////////////////
	s_offset.x = animlist[animation][0]->sprite->w>>1;
	s_offset.y = animlist[animation][0]->sprite->h>>1;

	//Dialogue and misc
	type = Ent_IntObj;

	//get us started on a tile
	AddToWorld(this, xpos, ypos);

	if(DEBUG)
		fprintf(stdout,"Starting on %i %i \n",localposition.x,localposition.y);
}

InteractableObject::~InteractableObject(){
	for(int i = 0; i < numAnims; i++){
		for(int j = 0; j < NUM_ANIM_DIRS; j++)
			FreeAnimation(animlist[i][j]);
	}
}

void InteractableObject::Update(){


}


void InteractableObject::Talk(Textbox *t){
	SetText(flavortext->text,t,1,flavortext->hasPrompt,flavortext);
}


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

