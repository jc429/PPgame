#ifndef _CHARDATA_
#define _CHARDATA_

#include "global.h"
#include <vector>
using std::vector;

//these numbers are to sort chardata, so everything doesn't have a random ID
#define ID_PARTY	400
#define ID_NPC		500
#define ID_ENEMY	600
#define ID_MISC		900


typedef struct Stats_T{
	int max_health;
	int max_stamina;

	int strength;
	int phys_def;
	int magic;
	int mag_def;

	int agility;
	int dexterity;
	int luck;
	int intelligence;
	int willpower;
	int charisma;

	int quality;
	int humor;
	int style;
	int grace;
	int spookiness;
	int cheesiness;
	int legs;
}Stats;

typedef enum Status_T{
	S_FINE		= 0,
	S_DEAD		= 1,
	S_BURN		= 2,
	S_STUN		= 4,
	S_SLEEP		= 8,
	S_POISON	= 16,
	S_FREEZE	= 32,
	S_DESPAIR	= 64,
	S_BLIND		= 128,
	S_STYLEDON	= 256,

}Status;

typedef struct Technique_T{
	int techID;
	int icon;
	char* displayName;
	char* description;
}Technique;

typedef struct CharData_T{
	int id;
	char name[32];
	int level;
	int exp;

	int health;

	Stats stats_base;		//base stats, i.e. the ones that go up with each level and stuff
	Stats stats_mod;		//temporary stat modifiers, reset at battle end or w/e. added or subtracted to base stats
	Stats growths;			//growth rates, stored as int/100 probably. used for level ups
}CharData;




CharData *LoadCharData(int charid);
CharData *CreateCharData(char *name);

void LevelUp(CharData *chr);


#endif