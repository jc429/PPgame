#include "combat_ui.h"
#include "combat.h"

#include <vector>
#include <string>
using std::vector;
using std::string;


extern Camera combatCamera;
vector<CombatUI*> _CombatUIStack;

static bool _InAttack;

CombatUI *selectcursor;
CombatUI *targetcursor;
CombatUI *testmenu;

char CombatMessages[8][255];


void InitCUI(){
	selectcursor = LoadCombatUI(CUI_CURSOR);
	targetcursor = LoadCombatUI(CUI_CURSOR);
	testmenu = LoadCombatUI(CUI_MENU);

	copy_string(CombatMessages[0],"A WILD ");
	strcat(CombatMessages[0],"FUCKTRUCK");
	strcat(CombatMessages[0]," APPEARS");

}

CombatUI *LoadCombatUI(CUIType type, Vec2i *loc){
	CombatUI *c = new CombatUI;
	switch(type){
	case CUI_MENU:
		c->contents.menu = *LoadBattleMenu(loc);
		break;
	case CUI_CURSOR:
		c->contents.cursor = *LoadCombatCursor();
		break;
	}
	c->type = type;
	return c;
}

Menu *LoadBattleMenu(Vec2i *loc){
	_InMenu = true;
	Menu *m = LoadMenu(MENU_BATTLE,loc);
	SetMenuItemAction(m->items[0],InitiateAttack);
	SetMenuItemAction(m->items[1],OpenInventory);
	SetMenuItemAction(m->items[2],CancelCombatUI);
	SetMenuItemAction(m->items[3],Flee);
	return m;
}


void UpdateCombatUI(CombatUI *c){
	if(InputPressed(PPINPUT_B)){
		CancelCombatUI();
		return;
	}
	switch(c->type){
	case CUI_MENU:
		UpdateMenu(&c->contents.menu);
		break;
	case CUI_CURSOR:
		UpdateCursor(&c->contents.cursor);
		break;
	}
}

void DrawCombatUI(CombatUI *c){
	switch(c->type){
	case CUI_MENU:
		if(_CombatUIStack.back() ==  c){
			c->contents.menu.cursor.active = true;
			for(int i = 0; i < 4; i++)
				c->contents.menu.cursor.anim[i] = c->contents.menu.cursor.anim_active[i];
		}else{
			c->contents.menu.cursor.active = false;
			for(int i = 0; i < 4; i++)
				c->contents.menu.cursor.anim[i] = c->contents.menu.cursor.anim_inactive[i];
		}
		DrawMenu(&c->contents.menu);
		break;
	case CUI_CURSOR:
		DrawCursor(&c->contents.cursor);
		break;
	}
}

void OpenCombatUI(CombatUI *c){
	switch(c->type){
	case CUI_MENU:
		c->contents.menu.active = true;
		c->contents.menu.cursor.location = 0;
		break;
	case CUI_CURSOR:
		break;
	}
	_CombatUIStack.push_back(c);
}

void CancelCombatUI(){
	_CombatUIStack.pop_back();
}

void UpdateCursor(CombatCursor *cc){
	if(InputPressed(PPINPUT_DIR_ANY))
		MoveCursor(cc);
	SetVec2i(cc->position,(int)cc->target->position_base.x,(int)cc->target->position_base.y - 50);
	if(InputPressed(PPINPUT_A)){
		if(!_InAttack){
			OpenCombatUI(testmenu);
			_InAttack = true;
		}else{
			QueuePlayerAttack();
			_InAttack = false;
			_CombatUIStack.clear();
		}
	}
}

void DrawCursor(CombatCursor *cc){
	DrawAnimation(cc->anim,cc->position,&combatCamera);
}

CombatCursor *LoadCombatCursor(CombatEnt *targ){
	CombatCursor *cc = new CombatCursor();
	cc->anim = LoadAnimation(LoadSprite(SPATH_COMBAT_CURSOR,32,32,1,16,32),0,0,1,1);
//	SetVec2i(cc->s_offset,16,32);
	cc->target = targ;
	return cc;
}

void MoveCursor(CombatCursor *cc){
	Vec2i dir;
	SetVec2i(dir,0,0);
	if(InputPressed(PPINPUT_UP))
		dir.y += 1;
	if(InputPressed(PPINPUT_DOWN))
		dir.y -= 1;
	if(InputPressed(PPINPUT_LEFT))
		dir.x -= 1;
	if(InputPressed(PPINPUT_RIGHT))
		dir.x += 1;
	cc->target = FindCombatEnt(cc->target,dir);
}

