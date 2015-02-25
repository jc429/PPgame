#include "player.h"
#include "camera.h"
#include <iostream>

extern Tile *World[WORLD_W][WORLD_H];
extern Camera mainCamera;


Player* CreatePlayer(Player *p){

	p = (Player*) malloc(sizeof(Player));

	//Inputs
	p->inputs = new InputNode;
	p->inputs->input = 00000000;
	p->inputs->prev = NULL;

	//Facing
	p->facing.x = 0;		//These should never both be zero
	p->facing.y = 1;		//Face south to begin i guess

	//Movement
	p->tomove.x = 0;	
	p->tomove.y = 0;
	p->moving = false;
	//Speeds and stuff
	p->movespeed = 2;
	
	//Position
	p->tile.x = 1;
	p->tile.y = 3;
	p->localposition.x = TILE_W*0.5;
	p->localposition.y = TILE_H*0.5;

	//Graphics
	p->frame = 0;
	p->sprite = LoadSprite("sprites/mad.png",32,32,4);
	p->s_offset.x = p->sprite->w*0.5;
	p->s_offset.y = p->sprite->h*0.5;

	//Dialogue and misc
	p->talking = false;


	//get us started on a tile
	UpdateTile(p);
	if(DEBUG)
		fprintf(stdout,"%i %i \n",p->localposition.x,p->localposition.y);
	return p;
}

void UpdatePlayer(Player *p){
	
	if(!p->talking)
		PlayerMovement(p);
}

void PlayerMovement(Player *p){
	
	if(p->moving){
		
		p->facing = p->tomove;
		MoveToTile(p,World[p->tile.x][p->tile.y],World[p->tile.x+p->tomove.x][p->tile.y+p->tomove.y]);
	}
	else{
		p->tomove.x = 0;
		p->tomove.y = 0;
		if(p->inputs->input>>4 > 0){
			p->facing.x = 0;
			p->facing.y = 0;
		}
		if((p->inputs->input & 1<<7)&&!(p->inputs->input & 1<<6)){
			p->facing.x = -1;
			if(InputBuffered(p->inputs, 7, INPUT_BUFFER)||InputBuffered(p->inputs, 5, INPUT_BUFFER)||InputBuffered(p->inputs, 4, INPUT_BUFFER))
				if(World[p->tile.x-1][p->tile.y]!=NULL)
					if(World[p->tile.x-1][p->tile.y]->free)
						p->tomove.x = -1;
		}
		else if((p->inputs->input & 1<<6)&&!(p->inputs->input & 1<<7)){
			p->facing.x = 1;
			if(InputBuffered(p->inputs, 6, INPUT_BUFFER)||InputBuffered(p->inputs, 5, INPUT_BUFFER)||InputBuffered(p->inputs, 4, INPUT_BUFFER))
				if(World[p->tile.x+1][p->tile.y]!=NULL)
					if(World[p->tile.x+1][p->tile.y]->free)
						p->tomove.x = 1;
		}
		if((p->inputs->input & 1<<5)&&!(p->inputs->input & 1<<4)){
			p->facing.y = -1;
			if(InputBuffered(p->inputs, 5, INPUT_BUFFER)||InputBuffered(p->inputs, 7, INPUT_BUFFER)||InputBuffered(p->inputs, 6, INPUT_BUFFER))
				if((World[p->tile.x][p->tile.y-1]!=NULL)&&(World[p->tile.x+p->tomove.x][p->tile.y-1]!=NULL))
					if((World[p->tile.x][p->tile.y-1]->free)&&(World[p->tile.x+p->tomove.x][p->tile.y-1]->free))
						p->tomove.y = -1;
		}
		else if((p->inputs->input & 1<<4)&&!(p->inputs->input & 1<<5)){
			p->facing.y = 1;
			if(InputBuffered(p->inputs, 4, INPUT_BUFFER)||InputBuffered(p->inputs, 7, INPUT_BUFFER)||InputBuffered(p->inputs, 6, INPUT_BUFFER))
				if((World[p->tile.x][p->tile.y+1]!=NULL)&&(World[p->tile.x+p->tomove.x][p->tile.y+1]!=NULL))
					if((World[p->tile.x][p->tile.y+1]->free)&&(World[p->tile.x+p->tomove.x][p->tile.y+1]->free))
						p->tomove.y = 1;
		}
		if((p->tomove.x!=0)||(p->tomove.y!=0)){
			p->moving=true;
			MoveToTile(p,World[p->tile.x][p->tile.y],World[p->tile.x+p->tomove.x][p->tile.y+p->tomove.y]);
		}
	}

	
	p->worldposition.x = p->tile.x*TILE_W+p->localposition.x;
	p->worldposition.y = p->tile.y*TILE_H+p->localposition.y;
}

bool InputBuffered (InputNode *input, int button, int buf){
	if(input->input & 1<<button){
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

void MoveToTile(Player *p, Tile *src, Tile *dest){
	Vec2i movement;
	movement.x = 0;
	movement.y = 0;
	if(src->position.x < dest->position.x)
		movement.x = p->movespeed;
	else if(src->position.x > dest->position.x)
		movement.x = -p->movespeed;
	if(src->position.y < dest->position.y)
		movement.y = p->movespeed;
	else if(src->position.y > dest->position.y)
		movement.y = -p->movespeed;

	p->localposition = p->localposition + movement;


	if((std::abs(p->localposition.x)%TILE_W==0.5*TILE_W)&&(std::abs(p->localposition.y)%TILE_H==0.5*TILE_H)){
		UpdateTile(p);
		p->moving = false;
	}
}

void UpdateTile(Player *p){
	World[p->tile.x][p->tile.y]->free = true;
	if(p->localposition.x >= TILE_W){
		p->tile.x++;
		p->localposition.x -= TILE_W;
	}
	if(p->localposition.x < 0){
		p->tile.x--;
		p->localposition.x += TILE_W;
	}
	if(p->localposition.y >= TILE_H){
		p->tile.y++;
		p->localposition.y -= TILE_H;
	}
	if(p->localposition.y < 0){
		p->tile.y--;
		p->localposition.y += TILE_H;
	}
	World[p->tile.x][p->tile.y]->free = false;
	if(DEBUG)
		std::cout<<p->tile.x<<", "<<p->tile.y<<std::endl;
}

void DrawPlayer(Player *p){

	if(DEBUG){
		DrawTile(p->tile);
		DrawCursor(p->tile+p->facing);
	}
	p->frame = DrawSprite(p->sprite,p->frame,p->worldposition-p->s_offset,&mainCamera);
}