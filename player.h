#ifndef _PLAYER_
#define _PLAYER_

#include "character.h"




class NPC: public OverworldCharacter{
public:
	bool rotates; //does this NPC turn when they speak?
	bool moves;
	Message *msg;	//any dialogue they have gets stored here 
	int actiontimer;	//counts down to 0 then does an something (maybe) 


	NPC (int xpos, int ypos, char *entName = NULL);
//	~NPC();

	void Update();
	void Move(Vec2i want_to_move);
	void Talk(TextboxEX *t);
};


/*
class Player: public NPC{
public:
	InputNode *inputs;

	Player(int xpos, int ypos, char *entName);
	~Player();

};*/

NPC **LoadEntitiesCFG(char *path);



void UpdatePlayer(OverworldCharacter *p);


void PlayerMovement(OverworldCharacter *p);



void GiveNPCMessage(NPC *npc, Message *msg);

#endif