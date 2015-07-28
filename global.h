#ifndef _GLOBAL_
#define _GLOBAL_

#include "settings.h"

#define OUTPUT(x) #x<<"= "<<(x)<<std::endl	//type <<OUTPUT(x) to print any fucking variable you want this is incredible


typedef enum{
	MAIN_MENU,
	OPTIONS,
	OVERWORLD,
	INVENTORY,
	PARTY,
	COMBAT,		
	CUTSCENE,				
	DIALOGUE_OVERLAY,	//Maybe unnecessary?
	DIALOGUE_FULL,
	DATE			
}GameState;

//Any globally accessible functions get declared here
/*
void EnterCombat();
void ExitCombat();
bool InCombat();*/
GameState GetCurrentState();

//void FightBoss();

#endif