#include "entity.h"
#include "tile.h"
#include "graphics.h"

#include <stdio.h>

OverworldEnt EntList[MAX_ENTS];
int numEnts;

extern Tile* World[WORLD_W][WORLD_H];
extern Camera mainCamera;

void OverworldEnt::Update(){
	return;
}
void OverworldEnt::Draw(){
	return;
}
void OverworldEnt::Talk(Textbox *t){
	return;
}

void AddToWorld(OverworldEnt *e, int xpos, int ypos){
	if(World[xpos][ypos] == NULL) return;
	World[xpos][ypos]->contents = e;
	World[xpos][ypos]->free = false;
}

InteractableObject::InteractableObject(int xpos, int ypos){
	talks = true;
	passable = false;
	flavortext = NewMessage();

	//name = NULL;


	

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
	

	//Dialogue and misc
	type = Ent_IntObj;

	//get us started on a tile
	AddToWorld(this, xpos, ypos);
	
//	if(DEBUG)
	//	fprintf(stdout,"Starting on %i %i \n",localposition.x,localposition.y);
}

InteractableObject *LoadSign(int xpos, int ypos){
	InteractableObject *ent = new InteractableObject(xpos,ypos);
	ent->chardata = CreateCharData("Sign");
	Sprite *s = LoadSprite(SPATH_SIGN_GENERIC,32,32,1,16,24);
	ent->animlist[0][0] = LoadAnimation(s,0,0,1,1,1);
	ent->numAnims = 1;

	ent->chardata->portrait = LoadAnimation(LoadSprite("sprites/sign-spk.png",64,64,1,32,64),0,0,1);
	////////////////////////////////////////////////////////////////////////
/*	ent->s_offset.x = ent->animlist[ent->animation][0]->sprite->w>>1;
	ent->s_offset.y = ent->animlist[ent->animation][0]->sprite->h>>1;
	ent->s_offset.y += 8;
	ent->s_offset.y += World[ent->tile.x][ent->tile.y]->structure->height <<2;*/

	return ent;
}

InteractableObject *LoadEgg(int xpos, int ypos){
	InteractableObject *ent = new InteractableObject(xpos,ypos);
	ent->chardata = CreateCharData("Egg");
	Sprite *s = LoadSprite("sprites/egg-over.png",64,64,1,32,36);
	ent->animlist[0][0] = LoadAnimation(s,0,0,1,1,1);
	ent->numAnims = 1;
	////////////////////////////////////////////////////////////////////////
/*	ent->s_offset.x = ent->animlist[ent->animation][0]->sprite->w>>1;
	ent->s_offset.y = ent->animlist[ent->animation][0]->sprite->h>>1;
	ent->s_offset.y += 8;
	ent->s_offset.y += World[ent->tile.x][ent->tile.y]->structure->height <<2;*/

	return ent;
}

InteractableObject::~InteractableObject(){
	for(int i = 0; i < numAnims; i++){
		for(int j = 0; j < NUM_ANIM_DIRS; j++)
			FreeAnimation(animlist[i][j]);
	}
}

void InteractableObject::Update(){
	drawn = false;
}

void InteractableObject::Draw(){
	Vec2i heightoffset;
	SetVec2i(heightoffset,0,World[tile.x][tile.y]->structure->height<<2);
	DrawAnimation(animlist[animation][direction],worldposition - heightoffset,&mainCamera);
}

void InteractableObject::Talk(Textbox *t){
	SetText(flavortext->text,t,1,flavortext->hasPrompt,flavortext);
}


void InitEntList(){
	int i;
	for(i = 0;i < MAX_ENTS;i++){

	}
}

OverworldEnt *NewOverworldEnt(){
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

void FreeOverworldEnt(OverworldEnt *ent){
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

void SetEntAnims(OverworldEnt *ent, Sprite *s){
	ent->animlist[ANIM_CHAR_IDLE][ANIM_DIR_S] = LoadAnimation(s,0,0,1);
	ent->animlist[ANIM_CHAR_IDLE][ANIM_DIR_N] = LoadAnimation(s,5,5,1);
	ent->animlist[ANIM_CHAR_IDLE][ANIM_DIR_W] = LoadAnimation(s,10,10,1);
	ent->animlist[ANIM_CHAR_IDLE][ANIM_DIR_E] = LoadAnimation(s,15,15,1);
	ent->animlist[ANIM_CHAR_IDLE][ANIM_DIR_SW] = LoadAnimation(s,20,20,1);
	ent->animlist[ANIM_CHAR_IDLE][ANIM_DIR_SE] = LoadAnimation(s,25,25,1);
	ent->animlist[ANIM_CHAR_IDLE][ANIM_DIR_NW] = LoadAnimation(s,30,30,1);
	ent->animlist[ANIM_CHAR_IDLE][ANIM_DIR_NE] = LoadAnimation(s,35,35,1);

	ent->animlist[ANIM_CHAR_WALK][ANIM_DIR_S] = LoadAnimation(s,0,0,4);
	ent->animlist[ANIM_CHAR_WALK][ANIM_DIR_N] = LoadAnimation(s,5,5,4);
	ent->animlist[ANIM_CHAR_WALK][ANIM_DIR_W] = LoadAnimation(s,10,10,4);
	ent->animlist[ANIM_CHAR_WALK][ANIM_DIR_E] = LoadAnimation(s,15,15,4);
	ent->animlist[ANIM_CHAR_WALK][ANIM_DIR_SW] = LoadAnimation(s,20,20,4);
	ent->animlist[ANIM_CHAR_WALK][ANIM_DIR_SE] = LoadAnimation(s,25,25,4);
	ent->animlist[ANIM_CHAR_WALK][ANIM_DIR_NW] = LoadAnimation(s,30,30,4);
	ent->animlist[ANIM_CHAR_WALK][ANIM_DIR_NE] = LoadAnimation(s,35,35,4);
}