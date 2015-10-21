#include "player.h"
#include "camera.h"
#include "gamestate.h"
#include <iostream>

extern Tile *World[WORLD_W][WORLD_H];
extern Camera mainCamera;
extern OverworldCharacter *_Player;
extern TextboxEX mainTextbox; //the main dialogue box for now
extern InputNode *_Inputs;
extern OverworldCharacter *CharList[MAX_CHARACTERS];
extern bool _Dialogue;	//are we currently talking?

//extern vector<struct Menu_T*> _OverworldMenuStack;
/*
Player::Player(int xpos, int ypos, char *entName):NPC(xpos,ypos, entName){
	copy_string(chardata->name,entName);

	SetPlayer(true);

	//Inputs
	inputs = new InputNode;
	inputs->input = 0;
	inputs->prev = NULL;

	//Movement
	movelock = false;

	

	//Graphics
	numAnims = 0;
	animation = ANIM_CHAR_IDLE;
	for(int i = 0; i < MAX_ANIMS; i++)
		for(int j = 0; j < NUM_ANIM_DIRS; j++)
			animlist[i][j]=NULL;
	////////////////////////////////////////////////////////////////// there has to be a better way to do this
	Sprite *s = LoadSprite(SPATH_PLAYER_OVERWORLD,32,32,5,16,20);
	animlist[ANIM_CHAR_IDLE][ANIM_DIR_S] = LoadAnimation(s,0,0,1,1,1,12);
	animlist[ANIM_CHAR_IDLE][ANIM_DIR_SE] = LoadAnimation(s,5,5,1,1,1,12);
	animlist[ANIM_CHAR_IDLE][ANIM_DIR_E] = LoadAnimation(s,5,5,1,1,1,12);
	animlist[ANIM_CHAR_IDLE][ANIM_DIR_NE] = LoadAnimation(s,5,5,1,1,1,12);
	animlist[ANIM_CHAR_IDLE][ANIM_DIR_N] = LoadAnimation(s,10,10,1,1,1,12);
	animlist[ANIM_CHAR_IDLE][ANIM_DIR_NW] = LoadAnimation(s,15,15,1,1,1,12);
	animlist[ANIM_CHAR_IDLE][ANIM_DIR_W] = LoadAnimation(s,15,15,1,1,1,12);
	animlist[ANIM_CHAR_IDLE][ANIM_DIR_SW] = LoadAnimation(s,15,15,1,1,1,12);

	animlist[ANIM_CHAR_WALK][ANIM_DIR_S] = LoadAnimation(s,0,0,4,1,1,12);
	animlist[ANIM_CHAR_WALK][ANIM_DIR_SE] = LoadAnimation(s,5,5,4,1,1,12);
	animlist[ANIM_CHAR_WALK][ANIM_DIR_E] = LoadAnimation(s,5,5,4,1,1,12);
	animlist[ANIM_CHAR_WALK][ANIM_DIR_NE] = LoadAnimation(s,5,5,4,1,1,12);
	animlist[ANIM_CHAR_WALK][ANIM_DIR_N] = LoadAnimation(s,10,10,4,1,1,12);
	animlist[ANIM_CHAR_WALK][ANIM_DIR_NW] = LoadAnimation(s,15,15,4,1,1,12);
	animlist[ANIM_CHAR_WALK][ANIM_DIR_W] = LoadAnimation(s,15,15,4,1,1,12);
	animlist[ANIM_CHAR_WALK][ANIM_DIR_SW] = LoadAnimation(s,15,15,4,1,1,12);
	//animlist[1] = LoadAnimation(s2,0,0,2,1,0);

	numAnims = 2;
	////////////////////////////////////////////////////////////////////////

}*/
/*
Player::~Player(){
	for(int i = 0; i < numAnims; i++){
		for(int j = 0; j < NUM_ANIM_DIRS; j++)
			FreeAnimation(animlist[i][j]);
	}
}*/



void UpdatePlayer(OverworldCharacter *p){
	p->drawn = false;

	if(_Dialogue || p->talking){
		if(InputPressed(PPINPUT_TEXT_ADVANCE,_Inputs)){	
			if(mainTextbox.donewriting){
				if(mainTextbox.msg){
					if(!mainTextbox.msg->next[0]){
						p->talking = false;
						_Dialogue = false;
						if(CheckContentsTalk(World[p->tile.x+p->facing.x][p->tile.y+p->facing.y])){
							World[p->tile.x+p->facing.x][p->tile.y+p->facing.y]->contents->talking = false;
							if(mainTextbox.msg->atEnd != NULL)
								mainTextbox.msg->atEnd();
						}

					}else{
						if(!mainTextbox.msg->hasPrompt){
							if(mainTextbox.msg->atEnd == NULL)
								AdvanceText();
							else
								mainTextbox.msg->atEnd;
						}
					}
				}else{
					p->talking = false;
					_Dialogue = false;
					if(CheckContentsTalk(World[p->tile.x+p->facing.x][p->tile.y+p->facing.y])){
						World[p->tile.x+p->facing.x][p->tile.y+p->facing.y]->contents->talking = false;
					}
				}
			}
		}
		
	}
	else{
		if(InputPressed(PPINPUT_A,_Inputs)){
			//soooo many conditions to check
			if(CurrentScene()->MenuStack.empty() && GetCurrentState() == GS_OVERWORLD){
			if(World[p->tile.x+p->facing.x][p->tile.y+p->facing.y]!=NULL){
			if(CheckTileHeights(World[p->tile.x+p->facing.x][p->tile.y+p->facing.y],World[p->tile.x][p->tile.y])){
			if(World[p->tile.x+p->facing.x][p->tile.y+p->facing.y]->contents!=NULL){
			if(World[p->tile.x+p->facing.x][p->tile.y+p->facing.y]->contents->talks){
			if(!p->talking && !p->moving && !_Dialogue){
			if(World[p->tile.x+p->facing.x][p->tile.y+p->facing.y]->contents != p){
				_Dialogue = true;
				p->talking = true;
				World[p->tile.x+p->facing.x][p->tile.y+p->facing.y]->contents->Talk(&mainTextbox);
				TransferPlayer(World[p->tile.x][p->tile.y]->character,World[p->tile.x+p->facing.x][p->tile.y+p->facing.y]->character);
			}
			}
			}
			}
			}
			}
			}
		}
	}

	if((!p->talking)&&(!p->movelock)){
		switch(_GlobalMovementType){
		case MovementType_Grid:
			PlayerMovementGrid(p);
			break;
		case MovementType_Free:
			PlayerMovementFree(p);
			break;
		}

	}
}

Vec2i GetIntendedMovement(OverworldCharacter *p){
	Vec2i want_to_move(0,0);
	if((_Inputs->input & PPINPUT_LEFT)&&!(_Inputs->input & PPINPUT_RIGHT)){
		p->facing.x = -1;
		if(_GlobalMovementType != MovementType_Grid || InputBuffered(_Inputs, PPINPUT_LEFT, INPUT_BUFFER)
			||InputBuffered(_Inputs, PPINPUT_UP, INPUT_BUFFER)||InputBuffered(_Inputs, PPINPUT_DOWN, INPUT_BUFFER))
			want_to_move.x = -1;
	}
	else if((_Inputs->input & PPINPUT_RIGHT)&&!(_Inputs->input & PPINPUT_LEFT)){
		p->facing.x = 1;
		if(_GlobalMovementType != MovementType_Grid || InputBuffered(_Inputs, PPINPUT_RIGHT, INPUT_BUFFER)
			||InputBuffered(_Inputs, PPINPUT_UP, INPUT_BUFFER)||InputBuffered(_Inputs, PPINPUT_DOWN, INPUT_BUFFER))
			want_to_move.x = 1;
	}
	if((_Inputs->input & PPINPUT_UP)&&!(_Inputs->input & PPINPUT_DOWN)){
		p->facing.y = -1;
		if(_GlobalMovementType != MovementType_Grid || InputBuffered(_Inputs, PPINPUT_UP, INPUT_BUFFER)
			||InputBuffered(_Inputs, PPINPUT_LEFT, INPUT_BUFFER)||InputBuffered(_Inputs, PPINPUT_RIGHT, INPUT_BUFFER))
			want_to_move.y = -1;
	}
	else if((_Inputs->input & PPINPUT_DOWN)&&!(_Inputs->input & PPINPUT_UP)){
		p->facing.y = 1;
		if(_GlobalMovementType != MovementType_Grid || InputBuffered(_Inputs, PPINPUT_DOWN, INPUT_BUFFER)
			||InputBuffered(_Inputs, PPINPUT_LEFT, INPUT_BUFFER)||InputBuffered(_Inputs, PPINPUT_RIGHT, INPUT_BUFFER))
			want_to_move.y = 1;
	}
	return want_to_move;
}

void PlayerMovementGrid(OverworldCharacter *p){
	if(!p->moving){
		if(!CurrentScene()->MenuStack.empty())
			return;
		p->UpdateTile();

		p->animation = ANIM_CHAR_IDLE;
		p->tomove.x = 0;
		p->tomove.y = 0;
		if(_Inputs->input & PPINPUT_DIR_ANY){	
			//if any directions are being held, reset our facing to 0,0 (nothing) so we can recalculate it
			//otherwise just use the previous facing coordinates
			p->facing.x = 0;
			p->facing.y = 0;
		}
		Vec2i want_to_move = GetIntendedMovement(p);
		
		if(want_to_move.x * want_to_move.y != 0){ //if we want to move diagonally, 
			if((CheckTileAvailable(World[p->tile.x+want_to_move.x][p->tile.y+want_to_move.y])) //and we can move diagonally,
			&&(CheckTileHeights(World[p->tile.x+want_to_move.x][p->tile.y+want_to_move.y],World[p->tile.x][p->tile.y]))){
				//check that an adjacent tile is free to pass through
				if(((CheckTileAvailable(World[p->tile.x+want_to_move.x][p->tile.y]))
				&&!(CheckTileAvailable(World[p->tile.x+want_to_move.x][p->tile.y+want_to_move.y])))
				||!(CheckTileHeights(World[p->tile.x][p->tile.y+want_to_move.y],World[p->tile.x][p->tile.y]))){
					want_to_move.y = 0;
				}
				if(((CheckTileAvailable(World[p->tile.x][p->tile.y+want_to_move.y]))
				&&!(CheckTileAvailable(World[p->tile.x+want_to_move.x][p->tile.y+want_to_move.y])))
				||!(CheckTileHeights(World[p->tile.x+want_to_move.x][p->tile.y],World[p->tile.x][p->tile.y]))){
					want_to_move.x = 0;
				}	
			}else{	//if the diagonal tile isnt free
				if((CheckTileAvailable(World[p->tile.x+want_to_move.x][p->tile.y]))
				&&(CheckTileHeights(World[p->tile.x+want_to_move.x][p->tile.y],World[p->tile.x][p->tile.y]))){		//try going horizontal
					want_to_move.y = 0;
				}		
				else if((CheckTileAvailable(World[p->tile.x][p->tile.y+want_to_move.y]))
				&&(CheckTileHeights(World[p->tile.x][p->tile.y+want_to_move.y],World[p->tile.x][p->tile.y]))){
					want_to_move.x = 0;
				}	
			}
		}
		if(CheckTileAvailable(World[p->tile.x+want_to_move.x][p->tile.y+want_to_move.y])){
			if(CheckTileHeights(World[p->tile.x+want_to_move.x][p->tile.y+want_to_move.y],World[p->tile.x][p->tile.y]))			
				p->tomove = want_to_move;
		}	
//		printf("%i, %i \n",want_to_move.x,want_to_move.y);


		if((p->tomove.x!=0)||(p->tomove.y!=0)){
			p->animation = ANIM_CHAR_WALK;
			p->moving=true;
			p->movex = (p->tomove.x != 0);
			p->movey = (p->tomove.y != 0);
			p->UpdateTile();
			p->tile_src.x = p->tile.x;
			p->tile_dest.x = p->tile.x+p->tomove.x;
			p->tile_src.y =  p->tile.y;
			p->tile_dest.y = p->tile.y+p->tomove.y;
			p->MoveToTile(World[p->tile_src.x][p->tile_src.y],World[p->tile_dest.x][p->tile_dest.y]);
			
			p->facing = p->tomove;
		}
	}

	p->UpdateDirection();
//	fprintf(stdout,"%i\n",p->direction);
	p->UpdateWorldPosition();
//	p->worldposition.x = p->tile.x*TILE_W+p->localposition.x;
//	p->worldposition.y = p->tile.y*TILE_H+p->localposition.y - (World[p->tile.x][p->tile.y]->structure->height<<2);
}

void PlayerMovementFree(OverworldCharacter *p){
	p->tomove = GetIntendedMovement(p);
	
	if(!CurrentScene()->MenuStack.empty())
		return;

	if((p->tomove.x != 0)||(p->tomove.y!=0)){
		p->animation = ANIM_CHAR_WALK;
		p->moving = true;
		p->movex = (p->tomove.x != 0);
		p->movey = (p->tomove.y != 0);
		p->UpdateTile();

	
		p->facing = p->tomove;
	}else{
		p->animation = ANIM_CHAR_IDLE;
		p->moving = false;
	}
	
}