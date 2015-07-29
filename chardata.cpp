#include "chardata.h"
#include "pstring.h"

#define NUMCHARDATA 128
CharData _CharDataList[NUMCHARDATA];
static int numcds = 0;

CharData *LoadCharData(int charid){
	CharData *cd;
	int i;
	for(i = 0; i < NUMCHARDATA; i++){
		if(_CharDataList[i].id == charid)
			return  &_CharDataList[i];
	}


	cd = &_CharDataList[numcds];
	numcds++;
	cd->id = charid; //FIX: make it a number that isnt already used 
	cd->portrait = LoadAnimation(LoadSprite("sprites/speaker-gen.png",64,64,1,32,64),0,0,1);
	return cd;
}


CharData *CreateCharData(char *name){
	CharData *cd = &_CharDataList[numcds];
	numcds++;

	copy_string(cd->name,name);

	cd->id = numcds;

	cd->portrait = LoadAnimation(LoadSprite("sprites/speaker-gen.png",64,64,1,32,64),0,0,1);

	return cd;
}

void LevelUp(CharData *chr){
//	chr->level++;
	

}