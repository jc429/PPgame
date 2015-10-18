#ifndef _GLOBAL_
#define _GLOBAL_

#include "settings.h"

#define OUTPUT(x) #x<<"= "<<(x)<<std::endl	//type <<OUTPUT(x) to print any fucking variable you want this is incredible


typedef enum{
	GS_MAIN_MENU,
	GS_OPTIONS,
	GS_OVERWORLD,
	GS_INVENTORY,
	GS_SAVE_LOAD,
	GS_STATUS,

	GS_CUTSCENE,				
	GS_DIALOGUE_OVERLAY,	//Maybe unnecessary?
	GS_DIALOGUE_FULL			
}GameState;

namespace GlobalSettings{
	
}


//Any globally accessible functions get declared here
/*
void EnterCombat();
void ExitCombat();
bool InCombat();*/

//void FightBoss();

#endif