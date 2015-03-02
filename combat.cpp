#include "combat.h"

extern Camera combatCamera;
Animation *combatBG;



void LoadCombatBG(){
	Sprite *bgSpr = LoadSprite("sprites/battlebg.png",320,240,1);
	combatBG = LoadAnimation(bgSpr,0,0,10,1,1);
}

void DrawCombatBG(){
	Vec2i position = {0,0};
	DrawAnimation(combatBG,position,&combatCamera);
}