#include"combat_ent.h"
#include "combat.h"


CombatEnt *Party[MAX_PARTY];
CombatEnt *Enemies[MAX_ENEMIES];
extern Camera combatCamera;

Vec2i PartySlots[MAX_PARTY];
Vec2i EnemySlots[MAX_ENEMIES];

void InitCombatSlots(){
	Vec2i pos = {30,180};
	for(int i = 0; i < MAX_PARTY;i++){
		PartySlots[i] = pos;
		pos.x+= 40;
	}
	pos.x = 40;
	pos.y = 80;
	for(int i = 0; i < MAX_ENEMIES;i++){
		EnemySlots[i] = pos;
		if(i%2 != 0)
			pos.y -= 20;
		else
			pos.y += 020;
		pos.x+= 50;
	}
}

void LoadAllies(){
	//Vec2i pos = {50,40};
	Sprite *bs = LoadSprite(SPATH_ALLY_GENERIC,32,32,4);
	for(int i = 0; i < MAX_PARTY;i++)
		Party[i] = NULL;

	for(int i = 0; i < 2;i++){
		CombatEnt *ent = LoadCombatEnt();
		ent->name = "guy";

		ent->friendly = true;

		ent->numAnims = 0;

		ent->s_offset.x = 16;
		ent->s_offset.y = 16;
		ent->animation = 0;
		ent->animlist[ent->numAnims] = LoadAnimation(bs,i,i,1,0,0);
		ent->numAnims = 1;
		ent->position_base.x = PartySlots[i].x;
		ent->position_base.y = PartySlots[i].y;
		ent->position = ent->position_base;

		Party[i] = ent;
	}
}

void LoadEnemies(){
		Vec2i pos = {30,70};
	Sprite *bs = LoadSprite(SPATH_ENEMY_GENERIC,128,128,1);
	for(int i = 0; i < MAX_ENEMIES;i++)
		Enemies[i] = NULL;
	int i =3;

		CombatEnt *ent = LoadCombatEntCFG("testfiles/enemy-test.json");

		ent->stats_base.max_health = 300;
		ent->health = ent->stats_base.max_health;
		
	//	ent->name = "ENRAGED EGG";

		ent->friendly = false;

		ent->numAnims = 0;

		ent->s_offset.x = 64;
		ent->s_offset.y = 64;
		ent->animation = 0;
		ent->animlist[ent->numAnims] = LoadAnimation(bs,0,0,1,0,0);
		ent->numAnims = 1;
		
		ent->position_base.x = EnemySlots[i].x;
		ent->position_base.y = EnemySlots[i].y;
		ent->position = ent->position_base;

		ent->exhaustion = 240;

		Enemies[i] = ent;
	
}

CombatEnt *LoadCombatEnt(){
	CombatEnt *ent = new CombatEnt();
	
	ent->stats_base.max_health = 100;		
	ent->health = ent->stats_base.max_health;	

	return ent;
}

void DrawAllies(){
	for(int i = 0; i < MAX_PARTY;i++){
		if(Party[i]!=NULL){
			DrawAnimation(Party[i]->animlist[Party[i]->animation],(Vec2i)Party[i]->position-Party[i]->s_offset,&combatCamera);
		//	DrawHPBar(Party[i]);
		}
	}
}


void DrawEnemies(){
	for(int i = 0; i < MAX_ENEMIES;i++){
		if(Enemies[i]!=NULL){
			DrawAnimation(Enemies[i]->animlist[Enemies[i]->animation],Enemies[i]->position-Enemies[i]->s_offset,&combatCamera);
		//	DrawHPBar(Enemies[i]);
		}
	}
}

void EnemyAI(){
	for(int i = 0; i < MAX_ENEMIES;i++){
		if(Enemies[i]!=NULL){
			Enemies[i]->exhaustion--;
			if(Enemies[i]->exhaustion <= 0){
				CombatEntThink(Enemies[i]);
				Enemies[i]->exhaustion = 400;
			}
		}
	}
}

void CombatEntThink(CombatEnt *ent){
	CombatEnt *target;
	int targ;
	do{
		targ = RandomInt(0,MAX_PARTY);
	}while(Party[targ] == NULL);
	target = Party[targ];
	QueueAttack(ent,target);

}

void PerformCurrentEvent(CombatEnt *ent){
	if(ent==NULL) return;
	if(ent->events.empty()) return;
	switch(ent->events.front().type){
	case EVENT_DEBUG:
		break;
	case EVENT_MOTION:
		PerformMotion(ent,ent->events.front().e.motion);
		break;
	case EVENT_DAMAGE:
		PerformDamage(ent,ent->events.front().e.damage);
		break;
	}
}


void AddMotion(CombatEnt *ent, Vec2d dest, int duration){
	ActionEvent m;
	m.type = EVENT_MOTION;
	if(!ent->events.empty()){
		m.e.motion.src = ent->position_target;		//replace with position of last motion event in stack
	}else
		m.e.motion.src = ent->position;
	m.e.motion.dest = dest;
	ent->position_target = dest;
	m.e.motion.duration = duration;
	ent->events.push(m);
}

void AddMotionRelative(CombatEnt *ent, Vec2d dest, int duration){
	if(!ent->events.empty()){
		dest = dest + ent->position_target;	//replace with position of last motion event in stack
	}else
		dest = dest + ent->position;
	AddMotion(ent,dest,duration);
}

void AddDamage(CombatEnt *ent, CombatEnt *target, int damage){
	ActionEvent d;
	d.type = EVENT_DAMAGE;
	d.e.damage.damage = damage;
	d.e.damage.target = target;
	ent->events.push(d);
}


void PerformMotion(CombatEnt *ent, EntMotion m){
	Vec2d increment;
	increment = (m.dest - m.src);
	increment.x /= m.duration;
	increment.y /= m.duration;


	if(ent->position == m.dest){
		ent->events.pop();
	}else{
		if(		//if going one more increment would pass the destination
			((ent->position.x <= m.dest.x && ent->position.x + increment.x >= m.dest.x)||
			(ent->position.x >= m.dest.x && ent->position.x + increment.x <= m.dest.x))&&
			((ent->position.y <= m.dest.y && ent->position.y + increment.y >= m.dest.y)||
			(ent->position.y >= m.dest.y && ent->position.y + increment.y <= m.dest.y))
			){
			ent->position = m.dest;
			ent->events.pop();
		}else
			ent->position = ent->position + increment;
	}
}


void PerformDamage(CombatEnt *ent, EntDamage d){
	d.target->health -= d.damage;
	ent->events.pop();
}

CombatEnt *FindCombatEnt(CombatEnt *current, Vec2i dir){ //direction to look for combat ent
	CombatEnt** ent_list;
	int list_len;

	if(current->friendly){
		ent_list = Party;
		list_len = MAX_PARTY;
	}else{
		ent_list = Enemies;
		list_len = MAX_ENEMIES;
	}

	int cursor = 0;
	for(;cursor < list_len;cursor++){
		if(ent_list[cursor] != NULL && ent_list[cursor]->position_base == current->position_base)
			break;
	}
	if(dir.x > 0 || dir.y > 0)
		do{
			cursor++;
			if(cursor >= list_len)
				cursor = 0;
		}while(ent_list[cursor] == NULL);
	else if(dir.x < 0 || dir.y < 0)
		do{
			cursor--;
			if(cursor < 0)
				cursor = list_len - 1;
		}while(ent_list[cursor] == NULL);

	return ent_list[cursor];

	//EVENTUALLY SUPPORT THIS BUT IM LAZY SO WE ITERATE FOR NOW:

	/* hoo boy time to find the nearest neighbor */
	//int buffer; //how much y variance we want to allow if looking for nearest x, or vice-versa
	//for now let's just iterate....
	/*int cursor = 0;
	for(;cursor < list_len;cursor++){
		if(ent_list[cursor]->position_base == current->position_base)
			break;
	}
	cursor = (cursor+dir.y)%list_len;
	return ent_list[cursor];*/
}