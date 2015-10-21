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
	movex = false;
	movey = false;

	//Speeds and stuff
	movespeed = 1.2f;		//needs to be 1 right now..
			
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
	AddToWorld(xpos, ypos);
	UpdateTile();
	if(DEBUG){
//		printf("Starting on %i %i \n",tile.x,tile.y);
	}
	UpdateWorldPosition();
//	worldposition.x = tile.x*TILE_W+localposition.x;
//	worldposition.y = tile.y*TILE_H+localposition.y;

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

	DrawAnimation(animlist[animation][direction],WorldPosition(),&mainCamera);
	drawn = true;
}

void OverworldCharacter::SetPlayer(bool p){
	isPlayer = p;
	movelock = !p;
	Logger::Log(chardata->name);
	_Player = this;

}

void OverworldCharacter::Movement(){
	switch(_GlobalMovementType){
	case MovementType_Grid:
		if(moving){
			if(animation != ANIM_CHAR_WALK)
				animation = ANIM_CHAR_WALK;
			facing = tomove;
		
			MoveToTile(World[tile_src.x][tile_src.y],World[tile_dest.x][tile_dest.y]);

		}else{
			UpdateTile();
		}
		UpdateDirection();
		UpdateWorldPosition();
		break;
	case MovementType_Free:
		FreeMove();
		UpdateTile();
		UpdateWorldPosition();
		break;
	case MovementType_Platformer:
		PlatformerMove();
		break;
	}


}



void OverworldCharacter::StepOutOfTile(Tile *tile){
	if(tile == NULL) return;
	tile->free = true;
	tile->contents = NULL;
	tile->character = NULL;
}

void OverworldCharacter::StepIntoTile(Tile *tile){
	if(tile == NULL) return;

	tile->free = false;
	tile->contents = this;	
	tile->character = this;
}

void OverworldCharacter::MoveToTile(Tile *src, Tile *dest){
	StepIntoTile(dest);
	//fprintf(stdout,"moving from %i, %i to %i, %i \n",src->position.x/TILE_W,src->position.y/TILE_H,dest->position.x/TILE_W,dest->position.y/TILE_H);
	Vec2i movedir;
	movedir.x = 0;
	movedir.y = 0;
	if(src->position.x < dest->position.x)
		movedir.x = 1;
	else if(src->position.x > dest->position.x)
		movedir.x = -1;
	if(src->position.y < dest->position.y)
		movedir.y = 1;
	else if(src->position.y > dest->position.y)
		movedir.y = -1;

	Vec2f movement;
	movement.x = movedir.x * movespeed*(FRAMEDELAY/TILE_W);
	movement.y = movedir.y * movespeed*(FRAMEDELAY/TILE_H);

	if(movement.x > 0){
		//if moving at our current increment would overshoot the destination, fix it
		if((WorldPosition().x + movement.x) > (dest->position.x + (0.5f*TILE_W))){	
			movement.x = (dest->position.x + (0.5f*TILE_W)) - WorldPosition().x;
		}
	}
	else if(movement.x < 0){
		if((WorldPosition().x + movement.x) < (dest->position.x + (0.5f*TILE_W))){
			movement.x = (dest->position.x + (0.5f*TILE_W)) - WorldPosition().x;
		}
	}

	if(movement.y > 0){
		//if moving at our current increment would overshoot the destination, fix it
		if((WorldPosition().y + movement.y) > (dest->position.y + (0.5f*TILE_H))){	
			movement.y = (dest->position.y + (0.5f*TILE_H)) - WorldPosition().y;
		}
	}
	else if(movement.y < 0){
		if((WorldPosition().y + movement.y) < (dest->position.y + (0.5f*TILE_H))){	
			movement.y = (dest->position.y + (0.5f*TILE_H)) - WorldPosition().y;
		}
	}
	//printf("%f,%f",movement.x,movement.y);
	
	localposition.x += movement.x;
	localposition.y += movement.y;
	

	if((WorldPosition().x == dest->position.x + 0.5f*TILE_W)
		&&(WorldPosition().y == dest->position.y + 0.5f*TILE_H)){
		moving = false;
		movex = false;
		movey = false;
	}
}

void OverworldCharacter::FreeMove(){
	Vec2f movement;
	movement.x = (float)tomove.x * movespeed*(FRAMEDELAY/TILE_W);
	movement.y = (float)tomove.y * movespeed*(FRAMEDELAY/TILE_H);

	localposition.x += movement.x;
	localposition.y += movement.y;

	UpdateDirection();


}

void OverworldCharacter::PlatformerMove(){	//movement function	with physics
	Vec2f movement;
	movement.x = tomove.x*movespeed;
	
	
	//do some gravity shit for movement.y

	localposition.x += movement.x;
	localposition.y += movement.y;	
}

void OverworldCharacter::UpdateTile(){
	StepOutOfTile(World[this->tile.x][this->tile.y]);
	if(this->localposition.x > TILE_W){
		this->tile.x++;
		this->localposition.x -= TILE_W;
	}
	if(this->localposition.x < 0){
		this->tile.x--;
		this->localposition.x += TILE_W;
	}
	if(this->localposition.y > TILE_H){
		this->tile.y++;
		this->localposition.y -= TILE_H;
	}
	if(this->localposition.y < 0){
		this->tile.y--;
		this->localposition.y += TILE_H;
	}
	StepIntoTile(World[this->tile.x][this->tile.y]);
}

void OverworldCharacter::UpdateDirection(){
	switch(this->facing.x){
	case 0:
		switch(this->facing.y){
		case 0:
			this->direction = ANIM_DIR_S;
			break;
		case 1:
			this->direction = ANIM_DIR_S;
			break;
		case -1:
			this->direction = ANIM_DIR_N;
			break;
		}
		break;
	case 1:
		switch(this->facing.y){
		case 0:
			this->direction = ANIM_DIR_E;
			break;
		case 1:
			this->direction = ANIM_DIR_SE;
			break;
		case -1:
			this->direction = ANIM_DIR_NE;
			break;
		}
		break;
	case -1:
		switch(this->facing.y){
		case 0:
			this->direction = ANIM_DIR_W;
			break;
		case 1:
			this->direction = ANIM_DIR_SW;
			break;
		case -1:
			this->direction = ANIM_DIR_NW;
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
