#include "player.h"
#include "camera.h"
#include <iostream>

extern Tile *World[WORLD_W][WORLD_H];
extern Camera mainCamera;
extern OverworldCharacter *_Player;

extern OverworldCharacter *CharList[MAX_CHARACTERS];

NPC::NPC (int xpos, int ypos, char *entName):OverworldCharacter(xpos,ypos, entName){
	
//	name = NULL;
	rotates = true;
	
	moves = false;

		//Dialogue and misc
	talks = false;
	msg = NULL;
	actiontimer = 100;

	//Graphics
	numAnims = 0;
	animation = ANIM_CHAR_IDLE;
	for(int i = 0; i < MAX_ANIMS; i++)
		for(int j = 0; j < NUM_ANIM_DIRS; j++)
			animlist[i][j]=NULL;
	////////////////////////////////////////////////////////////////// there has to be a better way to do this
	Sprite *s = LoadSprite(SPATH_NPC_GENERIC,32,32,5,16,24);
	///Sprite *s2 = LoadSprite("sprites/rainbow.png",32,32,4);
	SetEntAnims(this,s);
	//animlist[1] = LoadAnimation(s2,0,0,2,1,0);

	numAnims = 2;
	////////////////////////////////////////////////////////////////////////
/*	s_offset.x = animlist[animation][0]->sprite->w>>1;
	s_offset.y = animlist[animation][0]->sprite->h>>1;
	s_offset.y += 8;*/




}

void NPC::Update(){
	OverworldCharacter::Update();
	
	if(IsPlayer()) return;

	actiontimer--;
	if(actiontimer <=0){
		if(rotates && !talking){
			facing.x = RandomIntInclusive(-1,1);
			facing.y = RandomIntInclusive(-1,1);
			
			if(moves){
				Vec2i wanttomove;
				wanttomove.x = RandomIntInclusive(-1,1);
				wanttomove.y = RandomIntInclusive(-1,1); 
				Move(wanttomove);
			}
		}
		actiontimer = 100;
	}
	UpdateDirection(this);
}

void NPC::Move(Vec2i want_to_move){
	if(want_to_move.x * want_to_move.y != 0){ //if we want to move diagonally, 
		if((CheckTileAvailable(World[tile.x+want_to_move.x][tile.y+want_to_move.y])) //and we can move diagonally,
		&&(CheckTileHeights(World[tile.x+want_to_move.x][tile.y+want_to_move.y],World[tile.x][tile.y]))){
			//check that an adjacent tile is free to pass through
			if(((CheckTileAvailable(World[tile.x+want_to_move.x][tile.y]))
			&&!(CheckTileAvailable(World[tile.x+want_to_move.x][tile.y+want_to_move.y])))
			||!(CheckTileHeights(World[tile.x][tile.y+want_to_move.y],World[tile.x][tile.y]))){
				want_to_move.y = 0;
			}
			if(((CheckTileAvailable(World[tile.x][tile.y+want_to_move.y]))
			&&!(CheckTileAvailable(World[tile.x+want_to_move.x][tile.y+want_to_move.y])))
			||!(CheckTileHeights(World[tile.x+want_to_move.x][tile.y],World[tile.x][tile.y]))){
				want_to_move.x = 0;
			}	
		}else{	//if the diagonal tile isnt free
			if((CheckTileAvailable(World[tile.x+want_to_move.x][tile.y]))
			&&(CheckTileHeights(World[tile.x+want_to_move.x][tile.y],World[tile.x][tile.y]))){		//try going horizontal
				want_to_move.y = 0;
			}		
			else if((CheckTileAvailable(World[tile.x][tile.y+want_to_move.y]))
			&&(CheckTileHeights(World[tile.x][tile.y+want_to_move.y],World[tile.x][tile.y]))){
				want_to_move.x = 0;
			}	
		}
	}
		
	if(CheckTileAvailable(World[tile.x+want_to_move.x][tile.y+want_to_move.y])){
		if(CheckTileHeights(World[tile.x+want_to_move.x][tile.y+want_to_move.y],World[tile.x][tile.y]))	{		
			tomove = want_to_move;
			moving = true;
			movex = (tomove.x != 0);
			movey = (tomove.y != 0);
		}
	}	
	
	tile_src.x = tile.x;
	tile_dest.x = tile.x + tomove.x;
	tile_src.y =  tile.y;
	tile_dest.y = tile.y + tomove.y;
	if(World[tile_dest.x][tile_dest.y] == NULL)
		tile_dest = tile_src;
}

void NPC::Talk(TextboxEX *t){
	talking = true;
	if(rotates){
		//make our direction mirror the player's (you face someone when you talk to them right)
		Vec2i mirror(-1,-1);
		facing = _Player->facing * mirror;
	}
	UpdateDirection(this);
	if(msg != NULL)
		SetTextEX(msg->text,t,1,msg->hasPrompt,msg);
}


void GiveNPCMessage(NPC *npc, Message *msg){
	if(msg == NULL) return;
	npc->msg = msg;
	npc->talks = true;
	SetSpeaker(msg,npc->chardata);
}