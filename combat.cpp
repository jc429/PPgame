#include "combat.h"
#include "gamestate.h"
#include "audio.h"
#include <queue>
#include <vector>
#include <string>
using std::queue;
using std::vector;
using std::string;

extern InputNode *_Inputs;
extern Camera combatCamera;
Animation *combatBG;
extern Textbox combatTextbox;

extern Menu *_CurrentMenu;
//extern vector<struct Menu_T*> _CombatMenuStack;
extern vector<CombatUI*> _CombatUIStack;
Music *bgm;

extern CombatEnt *CombatParty[MAX_PARTY_COMBAT];
extern CombatEnt *Enemies[MAX_ENEMIES];

static queue<CombatAction*> _ActionStack;
static int _MaxActions;
int ActionTimer;
#define ROUND_TIME 120

static vector<CombatEffect*> _EffectStack;


static BattlePhase _BattlePhase;
static TurnPhase _TurnPhase;

extern CombatUI *selectcursor;
extern CombatUI *targetcursor;
extern CombatUI *testmenu;

extern char CombatMessages[8][255];


static Uint32 frontcolor = pCol_Yellow;
static Uint32 backcolor = pCol_DarkRed;
void SwapColors();

void Combat::Update(){
	UpdateCombat();
	
}

void Combat::Draw(){
	DrawCombatBG();
	DrawEnemies();
	DrawAllies();
	DrawCombatUI();
}


void SwapColors(){
	Uint32 temp = frontcolor;
	frontcolor = backcolor;
	backcolor = temp;
}



void InitCombat(){
	Sprite *ctspr = LoadSprite(SPATH_COMBAT_TEXTBOX,320,40,1);

//	InitCombatMessages();

	InitMainTextbox(&combatTextbox,2,40,ctspr);

	
	InitCUI();


	SetText(CombatMessages[0],&combatTextbox,0);

	InitCombatSlots();
	
	
	_MaxActions = 3;

	bgm = LoadMusic(APATH_BGM_COMBAT,50);

}

void SetupCombat(){
	_BattlePhase = BATTLE_START;
	//play combat music
	PlayMusic(bgm);
	LoadEnemies();
	LoadAllies();
	selectcursor->contents.cursor.target = CombatParty[0];
	targetcursor->contents.cursor.target = Enemies[0];
	copy_string(CombatMessages[0], "A %ENAME% attacks!");
//	copy_string(CombatMessages[1],"HAHAHAHAHA %FUCK% YOU");
	/*copy_string(CombatMessages[1],"What will ");
	strcat(CombatMessages[1],Party[0]->name);
	strcat(CombatMessages[1]," do?")*/
	SetText(CombatMessages[0],&combatTextbox,1);
	ActionTimer = ROUND_TIME;
}



void UpdateCombat(){
	switch(_BattlePhase){
	case BATTLE_START:
		_BattlePhase = BATTLE_MID;
		break;
	case BATTLE_MID:
		EnemyAI();
		if(!_CombatUIStack.empty()){
			UpdateCombatUI(_CombatUIStack.back());
		}
		UpdateTurn();
		break;
	}
	
	
}

void UpdateTurn(){
	if(ActionTimer>0)
		ActionTimer--;
	if(ActionTimer == 0){
		SwapColors();
		if(!_ActionStack.empty()){
			ExecuteQueue();
		}
		ActionTimer = ROUND_TIME; //placeholder
	}
	
	for(int i = 0; i < MAX_PARTY_COMBAT; i++)
		PerformCurrentEvent(CombatParty[i]);
//	for(int i = 0; i < MAX_ENEMIES; i++)
//		PerformCurrentEvent(Enemies[i]);
	/*enemy cant properly target players anymore? idk*/
	
	
	if(InputPressed(PPINPUT_A)){
		if(_CombatUIStack.empty())
			OpenCombatUI(selectcursor);
	}



	/*
	switch(_TurnPhase){
	case ACTION_SELECT:
		break;
	case ACTING:
		break;
	case RESOLUTION:
		break;
	}*/
}

void TestAction(CombatEnt* owner, CombatEnt *target){
	CombatEffect *c = new CombatEffect();
	Vec2i pos;
	pos.x = RandomInt(0,GAME_RES_X);
	pos.y = RandomInt(0,GAME_RES_Y);
	c->anim = owner->animlist[0];
	c->location = pos;
	_EffectStack.push_back(c);
}

void UseItem(CombatEnt* owner, CombatEnt *target){

}

void Attack(CombatEnt* owner, CombatEnt *target){
	
	if(target==NULL) return;

	Vec2d dest = target->position;
	dest.y -= 15;
	AddMotion(owner,dest,40);
	dest.y -= 15;
	AddMotion(owner,dest,5);
	
	AddDamage(owner,target, 16);
	dest.x += 15;
	AddMotion(owner,dest,5);
	AddMotion(owner,owner->position_base,40);

}

void QueueAction(CombatAction* action){
	if((int)_ActionStack.size() < _MaxActions)
		_ActionStack.push(action);
}

void ExecuteQueue(){
	ExecuteAction(_ActionStack.front());
	_ActionStack.pop();
}

void ExecuteAction(CombatAction* action){
	if(action->text != NULL)
		SetText(action->text,&combatTextbox,0);

	action->Action(action->ent,action->target);
}

void AdvanceTurnPhase(){  //not implemented yet
	switch(_TurnPhase){
	case ACTION_SELECT:
		_TurnPhase = ACTING;
		break;
	case ACTING:
		_TurnPhase = RESOLUTION;
		break;
	case RESOLUTION:
		_TurnPhase = ACTION_SELECT;
		break;
	}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void DrawHPBar(CombatEnt *ent){
	if(ent == NULL) return;
	//jank ass hp bar code
	SDL_Rect hpbar;
	Uint32 barcolor;

	if(ent->chardata->health == ent->chardata->stats_base.max_health)
		barcolor = pCol_Blue;
	else if(((double)ent->chardata->health / (double)ent->chardata->stats_base.max_health) > 0.5)
		barcolor = pCol_DarkGreen;
	else if(((double)ent->chardata->health / (double)ent->chardata->stats_base.max_health) > 0.2)
		barcolor = pCol_DarkYellow;
	else
		barcolor = pCol_DarkRed;

	hpbar.x = (int)ent->position_base.x - 12;
	hpbar.y = (int)ent->position_base.y;// + (ent->s_offset.y>>4);
	hpbar.h = 4;
	hpbar.w = 25;
	DrawRectFill(&hpbar,&combatCamera,pCol_DarkGray);
	hpbar.w = ((double)ent->chardata->health / (double)ent->chardata->stats_base.max_health)*25;
	if(hpbar.w < 0) hpbar.w = 0;
	DrawRectFill(&hpbar,&combatCamera,barcolor);
	hpbar.w = 25;
	DrawRect(&hpbar,&combatCamera,pCol_Black);
}

void DrawHPBarFull(CombatEnt *ent){
	if(ent == NULL) return;
	//jank ass hp bar code
	SDL_Rect hpbar;
	Uint32 barcolor;

	if(ent->chardata->health == ent->chardata->stats_base.max_health)
		barcolor = pCol_Cyan;
	else if(((double)ent->chardata->health / (double)ent->chardata->stats_base.max_health) > 0.5)
		barcolor = pCol_Green;
	else if(((double)ent->chardata->health / (double)ent->chardata->stats_base.max_health) > 0.2)
		barcolor = pCol_Yellow;
	else
		barcolor = pCol_Red;

	hpbar.x = (int)ent->position_base.x - 50;
	hpbar.y = (int)ent->position_base.y + 6;// + (ent->s_offset.y>>4);
	hpbar.h = 6;
	hpbar.w = 100;
	DrawRectFill(&hpbar,&combatCamera,pCol_Gray);
	hpbar.w = ((double)ent->chardata->health / (double)ent->chardata->stats_base.max_health)*100;
	if(hpbar.w < 0) hpbar.w = 0;
	

	DrawRectFill(&hpbar,&combatCamera,barcolor);
	hpbar.w = 100;
	DrawRect(&hpbar,&combatCamera,pCol_Black);
}


void LoadCombatBG(){
	Sprite *bgSpr = LoadSprite(SPATH_BG_COMBAT,320,240,1);
	combatBG = LoadAnimation(bgSpr,0,0,1,1,1);
}

void DrawCombatBG(){
	Vec2i position = {0,0};
	DrawAnimation(combatBG,position,&combatCamera);
}

void DrawCombatUI(){
	DrawEffectStack();
	/*for(int i = 0; i < MAX_PARTY_COMBAT;i++){
		DrawHPBar(CombatParty[i]);
	}
	for(int i = 0; i < MAX_ENEMIES;i++){
		DrawHPBar(Enemies[i]);
	}*/
	DrawBattleTimer();
	DrawActionStack();
	DrawTextbox(&combatTextbox);
	if(!_CombatUIStack.empty()){
		for(int i = 0; i < (int)_CombatUIStack.size(); i++){
			DrawCombatUI(_CombatUIStack[i]);
		}
	}
}

void DrawEffectStack(){
	for(int i = 0; i < (int)_EffectStack.size(); i++){
		DrawAnimation(_EffectStack.at(i)->anim,_EffectStack.at(i)->location,&combatCamera);
	}
}

void DrawBattleTimer(){
	

	SDL_Rect timer;
	timer.y = 5;
	timer.h = 5;

	timer.x = 5;
	timer.w = ROUND_TIME;
	DrawRectFill(&timer,&combatCamera,backcolor);
	
	timer.x = 5+((ROUND_TIME - ActionTimer)>>1);
	timer.w = ActionTimer;
	DrawRectFill(&timer,&combatCamera,frontcolor);

	timer.x = 5;
	timer.w = ROUND_TIME;
	DrawRect(&timer,&combatCamera,pCol_Black);
}

void DrawActionStack(){
	SDL_Rect box;
	box.x = 5;
	box.y = 11;
	box.w = 5;
	box.h = 5;
	for(int i = 0; i < _MaxActions; i++){
		box.x = 5 + (i*6);
		if(i < (int)_ActionStack.size())
			DrawRectFill(&box,&combatCamera,pCol_White);
		DrawRect(&box,&combatCamera,pCol_Black);
	}

}

void InitiateAttack(){
	for(int i = 0; i < MAX_ENEMIES; i++)
		if(Enemies[i] != NULL)
			targetcursor->contents.cursor.target = Enemies[i];
	OpenCombatUI(targetcursor);
}

void QueuePlayerAttack(){
	
	CombatEnt *attacker = selectcursor->contents.cursor.target;
	CombatEnt *target = targetcursor->contents.cursor.target;

	QueueAttack(attacker,target);

}

void QueueAttack(CombatEnt *owner, CombatEnt *target){
	CombatAction *c = new CombatAction();
	c->ent = owner;
	c->target = target;
	
	copy_string(c->text,c->ent->chardata->name);
	strcat(c->text," attacks!");
	c->type = ACT_DEBUG;
	c->Action = Attack;
	QueueAction(c);
}

void Flee(){
	ExitCombat();
}