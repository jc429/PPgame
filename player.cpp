#include "player.h"
#include "camera.h"
#include <iostream>

extern Tile *World[WORLD_W][WORLD_H];
extern Camera mainCamera;
extern Player *_Player;
extern Textbox mainTextbox; //the main dialogue box for now
extern InputNode *_Inputs;
extern bool _Dialogue;	//are we currently talking?

int playerid;

Player::Player(){
	playerid = ID_PARTY;
	chardata = LoadCharData(playerid);
	copy_string(chardata->name,"Player");

	AddToCombatParty(chardata->id);

	//Inputs
	inputs = new InputNode;
	inputs->input = 0;
	inputs->prev = NULL;

	//Facing
	facing.x = 0;		//These should never both be zero
	facing.y = 1;		//Face south to begin i guess
	direction = ANIM_DIR_S;

	//Movement
	tomove.x = 0;	
	tomove.y = 0;
	moving = false;
	movelock = false;

	//Speeds and stuff
	movespeed = 2;
	
	//Position
	tile.x = 1;
	tile.y = 3;
	localposition.x = TILE_W>>1;
	localposition.y = TILE_H>>1;

	//Graphics
	numAnims = 0;
	animation = ANIM_CHAR_IDLE;
	for(int i = 0; i < MAX_ANIMS; i++)
		for(int j = 0; j < NUM_ANIM_DIRS; j++)
			animlist[i][j]=NULL;
	////////////////////////////////////////////////////////////////// there has to be a better way to do this
	Sprite *s = LoadSprite(SPATH_PLAYER_OVERWORLD,64,64,5,32,40);
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
	/*s_offset.x = animlist[animation][0]->sprite->w>>1;
	s_offset.y = animlist[animation][0]->sprite->h>>1;
	s_offset.y += 8;*/

	//Dialogue and misc
	talking = false;
	passable = false;
	type = Ent_Player;
//	name = NULL;

	//get us started on a tile
	UpdateTile(this);
	if(DEBUG){
//		printf("Starting on %i %i \n",tile.x,tile.y);
	}
}

Player::~Player(){
	for(int i = 0; i < numAnims; i++){
		for(int j = 0; j < NUM_ANIM_DIRS; j++)
			FreeAnimation(animlist[i][j]);
	}
}

void Player::Update(){


}

void Player::Draw(){
	DrawPlayer(this);
}


void UpdatePlayer(Player *p){
	p->inputs = _Inputs;
//	World[p->tile.x][p->tile.y]->contents = p;
	if(_Dialogue || _Player->talking){
		if(InputPressed(PPINPUT_TEXT_ADVANCE,_Player->inputs)){	
			if(mainTextbox.donewriting){
				if(mainTextbox.msg){
					if(!mainTextbox.msg->next[0]){
						_Player->talking = false;
						_Dialogue = false;
						if(CheckContentsTalk(World[_Player->tile.x+_Player->facing.x][_Player->tile.y+_Player->facing.y])){
							World[_Player->tile.x+_Player->facing.x][_Player->tile.y+_Player->facing.y]->contents->talking = false;
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
					_Player->talking = false;
					_Dialogue = false;
					if(CheckContentsTalk(World[_Player->tile.x+_Player->facing.x][_Player->tile.y+_Player->facing.y])){
						World[_Player->tile.x+_Player->facing.x][_Player->tile.y+_Player->facing.y]->contents->talking = false;
					}
				}
			}
		}
	}
	else{
		if(InputPressed(PPINPUT_A,_Player->inputs)){
			if(World[_Player->tile.x+_Player->facing.x][_Player->tile.y+_Player->facing.y]!=NULL){
				if(CheckTileHeights(World[_Player->tile.x+_Player->facing.x][_Player->tile.y+_Player->facing.y],World[_Player->tile.x][_Player->tile.y])){
				if(World[_Player->tile.x+_Player->facing.x][_Player->tile.y+_Player->facing.y]->contents!=NULL){
					if(World[_Player->tile.x+_Player->facing.x][_Player->tile.y+_Player->facing.y]->contents->talks){
						if(!_Player->talking && !_Dialogue){
							_Dialogue = true;
							_Player->talking = true;
							World[_Player->tile.x+_Player->facing.x][_Player->tile.y+_Player->facing.y]->contents->Talk(&mainTextbox);
						}
					}
				}
			}
			}
		}
	}

	//UpdateTile(p);
	if((!p->talking)&&(!p->movelock))
		PlayerMovement(p);
}

void PlayerMovement(Player *p){

	if(p->moving){
		if(p->animation != ANIM_CHAR_WALK)
			p->animation = ANIM_CHAR_WALK;
		p->facing = p->tomove;
		UpdateTile(p);
		MoveToTile(p,World[p->tile_src.x][p->tile_src.y],World[p->tile_dest.x][p->tile_dest.y]);

	}
	else{
		p->animation = ANIM_CHAR_IDLE;
		p->tomove.x = 0;
		p->tomove.y = 0;
		if(p->inputs->input & PPINPUT_DIR_ANY){	
			//if any directions are being held, reset our facing to 0,0 (nothing) so we can recalculate it
			//otherwise just use the previous facing coordinates
			p->facing.x = 0;
			p->facing.y = 0;
		}
		Vec2i want_to_move;
		SetVec2i(want_to_move,0,0);
		if((p->inputs->input & PPINPUT_LEFT)&&!(p->inputs->input & PPINPUT_RIGHT)){
			p->facing.x = -1;
			if(InputBuffered(p->inputs, PPINPUT_LEFT, INPUT_BUFFER)||InputBuffered(p->inputs, PPINPUT_UP, INPUT_BUFFER)||InputBuffered(p->inputs, PPINPUT_DOWN, INPUT_BUFFER))
				want_to_move.x = -1;
		}
		else if((p->inputs->input & PPINPUT_RIGHT)&&!(p->inputs->input & PPINPUT_LEFT)){
			p->facing.x = 1;
			if(InputBuffered(p->inputs, PPINPUT_RIGHT, INPUT_BUFFER)||InputBuffered(p->inputs, PPINPUT_UP, INPUT_BUFFER)||InputBuffered(p->inputs, PPINPUT_DOWN, INPUT_BUFFER))
				want_to_move.x = 1;
		}
		if((p->inputs->input & PPINPUT_UP)&&!(p->inputs->input & PPINPUT_DOWN)){
			p->facing.y = -1;
			if(InputBuffered(p->inputs, PPINPUT_UP, INPUT_BUFFER)||InputBuffered(p->inputs, PPINPUT_LEFT, INPUT_BUFFER)||InputBuffered(p->inputs, PPINPUT_RIGHT, INPUT_BUFFER))
				want_to_move.y = -1;
		}
		else if((p->inputs->input & PPINPUT_DOWN)&&!(p->inputs->input & PPINPUT_UP)){
			p->facing.y = 1;
			if(InputBuffered(p->inputs, PPINPUT_DOWN, INPUT_BUFFER)||InputBuffered(p->inputs, PPINPUT_LEFT, INPUT_BUFFER)||InputBuffered(p->inputs, PPINPUT_RIGHT, INPUT_BUFFER))
				want_to_move.y = 1;
		}
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
			UpdateTile(p);
			p->tile_src.x = p->tile.x;
			p->tile_dest.x = p->tile.x+p->tomove.x;
			p->tile_src.y =  p->tile.y;
			p->tile_dest.y = p->tile.y+p->tomove.y;
			MoveToTile(p,World[p->tile_src.x][p->tile_src.y],World[p->tile_dest.x][p->tile_dest.y]);
		}
	}
	
	UpdateDirection(p);
//	fprintf(stdout,"%i\n",p->direction);
	
	p->worldposition.x = p->tile.x*TILE_W+p->localposition.x;
	p->worldposition.y = p->tile.y*TILE_H+p->localposition.y - (World[p->tile.x][p->tile.y]->structure->height<<2);
}


void MoveToTile(OverworldCharacter *c, Tile *src, Tile *dest){
	StepIntoTile(c,dest);
	//fprintf(stdout,"moving from %i, %i to %i, %i \n",src->position.x/TILE_W,src->position.y/TILE_H,dest->position.x/TILE_W,dest->position.y/TILE_H);
	Vec2i movement;
	movement.x = 0;
	movement.y = 0;
	if(src->position.x < dest->position.x)
		movement.x = c->movespeed;
	else if(src->position.x > dest->position.x)
		movement.x = -c->movespeed;
	if(src->position.y < dest->position.y)
		movement.y = c->movespeed;
	else if(src->position.y > dest->position.y)
		movement.y = -c->movespeed;

	c->localposition = c->localposition + movement;

//	dest->contents = c;

	if((std::abs(c->localposition.x)%TILE_W==0.5*TILE_W)&&(std::abs(c->localposition.y)%TILE_H==0.5*TILE_H)){
	//	UpdateTile(c);
		c->moving = false;
	}
}

void UpdateTile(OverworldCharacter *c){
	StepOutOfTile(c,World[c->tile.x][c->tile.y]);
	if(c->localposition.x > TILE_W){
		c->tile.x++;
		c->localposition.x -= TILE_W;
	}
	if(c->localposition.x < 0){
		c->tile.x--;
		c->localposition.x += TILE_W;
	}
	if(c->localposition.y > TILE_H){
		c->tile.y++;
		c->localposition.y -= TILE_H;
	}
	if(c->localposition.y < 0){
		c->tile.y--;
		c->localposition.y += TILE_H;
	}
	StepIntoTile(c,World[c->tile.x][c->tile.y]);
}

void DrawPlayer(Player *p){
	if(DEBUG_DRAW_RECTS)
		DrawTile(_Player->tile);

	if(p->animation < 0)
		p->animation = (CharAnim)0;
	if(p->animation+1 > p->numAnims)
		p->animation =  (CharAnim)(p->numAnims-1);

	DrawAnimation(p->animlist[p->animation][p->direction],p->worldposition,&mainCamera);

}

void UpdateDirection(OverworldCharacter *c){
	switch(c->facing.x){
	case 0:
		switch(c->facing.y){
		case 0:
			c->direction = ANIM_DIR_S;
			break;
		case 1:
			c->direction = ANIM_DIR_S;
			break;
		case -1:
			c->direction = ANIM_DIR_N;
			break;
		}
		break;
	case 1:
		switch(c->facing.y){
		case 0:
			c->direction = ANIM_DIR_E;
			break;
		case 1:
			c->direction = ANIM_DIR_SE;
			break;
		case -1:
			c->direction = ANIM_DIR_NE;
			break;
		}
		break;
	case -1:
		switch(c->facing.y){
		case 0:
			c->direction = ANIM_DIR_W;
			break;
		case 1:
			c->direction = ANIM_DIR_SW;
			break;
		case -1:
			c->direction = ANIM_DIR_NW;
			break;
		}
		break;
	}
}