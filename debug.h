#ifndef _DEBUG_
#define _DEBUG_

//debug mode toggle
#define DEBUG 1				//PROTIP: wrap any code you want to execute in if(DEBUG){<code>} and set this to 1 or 0 as necessary  

#define DEBUG_DRAW_RECTS 1

#define FORCE_DEBUG_LEVEL 0

#define DEBUG_MUTE 1

#define DEBUG_START_MODE GS_OVERWORLD

#define DEBUG_INFINITE_MONEY 0

void DebugLoads();
void DebugUpdates();

typedef enum ErrorLevel{
	Error_Harmless = 1,
};

#include <string>
namespace Logger{
	void Log(char *msg);
	void Log(std::string msg);
	void Log(int n);
	void Log(float f);
	void Log(float x, float y);

	void Error(ErrorLevel level, char *msg);
}

//debug function declarations



#endif