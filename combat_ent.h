#ifndef _COMBAT_ENT_
#define _COMBAT_ENT_

#include "global.h"
#include "sprites.h"
#include "pmath.h"
#include <queue>
using std::queue;

//for combat entities and anything that affects them

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


typedef enum EventType_T{
	EVENT_DEBUG,
	EVENT_MOTION,
	EVENT_DAMAGE,
}EventType;

typedef struct EntMotion_T{  //A scripted motion, for attacks and stuff
	Vec2d src;
	Vec2d dest;
	int duration;		//duration, in frames, of the movement
}EntMotion;

typedef struct EntDamage_T{  //A scripted motion, for attacks and stuff
	int damage;
	struct CombatEnt_T *target;
}EntDamage;

union Event{
	EntMotion motion;
	EntDamage damage;
};

typedef struct ActionEvent_T{
	EventType type;
	Event e;
}ActionEvent;



typedef struct CombatEnt_T{
	char *name;

	int level;
	int exp;

	int health;
	int exhaustion;			//cooldown time after performing an action - affected by stamina?
	int stamina;

	bool friendly;				//good or bad?
	
	Stats stats_base;		//base stats, i.e. the ones that go up with each level and stuff
	Stats stats_mod;		//temporary stat modifiers, reset at battle end or w/e. added or subtracted to base stats
	Stats growths;			//growth rates, stored as int/100 probably. used for level ups

	Vec2d position_base;		//where the combat ent generally resides
	Vec2d position;			//where the ent is currently
	Vec2d position_target;	//used for motion events

	Vec2i s_offset;					//sprite offset
	Animation *animlist[MAX_ANIMS]; //all animations this entity can have
	int animation;					//current animation
	int numAnims;

	Technique *TechniqueList[MAX_TECHS];
	Status status;

	queue<ActionEvent> events;
//	queue<EntMotion> motions;

}CombatEnt;

void InitCombatSlots();

void LoadAllies();
void LoadEnemies();
CombatEnt *LoadCombatEnt();
CombatEnt *LoadCombatEntCFG(char *path);
void DrawAllies();
void DrawEnemies();
void EnemyAI();
void CombatEntThink(CombatEnt *ent);

//stat stuff
void SetStats(CombatEnt *ent);

//Event stuff
void PerformCurrentEvent(CombatEnt *ent);
void AddMotion(CombatEnt *ent, Vec2d dest, int duration);
void AddMotionRelative(CombatEnt *ent, Vec2d dest, int duration);
void AddDamage(CombatEnt *ent, CombatEnt *target, int damage);

void PerformMotion(CombatEnt *ent, EntMotion m);
void PerformDamage(CombatEnt *ent, EntDamage d);


CombatEnt *FindCombatEnt(CombatEnt *current, Vec2i dir); //direction to look for combat ent

#endif