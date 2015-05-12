#include "chardata.h"
#include "pstring.h"

#define NUMCHARDATA 128
CharData _CharDataList[NUMCHARDATA];
static int numcds = 0;

CharData *LoadCharData(char *name){
	CharData *cd = &_CharDataList[numcds];
	numcds++;

	copy_string(cd->name,name);

	cd->level = 1;

	return cd;
}

void LevelUp(CharData *chr){
	chr->level++;
	

}