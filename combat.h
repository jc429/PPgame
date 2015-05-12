#ifndef _COMBAT_
#define _COMBAT_

//handles general combat stuff, like updating and drawing everything

#include "pmath.h"
#include "graphics.h"
#include "combat_ent.h"
#include "combat_ui.h"

enum ActionType{
	ACT_ATTACK,
	ACT_BLOCK,
	ACT_ITEM,
	ACT_DEBUG,
};

typedef struct CombatAction_T{
	CombatEnt *ent;
	CombatEnt *target;
	ActionType type;
	char text[255];		//the line of dialogue associated with an action
	void (*Action)(CombatEnt* owner, CombatEnt *target);
}CombatAction;

typedef struct CombatEffect_T{
	Animation *anim;
	Vec2i location;
}CombatEffect;


typedef enum BattlePhase_T{
	BATTLE_START,
	BATTLE_MID,
	BATTLE_END
}BattlePhase;

typedef enum TurnPhase_T{
	ACTION_SELECT,
	ACTING,
	RESOLUTION,
}TurnPhase;


void QueueAction(CombatAction* action);
void ExecuteQueue();
void ExecuteAction(CombatAction* action);
//Possible Actions
void TestAction(CombatEnt* owner, CombatEnt *target);
void UseItem(CombatEnt* owner, CombatEnt *target);
void Attack(CombatEnt* owner, CombatEnt *target);


void InitCombat();

void SetupCombat();

void EnterCombat();
void ExitCombat();
void UpdateCombat();
void UpdateTurn();
void AdvanceBattlePhase();
void AdvanceTurnPhase();


//loading/unloading
void LoadCombatBG();


//drawing
void DrawCombatBG();
void DrawHPBar(CombatEnt *ent);
void DrawHPBarFull(CombatEnt *ent);
void DrawBattleTimer();
void DrawEffectStack();
void DrawActionStack();
void DrawCombatUI();


//combat menu functions
void InitiateAttack();
void QueuePlayerAttack();
void QueueAttack(CombatEnt *owner, CombatEnt *target);
void Flee();


#endif