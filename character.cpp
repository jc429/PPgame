#include "player.h"
#include "camera.h"

extern Camera mainCamera;
extern Tile *World[WORLD_W][WORLD_H];

OverworldCharacter *_Player;
OverworldCharacter *CharList[MAX_CHARACTERS];

OverworldCharacter::OverworldCharacter(int xpos, int ypos, char *entName){
	static int id;
	
	if(entName == NULL)
		entName = "Some Guy";
	chardata = LoadCharData(ID_NPC+id);
	++id;
	copy_string(chardata->name,entName);

	
	//Facing
	facing.x = 0;		//These should never both be zero
	facing.y = 1;		//Face south to begin i guess
	direction = ANIM_DIR_S;
	

	//Movement
	tomove.x = 0;	
	tomove.y = 0;
	moving = false;

	//Speeds and stuff
	movespeed = 1;
	
	//Position
	tile.x = xpos;
	tile.y = ypos;
	localposition.x = TILE_W>>1;
	localposition.y = TILE_H>>1;

	//Dialogue and misc
	talking = false;
	passable = false;
	type = Ent_Character;
	SetPlayer(false);



	for(int i = 0; i < MAX_CHARACTERS; i++){
		if(CharList[i] == NULL){
			CharList[i] = this;
			break;
		}
	}


	//get us started on a tile
	AddToWorld(this, xpos, ypos);
	UpdateTile(this);
	if(DEBUG){
//		printf("Starting on %i %i \n",tile.x,tile.y);
	}
	
	worldposition.x = tile.x*TILE_W+localposition.x;
	worldposition.y = tile.y*TILE_H+localposition.y;

}

void OverworldCharacter::Update(){
	drawn = false;
	Movement();
	if(IsPlayer()){
		UpdatePlayer(this);
	}
}

void OverworldCharacter::Draw(){
	if(drawn) return;

	DrawAnimation(animlist[animation][direction],worldposition,&mainCamera);
	drawn = true;
}

void OverworldCharacter::SetPlayer(bool p){
	isPlayer = p;
	movelock = !p;
	printf("%s \n", chardata->name);
	_Player = this;

}

void OverworldCharacter::Movement(){

	if(moving){
		if(animation != ANIM_CHAR_WALK)
			animation = ANIM_CHAR_WALK;
		facing = tomove;
		
		MoveToTile(this,World[tile_src.x][tile_src.y],World[tile_dest.x][tile_dest.y]);

	}else{
		UpdateTile(this);
	}

/*	else{
		UpdateTile(this);

		animation = ANIM_CHAR_IDLE;
		tomove.x = 0;
		tomove.y = 0;
		
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
	}*/
	
	UpdateDirection(this);
//	fprintf(stdout,"%i\n",p->direction);
	
	worldposition.x = tile.x*TILE_W + localposition.x;
	worldposition.y = tile.y*TILE_H + localposition.y - (World[tile.x][tile.y]->structure->height<<2);
}



void StepOutOfTile(OverworldCharacter *c,Tile *tile){
	if(tile == NULL) return;
	tile->free = true;
	tile->contents = NULL;
	tile->character = NULL;
}

void StepIntoTile(OverworldCharacter *c,Tile *tile){
	if(tile == NULL) return;

	tile->free = false;
	tile->contents = c;	
	tile->character = c;
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

void TransferPlayer(OverworldCharacter *src, OverworldCharacter *dest){
	if(src == NULL || dest == NULL) return;

	src->SetPlayer(false);
	dest->SetPlayer(true);

	mainCamera.target = dest;
}
