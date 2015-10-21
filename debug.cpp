#include "debug.h"
#include "main.h"


/////*** Test Variables ***/////
Sound *testsound;
Music *testbgmusic;
Music *testbgmusic2;
/////**********************/////

//for anything that just needs to be loaded once
void DebugLoads(){
	testsound = LoadSound("sounds/selection_blip.wav",33);
	testbgmusic = LoadMusic("sounds/yoshi_story.mp3",40);
	testbgmusic2 = LoadMusic("sounds/gerudo_valley.mp3",100);	//not sure if volume affects anything...
	
	PlayMusic(testbgmusic);

}


//for anything that just needs to be tested in a generic update
void DebugUpdates(){
	if(InputPressed(PPINPUT_DEBUG1))
		PlayMusic(testbgmusic);
	if(InputPressed(PPINPUT_DEBUG2))
		PlayMusic(testbgmusic2);

	
	if(InputPressed(PPINPUT_A))
		PlaySound(testsound);
}

void Logger::Log(char *msg){
	puts(msg);
}
void Logger::Log(std::string msg){
	puts(msg.c_str());
}

void Logger::Log(int n){
	Logger::Log(std::to_string(static_cast<long long>(n)));
}

void Logger::Log(float f){
	Logger::Log(std::to_string(static_cast<long long>(f)));
}

void Logger::Log(float x, float y){
	std::string s = std::to_string(static_cast<long long>(x));
	s.append(", ");
	s.append(std::to_string(static_cast<long long>(y)));
	Logger::Log(s);
}
