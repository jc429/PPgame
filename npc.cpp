#include "player.h"
#include "camera.h"
#include <iostream>

extern Tile *World[WORLD_W][WORLD_H];
extern Camera mainCamera;
extern Player *_Player;

NPC::NPC (int xpos, int ypos, char *entName){

//	name = NULL;

	//Facing
	facing.x = 0;		//These should never both be zero
	facing.y = 1;		//Face south to begin i guess
	direction = ANIM_DIR_S;
	rotates = true;

	//Movement
	tomove.x = 0;	
	tomove.y = 0;
	moving = false;
//	movelock = false;

	//Speeds and stuff
	movespeed = 2;
	
	//Position
	tile.x = xpos;
	tile.y = ypos;
	localposition.x = TILE_W>>1;
	localposition.y = TILE_H>>1;

	//Graphics
	numAnims = 0;
	animation = ANIM_CHAR_IDLE;
	for(int i = 0; i < MAX_ANIMS; i++)
		for(int j = 0; j < NUM_ANIM_DIRS; j++)
			animlist[i][j]=NULL;
	////////////////////////////////////////////////////////////////// there has to be a better way to do this
	Sprite *s = LoadSprite(SPATH_NPC_GENERIC,64,64,5);
	///Sprite *s2 = LoadSprite("sprites/rainbow.png",32,32,4);
	animlist[ANIM_CHAR_IDLE][ANIM_DIR_S] = LoadAnimation(s,0,0,1,1,1);
	animlist[ANIM_CHAR_IDLE][ANIM_DIR_SE] = LoadAnimation(s,5,5,1,1,1);
	animlist[ANIM_CHAR_IDLE][ANIM_DIR_E] = LoadAnimation(s,5,5,1,1,1);
	animlist[ANIM_CHAR_IDLE][ANIM_DIR_NE] = LoadAnimation(s,5,5,1,1,1);
	animlist[ANIM_CHAR_IDLE][ANIM_DIR_N] = LoadAnimation(s,10,10,1,1,1);
	animlist[ANIM_CHAR_IDLE][ANIM_DIR_NW] = LoadAnimation(s,15,15,1,1,1);
	animlist[ANIM_CHAR_IDLE][ANIM_DIR_W] = LoadAnimation(s,15,15,1,1,1);
	animlist[ANIM_CHAR_IDLE][ANIM_DIR_SW] = LoadAnimation(s,15,15,1,1,1);

	animlist[ANIM_CHAR_WALK][ANIM_DIR_S] = LoadAnimation(s,0,0,4,1,1);
	animlist[ANIM_CHAR_WALK][ANIM_DIR_SE] = LoadAnimation(s,5,5,4,1,1);
	animlist[ANIM_CHAR_WALK][ANIM_DIR_E] = LoadAnimation(s,5,5,4,1,1);
	animlist[ANIM_CHAR_WALK][ANIM_DIR_NE] = LoadAnimation(s,5,5,4,1,1);
	animlist[ANIM_CHAR_WALK][ANIM_DIR_N] = LoadAnimation(s,10,10,4,1,1);
	animlist[ANIM_CHAR_WALK][ANIM_DIR_NW] = LoadAnimation(s,15,15,4,1,1);
	animlist[ANIM_CHAR_WALK][ANIM_DIR_W] = LoadAnimation(s,15,15,4,1,1);
	animlist[ANIM_CHAR_WALK][ANIM_DIR_SW] = LoadAnimation(s,15,15,4,1,1);
	//animlist[1] = LoadAnimation(s2,0,0,2,1,0);

	numAnims = 2;
	////////////////////////////////////////////////////////////////////////
	s_offset.x = animlist[animation][0]->sprite->w>>1;
	s_offset.y = animlist[animation][0]->sprite->h>>1;
	s_offset.y += 8;

	//Dialogue and misc
	talks = false;
	talking = false;
	msg = NULL;
	actiontimer = 100;
	passable = false;
	type = Ent_NPC;

	if(entName != NULL)
		strcpy(name,entName);
	else
		strcpy(name,"Some Guy");

	//get us started on a tile
	AddToWorld(this, xpos, ypos);
	UpdateTile(this);

	worldposition.x = tile.x*TILE_W+localposition.x;
	worldposition.y = tile.y*TILE_H+localposition.y;

	if(DEBUG){
//		fprintf(stdout,"NPC on %i %i \n",tile.x,tile.y);
	}
}

void NPC::Update(){
	actiontimer--;
	if(actiontimer <=0){
		if(rotates && !talking){
			facing.x = RandomIntInclusive(-1,1);
			facing.y = RandomIntInclusive(-1,1);
		}
		actiontimer = 100;
	}
	UpdateDirection(this);
}

void NPC::Draw(){
	DrawAnimation(animlist[animation][direction],worldposition-s_offset,&mainCamera);
}

void NPC::Talk(Textbox *t){
	talking = true;
	if(rotates){
		//make our direction mirror the player's (you face someone when you talk to them right)
		Vec2i mirror = {-1,-1};
		facing = _Player->facing * mirror;
	}
	UpdateDirection(this);
	if(msg != NULL)
		SetText(msg->text,t,1,msg->hasPrompt,msg);
}


void GiveNPCMessage(NPC *npc, Message *msg){
	npc->msg = msg;
	npc->talks = true;
	SetSpeaker(msg,npc);
}