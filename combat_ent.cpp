#include"combat_ent.h"
#include "combat.h"


CombatEnt *CombatParty[MAX_PARTY_COMBAT];
CombatEnt *Enemies[MAX_ENEMIES];
extern Camera combatCamera;

Vec2i CombatPartySlots[MAX_PARTY_COMBAT];
Vec2i EnemySlots[MAX_ENEMIES];

extern int playerid;

vector<CombatEnt*> enemylist;
int CombatPartyIDs[MAX_PARTY_COMBAT];
int EnemyIDs[MAX_ENEMIES];

void InitCombatSlots(){
	Vec2i pos = {60,160};
	for(int i = 0; i < MAX_PARTY_COMBAT;i++){
		CombatPartySlots[i] = pos;
		pos.x += 8;
		pos.y -= 12;
	}
	pos.x = GAME_RES_X - 60;
	pos.y = 160;
	for(int i = 0; i < MAX_ENEMIES;i++){
		EnemySlots[i] = pos;
		pos.x -= 8;
		pos.y -= 12;
	}
}

void AddToCombatParty(int charid){
	for(int i = 0; i < MAX_PARTY_COMBAT; i++){
		if(CombatPartyIDs[i] == 0){
			CombatPartyIDs[i] = charid;
			return;
		}
	}
}

void SetEnemies(int num, ...){
	va_list args;

	for(int i = 0; i < MAX_ENEMIES; i++)
		EnemyIDs[i] = 0;

	va_start(args,num);
	for(int i = 0; i < num; i++){
		if(i >= MAX_ENEMIES) return;
		EnemyIDs[i] = va_arg(args,int);
	}
	va_end(args);
}



void LoadAllies(){
	//Vec2i pos = {50,40};
	Sprite *bs = LoadSprite(SPATH_ALLY_GENERIC,64,64,5,32,48);
	Sprite *as = LoadSprite(SPATH_NPC_GENERIC,64,64,5,32,48);
	for(int i = 0; i < MAX_PARTY_COMBAT;i++)
		CombatParty[i] = NULL;

	for(int i = 0; i < 4;i++){
		if(CombatPartyIDs[i] == 0)
			continue;

		CombatEnt *ent = LoadCombatEnt(CombatPartyIDs[i]);
		//copy_string(ent->name,"guy");

		ent->friendly = true;

		ent->numAnims = 0;
		ent->animation = 0;
		if(i == 0)
			ent->animlist[ent->numAnims] = LoadAnimation(bs,5,5,1,0,0);
		else
			ent->animlist[ent->numAnims] = LoadAnimation(as,5,5,1,0,0);

		ent->numAnims = 1;
		ent->position_base.x = CombatPartySlots[i].x;
		ent->position_base.y = CombatPartySlots[i].y;
		ent->position = ent->position_base;

		CombatParty[i] = ent;
	}
}

void LoadEnemies(){
	//	Vec2i pos = {30,120};
	Sprite *bs = LoadSprite("sprites/enemies/begg.png",128,128,1,64,96);
	Sprite *es = LoadSprite("sprites/enemies/goomb.png",64,64,4,32,64);
	for(int i = 0; i < MAX_ENEMIES;i++)
		Enemies[i] = NULL;
	for(int i = 0; i < MAX_ENEMIES;i++){
		if(EnemyIDs[i] == 0)
			continue;

		//clone the enemy from the enemy list into a new usable copy
		CombatEnt *ent = new CombatEnt;		//FIX: replace the enemy list with combatents instead of pointers so i dont have to use new
		memcpy(ent,FindEnemy(EnemyIDs[i]),sizeof(CombatEnt));
		if(ent == NULL)
			continue;
		//copy chardata
		ent->chardata = new CharData;
		memcpy(ent->chardata,FindEnemy(EnemyIDs[i])->chardata,sizeof(CharData));

		//CombatEnt *ent = LoadCombatEnt(EnemyIDs[i]);
	//	CombatEnt *ent = LoadCombatEntCFG("testfiles/enemy-test.json");

//		ent->chardata->stats_base.max_health = 300;
		ent->chardata->health = ent->chardata->stats_base.max_health;

		ent->friendly = false;

	//	ent->numAnims = 0;


	//	ent->animation = 0;
	//	ent->animlist[ent->numAnims] = LoadAnimation(es,0,0,4,1,1,8);
	//	ent->numAnims = 1;
		
		ent->position_base.x = EnemySlots[i].x;
		ent->position_base.y = EnemySlots[i].y;
		ent->position = ent->position_base;

		ent->exhaustion = 240 + 48*i;

		Enemies[i] = ent;
	}
}

CombatEnt *FindEnemy(int id){
	int i = 0;
	for (std::vector<CombatEnt*>::iterator it = enemylist.begin(); it != enemylist.end(); ++it) {
		if(enemylist.at(i)->chardata->id == id)
			return enemylist.at(i);
		i++;
	}
	return NULL;
}

CombatEnt *LoadCombatEnt(int charid){
	CombatEnt *ent = new CombatEnt();

	ent->animation = 0;
	ent->numAnims = 0;

	ent->chardata = LoadCharData(charid);

	ent->chardata->stats_base.max_health = 100;		
	ent->chardata->health = ent->chardata->stats_base.max_health;	

	return ent;
}

void AddCombatEntAnim(CombatEnt *cent, Animation *anim, int position){
	cent->animlist[position] = anim;
	cent->numAnims++;
}

void DrawAllies(){
	for(int i = MAX_PARTY_COMBAT- 1; i >=0; i--){
		if(CombatParty[i]!=NULL){
			DrawAnimation(CombatParty[i]->animlist[CombatParty[i]->animation],CombatParty[i]->position,&combatCamera);
			if(i==0)
				DrawHPBarFull(CombatParty[i]);
			else
				DrawHPBar(CombatParty[i]);
		}
	}
}


void DrawEnemies(){
	for(int i =  MAX_ENEMIES-1; i >=0;i--){
		if(Enemies[i]!=NULL){
			DrawAnimation(Enemies[i]->animlist[Enemies[i]->animation],Enemies[i]->position,&combatCamera);
			if(i==0)
				DrawHPBarFull(Enemies[i]);
			else
				DrawHPBar(Enemies[i]);
		}
	}
}

void EnemyAI(){
	for(int i = 0; i < MAX_ENEMIES;i++){
		if(Enemies[i]!=NULL){
			Enemies[i]->exhaustion--;
			if(Enemies[i]->exhaustion <= 0){
				CombatEntThink(Enemies[i]);
				Enemies[i]->exhaustion = 300;
			}
		}
	}
}

void CombatEntThink(CombatEnt *ent){
	if(ent->chardata->health <=0)
		ExitCombat();		//replace this

	CombatEnt *target;
	int targ;
	do{
		targ = RandomInt(0,MAX_PARTY_COMBAT);
	}while(CombatParty[targ] == NULL);
	target = CombatParty[targ];
	QueueAttack(ent,target);

}

void PerformCurrentEvent(CombatEnt *ent){
	if(ent==NULL) 
		return;
	if(ent->events.empty()) 
		return;

	ActionEvent ae;
	//printf("%i \n",(ent->events.front()));
	ae = ent->events.front();
	switch(ae.type){
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
			(ent->position.x >= m.dest.x && ent->position.x + increment.x <= m.dest.x))||
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
	d.target->chardata->health -= d.damage;
	ent->events.pop();
}


CombatEnt *FindCombatEnt(CombatEnt *current, Vec2i dir){ //direction to look for combat ent
	CombatEnt** ent_list;
	int list_len;

	if(current->friendly){
		ent_list = CombatParty;
		list_len = MAX_PARTY_COMBAT;
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



void RoatateAllies(){
	CombatEnt *temp = CombatParty[0];
	for (int i = 0; i < MAX_PARTY_COMBAT-1; i++){
		CombatParty[i] = CombatParty[i+1];
	}
	CombatParty[MAX_PARTY_COMBAT-1] = temp;
	for (int i = 0; i < MAX_PARTY_COMBAT; i++){
		if(CombatParty[i] == NULL) return;
		CombatParty[i]->position_base.x = CombatPartySlots[i].x;
		CombatParty[i]->position_base.y = CombatPartySlots[i].y;
	}
}

void RoatateEnemies(){

}