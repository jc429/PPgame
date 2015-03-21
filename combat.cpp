#include "combat.h"

extern Camera combatCamera;
Animation *combatBG;
extern Textbox combatTextbox;

CombatEnt *Party[MAX_PARTY];
CombatEnt *Enemies[MAX_ENEMIES];

typedef enum BattlePhase_T{
	BATTLE_INTRO,
	BATTLE_MID,
	BATTLE_END
}BattlePhase;

typedef enum TurnPhase_T{
	ACTION_SELECT,
	ACTING,
	RESOLUTION,
}TurnPhase;

static BattlePhase _BattlePhase;
static TurnPhase _TurnPhase;

void InitCombat(){
	Sprite *ctspr = LoadSprite("sprites/combox.png",320,40,1);
	InitMainTextbox(&combatTextbox,2,40,ctspr);

	SetText("Smoke 420 weed blaze it 6969 xxDeviant_FishXxxX Theresa Smells",&combatTextbox,0);
}

void EnterCombat(){
	_BattlePhase = BATTLE_INTRO;
	InitCombat();
	//play combat music
	LoadEnemies();
	LoadAllies();

}
void UpdateCombat(){
	switch(_BattlePhase){
	case BATTLE_MID:
		UpdateTurn();
		break;
	}
}

void UpdateTurn(){
	switch(_TurnPhase){
	case ACTION_SELECT:
		break;
	case ACTING:
		break;
	case RESOLUTION:
		break;
	}
}

void AdvanceTurnPhase(){
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
void LoadEnemies(){
		Vec2i pos = {150,50};
	Sprite *bs = LoadSprite("sprites/enemy.png",56,50,1);
	for(int i = 0; i < MAX_ENEMIES;i++){
		CombatEnt *ent = new CombatEnt();
		ent->stats_base.max_health = 100;
		ent->health = ent->stats_base.max_health;
		ent->s_offset.x = 28;
		ent->s_offset.y = 25;
		ent->animation = 0;
		ent->animlist[ent->numAnims] = LoadAnimation(bs,0,0,1,0,0);
		ent->numAnims = 1;
		ent->position.y = (pos.y*i);
		ent->position.x = 10*i+pos.x;
		Enemies[i] = ent;
	}
}

void DrawEnemies(){
	for(int i = 0; i < MAX_ENEMIES;i++){
		DrawAnimation(Enemies[i]->animlist[Enemies[i]->animation],Enemies[i]->position-Enemies[i]->s_offset,&combatCamera);
		DrawHPBar(Enemies[i]);
	}
}

void DrawHPBar(CombatEnt *ent){
	//jank ass hp bar code
	SDL_Rect hpbar;
	hpbar.x = ent->position.x;
	hpbar.y = ent->position.y + ent->animlist[0]->sprite->h - 10;
	hpbar.w = ent->health>>2;
	hpbar.h = 4;
	DrawRect(&hpbar,&combatCamera);
}

void LoadAllies(){
	Vec2i pos = {50,50};
	Sprite *bs = LoadSprite("sprites/mad.png",32,32,4);
	for(int i = 0; i < MAX_PARTY;i++){
		CombatEnt *ent = new CombatEnt();
		ent->stats_base.max_health = 100;
		ent->health = 100;
		ent->s_offset.x = 16;
		ent->s_offset.y = 16;
		ent->animation = 0;
		ent->animlist[ent->numAnims] = LoadAnimation(bs,i,i,1,0,0);
		ent->numAnims = 1;
		ent->position.y = 20+(pos.y*i);
		ent->position.x = pos.x;
		Party[i] = ent;
	}
}

void DrawAllies(){
	for(int i = 0; i < MAX_PARTY;i++){
		DrawAnimation(Party[i]->animlist[Party[i]->animation],Party[i]->position-Party[i]->s_offset,&combatCamera);
		DrawHPBar(Party[i]);
	}
}

void LoadCombatBG(){
	Sprite *bgSpr = LoadSprite("sprites/battlebg.png",320,240,1);
	combatBG = LoadAnimation(bgSpr,0,0,10,1,1);
}

void DrawCombatBG(){
	Vec2i position = {0,0};
	DrawAnimation(combatBG,position,&combatCamera);
}

void DrawCombatUI(){
	DrawTextbox(&combatTextbox);
}