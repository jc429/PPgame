#include "player.h"
#include "camera.h"
#include <iostream>

extern Tile *World[WORLD_W][WORLD_H];
extern Camera mainCamera;
extern Player *_Player;
extern Textbox mainTextbox; //the main dialogue box for now
extern InputNode *_Inputs;
extern bool _Dialogue;	//are we currently talking?

Player::Player(){
	
	//Inputs
	inputs = new InputNode;
	inputs->input = 00000000;
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
	Sprite *s = LoadSprite("sprites/player.png",64,64,5);
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

	//Dialogue and misc
	talking = false;
	passable = false;
	type = Ent_Player;

	//get us started on a tile
	UpdateTile(this);
	if(DEBUG)
		fprintf(stdout,"Starting on %i %i \n",localposition.x,localposition.y);
}

Player::~Player(){
	for(int i = 0; i < numAnims; i++){
		for(int j = 0; j < NUM_ANIM_DIRS; j++)
			FreeAnimation(animlist[i][j]);
	}
}

void Player::Update(){


}


void UpdatePlayer(Player *p){
	p->inputs = _Inputs;
	if((p->inputs->input & PPINPUT_A)&&!(p->inputs->prev->input & PPINPUT_A)){
		if(_Dialogue || _Player->talking){
			if(mainTextbox.donewriting){
				if(mainTextbox.msg){
					if(!mainTextbox.msg->next){
						_Player->talking = false;
						_Dialogue = false;
					}else{
						if(!mainTextbox.msg->hasPrompt){
							AdvanceText();
						}
					}
				}else{
					_Player->talking = false;
					_Dialogue = false;
				}
			}
		}
		else{
			if(World[_Player->tile.x+_Player->facing.x][_Player->tile.y+_Player->facing.y]!=NULL){
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


	if((!p->talking)&&(!p->movelock))
		PlayerMovement(p);
}

void PlayerMovement(Player *p){
	if(p->moving){
		p->animation = ANIM_CHAR_WALK;
		p->facing = p->tomove;
		MoveToTile(p,World[p->tile.x][p->tile.y],World[p->tile.x+p->tomove.x][p->tile.y+p->tomove.y]);
	}
	else{
		p->animation = ANIM_CHAR_IDLE;
		p->tomove.x = 0;
		p->tomove.y = 0;
		if(p->inputs->input>>4 > 0){	
			//if any directions are being held, reset our facing to 0,0 (nothing) so we can recalculate it
			//otherwise just use the previous facing coordinates
			p->facing.x = 0;
			p->facing.y = 0;
		}
		if((p->inputs->input & PPINPUT_LEFT)&&!(p->inputs->input & PPINPUT_RIGHT)){
			p->facing.x = -1;
			if(InputBuffered(p->inputs, PPINPUT_LEFT, INPUT_BUFFER)||InputBuffered(p->inputs, PPINPUT_UP, INPUT_BUFFER)||InputBuffered(p->inputs, PPINPUT_DOWN, INPUT_BUFFER))
				if(World[p->tile.x-1][p->tile.y]!=NULL)
					if(World[p->tile.x-1][p->tile.y]->free)
						if(abs(World[p->tile.x-1][p->tile.y]->height - World[p->tile.x][p->tile.y]->height)<=1) //if the tile we want to go to is within 1 height of our current tile 
							p->tomove.x = -1;
		}
		else if((p->inputs->input & PPINPUT_RIGHT)&&!(p->inputs->input & PPINPUT_LEFT)){
			p->facing.x = 1;
			if(InputBuffered(p->inputs, PPINPUT_RIGHT, INPUT_BUFFER)||InputBuffered(p->inputs, PPINPUT_UP, INPUT_BUFFER)||InputBuffered(p->inputs, PPINPUT_DOWN, INPUT_BUFFER))
				if(World[p->tile.x+1][p->tile.y]!=NULL)
					if(World[p->tile.x+1][p->tile.y]->free){
						if(abs(World[p->tile.x+1][p->tile.y]->height - World[p->tile.x][p->tile.y]->height)<=1)
							p->tomove.x = 1;
					}
		}
		if((p->inputs->input & PPINPUT_UP)&&!(p->inputs->input & PPINPUT_DOWN)){
			p->facing.y = -1;
			if(InputBuffered(p->inputs, PPINPUT_UP, INPUT_BUFFER)||InputBuffered(p->inputs, PPINPUT_LEFT, INPUT_BUFFER)||InputBuffered(p->inputs, PPINPUT_RIGHT, INPUT_BUFFER))
				if((World[p->tile.x][p->tile.y-1]!=NULL)&&(World[p->tile.x+p->tomove.x][p->tile.y-1]!=NULL))
					if((World[p->tile.x][p->tile.y-1]->free)&&(World[p->tile.x+p->tomove.x][p->tile.y-1]->free))
						if(abs(World[p->tile.x][p->tile.y-1]->height - World[p->tile.x][p->tile.y]->height)<=1)
							p->tomove.y = -1;
		}
		else if((p->inputs->input & 1<<4)&&!(p->inputs->input & 1<<5)){
			p->facing.y = 1;
			if(InputBuffered(p->inputs, PPINPUT_DOWN, INPUT_BUFFER)||InputBuffered(p->inputs, PPINPUT_LEFT, INPUT_BUFFER)||InputBuffered(p->inputs, PPINPUT_RIGHT, INPUT_BUFFER))
				if((World[p->tile.x][p->tile.y+1]!=NULL)&&(World[p->tile.x+p->tomove.x][p->tile.y+1]!=NULL))
					if((World[p->tile.x][p->tile.y+1]->free)&&(World[p->tile.x+p->tomove.x][p->tile.y+1]->free))
						if(abs(World[p->tile.x][p->tile.y+1]->height - World[p->tile.x][p->tile.y]->height)<=1)
							p->tomove.y = 1;
		}
		if((p->tomove.x!=0)||(p->tomove.y!=0)){
			p->animation = ANIM_CHAR_WALK;
			p->moving=true;
			MoveToTile(p,World[p->tile.x][p->tile.y],World[p->tile.x+p->tomove.x][p->tile.y+p->tomove.y]);
		}
	}
	
	UpdateDirection(p);
//	fprintf(stdout,"%i\n",p->direction);
	
	p->worldposition.x = p->tile.x*TILE_W+p->localposition.x;
	p->worldposition.y = p->tile.y*TILE_H+p->localposition.y;
}

bool InputBuffered (InputNode *input, int button, int buf){
	if(input->input & button){
		if(buf == 0)
			return 1;
		else{
			if(input->prev != NULL)
				return (InputBuffered(input->prev,button,buf-1));
			else
				return 0;
		}
	}else
		return 0;
}

void MoveToTile(Character *c, Tile *src, Tile *dest){
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


	if((std::abs(c->localposition.x)%TILE_W==0.5*TILE_W)&&(std::abs(c->localposition.y)%TILE_H==0.5*TILE_H)){
		UpdateTile(c);
		c->moving = false;
	}
}

void UpdateTile(Character *c){
	World[c->tile.x][c->tile.y]->free = true;
	if(c->localposition.x >= TILE_W){
		c->tile.x++;
		c->localposition.x -= TILE_W;
	}
	if(c->localposition.x < 0){
		c->tile.x--;
		c->localposition.x += TILE_W;
	}
	if(c->localposition.y >= TILE_H){
		c->tile.y++;
		c->localposition.y -= TILE_H;
	}
	if(c->localposition.y < 0){
		c->tile.y--;
		c->localposition.y += TILE_H;
	}
	World[c->tile.x][c->tile.y]->free = false;
	if(DEBUG)
		std::cout<<c->tile.x<<", "<<c->tile.y<<std::endl;
}

void DrawPlayer(Player *p){

	if(DEBUG_DRAW_RECTS){
		DrawTile(p->tile);
		DrawCursor(p->tile+p->facing);
	}
	if(p->animation < 0)
		p->animation = (CharAnim)0;
	if(p->animation+1 > p->numAnims)
		p->animation =  (CharAnim)(p->numAnims-1);

	DrawAnimation(p->animlist[p->animation][p->direction],p->worldposition-p->s_offset,&mainCamera);

}

void UpdateDirection(Character *c){
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