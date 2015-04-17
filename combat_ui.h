#ifndef _COMBAT_UI_
#define _COMBAT_UI_


#include "global.h"
#include "sprites.h"
#include "ui.h"
#include "combat_ent.h"

typedef enum CUIType{
	CUI_CURSOR,
	CUI_MENU
};

typedef struct CombatCursor_T{
	Vec2i position;
	Vec2i s_offset;
	CombatEnt *target;
	Animation *anim;
}CombatCursor;

union CombatItem{
	Menu menu;
	CombatCursor cursor;
};

typedef struct CombatUI_T{
	CUIType type;
	CombatItem contents;
}CombatUI;

void InitCUI();

CombatUI *LoadCombatUI(CUIType type, Vec2i *loc = NULL);
Menu *LoadBattleMenu(Vec2i *loc = NULL);
CombatCursor *LoadCombatCursor(CombatEnt *targ = NULL);

void UpdateCombatUI(CombatUI * c);
void DrawCombatUI(CombatUI * c);

void OpenCombatUI(CombatUI *c);


void UpdateCursor(CombatCursor *cc);
void MoveCursor(CombatCursor *cc);

void DrawCursor(CombatCursor *cc);

#endif