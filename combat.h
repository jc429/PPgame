#ifndef _COMBAT_
#define _COMBAT_

#include "pmath.h"
#include "graphics.h"
#include "ui.h"



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
	S_STYLED	= 256,

}Status;

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

	bool ally;				//good or bad?
	
	Stats stats_base;		//base stats, i.e. the ones that go up with each level and stuff
	Stats stats_mod;		//temporary stat modifiers, reset at battle end or w/e. added or subtracted to base stats
	Stats growths;			//growth rates, stored as int/100 probably. used for level ups


	Vec2i position;
	Vec2i s_offset;					//sprite offset
	Animation *animlist[MAX_ANIMS]; //all animations this entity can have
	int animation;					//current animation
	int numAnims;

	Technique *TechniqueList[MAX_TECHS];
	Status status;
}CombatEnt;

void SetStats(CombatEnt *ent);

void InitCombat();
void EnterCombat();
void ExitCombat();
void UpdateCombat();
void UpdateTurn();
void AdvanceBattlePhase();
void AdvanceTurnPhase();

void LoadEnemies();
void LoadAllies();

void LoadCombatBG();
void DrawCombatBG();
void DrawEnemies();
void DrawAllies();
void DrawHPBar(CombatEnt *ent);
void DrawCombatUI();

#endif