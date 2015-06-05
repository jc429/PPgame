#ifndef _COMBAT_ENT_
#define _COMBAT_ENT_

#include "global.h"
#include "sprites.h"
#include "pmath.h"

#include "entity.h"
#include "chardata.h"

#include <queue>
using std::queue;

//for combat entities and anything that affects them



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



typedef struct CombatEnt_T:public Entity{
	CharData *chardata;
	
	int exhaustion;			//cooldown time after performing an action - affected by stamina?
	int stamina;

	bool friendly;				//good or bad?

	Vec2d position_base;		//where the combat ent generally resides
	Vec2d position;			//where the ent is currently
	Vec2d position_target;	//used for motion events

//	Vec2i s_offset;					//sprite offset
	Animation *animlist[MAX_ANIMS]; //all animations this CombatEnt can have
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
CombatEnt *FindEnemy(int id);
CombatEnt *LoadCombatEnt(int charid);
void AddCombatEntAnim(CombatEnt *cent, Animation *anim, int position = 0);
//CombatEnt *LoadCombatEntCFG(char *path);
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

vector<CombatEnt*> LoadEnemyDataCFG(char *path);

void RoatateAllies();
void RoatateEnemies();



#endif