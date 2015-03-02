#ifndef _COMBAT_
#define _COMBAT_

#include "pmath.h"
#include "graphics.h"

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
	int charisma;

	int quality;
	int humor;
	int style;
	int grace;
	int spookiness;
	int cheesiness;
	int legs;
}Stats;

typedef struct Technique_T{
	int techID;
	char* displayName;
	char* description;
}Technique;

typedef struct CombatEnt_T{
	int level;
	int exp;

	int health;
	int stamina;
	
	Stats stats_base;		//base stats, i.e. the ones that go up with each level and stuff
	Stats stats_mod;		//temporary stat modifiers, reset at battle end or w/e. added or subtracted to base stats
	Stats growths;			//growth rates, stored as int/100 probably. used for level ups

}CombatEnt;

void SetStats(CombatEnt *ent);

void LoadCombatBG();
void DrawCombatBG();


#endif