#ifndef _GLOBAL_
#define _GLOBAL_

#include "settings.h"

#define OUTPUT(x) #x<<"= "<<(x)<<std::endl	//type <<OUTPUT(x) to print any fucking variable you want this is incredible


//Any globally accessible functions get declared here

void LaunchCombat();
bool InCombat();

#endif