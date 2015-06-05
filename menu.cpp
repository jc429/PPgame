
#include "dialogue.h"
#include "graphics.h"
#include "inventory.h"

extern Camera uiCamera;
extern InputNode *_Inputs;
Menu *_CurrentMenu;
vector<struct Menu_T*> _MenuStack;
vector<struct Menu_T*> *curMenuStack = NULL;
//extern vector<struct Menu_T*> _CombatMenuStack;

MenuItem *LoadMenuItem(SDL_Rect box, Sprite *spr, Textbox *t, char* text){
	MenuItem *m = new MenuItem;
	m->bounds.x = box.x;
	m->bounds.y = box.y;
	m->bounds.w = box.w;
	m->bounds.h = box.h;
	m->bgsprite = spr;
	m->text = t;
	m->text->box.y -= 6;
	SetText(text,m->text,0);
	return m;
}

void FreeMenuItem(MenuItem *m){
	FreeSprite(m->bgsprite);
//	delete(m->text);
//	delete(m);
}

void SetMenuItemAction(MenuItem *m, void (*func)(), char* name){
	m->action = func;
	if(name){
		copy_string(m->text->lines[0],name);
	}
}

Menu *LoadMenu(MenuType mtype,Vec2i *loc){	
	//Loads a menu, with special conditions based on type

	Textbox *textboxes[8];
	for(int i = 0; i < 8; i++){
		textboxes[i] = NULL;
	}

	SDL_Rect itemRect;
	Sprite *cursorSpr = NULL;

	Menu *m = new Menu();

	m->cursor.location = 0;
	if(loc != NULL){
		SetRect(m->location,loc);
	}
	
	m->spr = LoadSprite(SPATH_PANEL_DEF,4,4,3);
	m->type = mtype;

	int cframes = 2; //num frames in cursor animation
	cursorSpr = LoadSprite("sprites/menucursor.png",8,8,4);
	for(int i = 0; i < 4; i++){
		m->cursor.anim_active[i] = LoadAnimation(cursorSpr,i*cframes,i*cframes,2,1,1,18);
		m->cursor.anim_inactive[i] = LoadAnimation(cursorSpr,i*cframes,i*cframes,1,1,1,18); //should be different eventually
		m->cursor.anim[i] = m->cursor.anim_active[i];
	}

	switch(mtype){
	case MENU_PAUSE:
		m->location.x = 10;
		m->location.y = 10;
		m->numItems = 6;
		m->itemsPerRow = 1;
		SetRect(itemRect,m->location.x,m->location.y,60,16);
		m->location = itemRect;
		

		for(int i = 0; i < m->numItems; i++){
			textboxes[i] = new Textbox();
			LoadTextbox(textboxes[i],1,6,NULL,itemRect);
			m->items[i] = LoadMenuItem(itemRect,NULL,textboxes[i]);
			if(i + 1 < m->numItems){
				itemRect.y += 16;
				m->location.h += 16;
			}
		}


		break;
	case MENU_YES_NO:
		if(loc == NULL){
			SetRect(m->location,250,116);
		}
		m->numItems = 2;
		m->itemsPerRow = 1;

		SetRect(itemRect,m->location.x,m->location.y,30,16);
		m->location = itemRect;

		textboxes[0] = new Textbox();
		LoadTextbox(textboxes[0],1,3,NULL,itemRect);
		m->items[0] = LoadMenuItem(itemRect,NULL,textboxes[0],"Yes");

		itemRect.y += 18;
		m->location.h += 18;

		textboxes[1] = new Textbox();
		LoadTextbox(textboxes[1],1,3,NULL,itemRect);
		m->items[1] = LoadMenuItem(itemRect,NULL,textboxes[1],"No");
		break;
	case MENU_BATTLE:		////////////////////////////////////////////////////////
		m->location.x = 190;
		m->location.y = 164;
		m->numItems = 4;
		m->itemsPerRow = 2;
		itemRect.x = m->location.x;
		itemRect.y = m->location.y;
		itemRect.w = 60;
		itemRect.h = 16;

		m->location = itemRect;
		m->location.w += 64;
		m->location.h += 16;


		textboxes[0] = new Textbox;
		//bg = LoadSprite(SPATH_MENU_BG_COMBAT,64,16,1);
		char* menulist[4];
		menulist[0] = "Attack";
		menulist[1] = "Item";
		menulist[2] = "Cry";
		menulist[3] = "Flee";
		for(int k = 0; k < 2; k++){
			for(int j = 0; j < 2; j++){
				textboxes[0] = new Textbox;
				itemRect.x = m->location.x + j*64;
				itemRect.y = m->location.y + k*16;
				LoadTextbox(textboxes[0],1,6,NULL,itemRect);
				m->items[j + (2*k)] = LoadMenuItem(itemRect,NULL,textboxes[0],menulist[j + (2*k)]);
			}
		}
		
		break;
	default:
		break;
	}
	
	return m;
}

void FreeMenu(Menu *m){
	for(int i = 0; i < m->numItems; i++){
	//	FreeMenuItem(m->items[i]);
	}
	//m->numItems = 0;
	//m->active = false;
	m->used = false;
}

void UpdateMenu(Menu *m){
	if((InputPressed(PPINPUT_LEFT))||(InputBuffered(_Inputs,PPINPUT_LEFT,3*INPUT_BUFFER))){
		DecrementCursor(m);	
	}
	if((InputPressed(PPINPUT_RIGHT))||(InputBuffered(_Inputs,PPINPUT_RIGHT,3*INPUT_BUFFER))){
		IncrementCursor(m);
	}
	if((InputPressed(PPINPUT_UP))||(InputBuffered(_Inputs,PPINPUT_UP,3*INPUT_BUFFER))){
		int numRows = m->numItems/m->itemsPerRow;
		if((m->cursor.location - m->itemsPerRow) < 0){
			m->cursor.location += m->itemsPerRow*(numRows-1);
		}
		else
			m->cursor.location -= m->itemsPerRow;		
	}
	if((InputPressed(PPINPUT_DOWN))||(InputBuffered(_Inputs,PPINPUT_DOWN,3*INPUT_BUFFER))){
		int numRows = m->numItems/m->itemsPerRow;
		if((m->cursor.location + m->itemsPerRow) >= m->numItems){
			m->cursor.location -= m->itemsPerRow*(numRows-1);
		}
		else
			m->cursor.location += m->itemsPerRow;
	}
	if(InputPressed(PPINPUT_A)){
		if(m->items[m->cursor.location]->action != NULL)
			m->items[m->cursor.location]->action();
	}
}

void IncrementCursor(Menu *m){
	m->cursor.location = (m->cursor.location+1)%m->numItems;
}

void DecrementCursor(Menu *m){
	m->cursor.location = (m->cursor.location+(m->numItems-1))%m->numItems;
}

void ClearMenus(){

}

void DrawMenu(Menu *m){
	if(m==NULL) return;
	Vec2i loc;
	loc.x = m->location.x;
	loc.y = m->location.y;
	DrawMenuSprite(m);

	for(int i = 0; i < m->numItems; i++){
		loc.x = m->items[i]->bounds.x;
		loc.y = m->items[i]->bounds.y;
	
		DrawSprite(m->items[i]->bgsprite,0,loc,&uiCamera);
		DrawTextbox(m->items[i]->text);
	}

	loc.x = m->items[m->cursor.location]->bounds.x; // - m->cursor.anim[0]->sprite->w;
	loc.y = m->items[m->cursor.location]->bounds.y; //- m->cursor.anim[0]->sprite->h;
	DrawAnimation(m->cursor.anim[0],loc,&uiCamera);
	loc.x += m->items[m->cursor.location]->bounds.w - m->cursor.anim[0]->sprite->w;
	DrawAnimation(m->cursor.anim[1],loc,&uiCamera);
	loc.x = m->items[m->cursor.location]->bounds.x;// - m->cursor.anim[0]->sprite->w;
	loc.y += m->items[m->cursor.location]->bounds.h - m->cursor.anim[0]->sprite->h;
	DrawAnimation(m->cursor.anim[2],loc,&uiCamera);
	loc.x += m->items[m->cursor.location]->bounds.w - m->cursor.anim[0]->sprite->w;	
	DrawAnimation(m->cursor.anim[3],loc,&uiCamera);

}

void DrawMenuSprite(Menu *m){

	DrawPanel(m->location,m->spr);
}


Menu *LoadMenuYesNo(Vec2i *loc, char *yes, char *no, void (*YesFunc)(),void (*NoFunc)()){
	//_InMenu = true;
	Menu *m = LoadMenu(MENU_YES_NO,loc);
	SetMenuItemAction(m->items[0],SelectAnswer1,yes);
	SetMenuItemAction(m->items[1],SelectAnswer2,no);
	if(*YesFunc != NULL)
		SetMenuItemAction(m->items[0],YesFunc);
	if(*NoFunc != NULL)
		SetMenuItemAction(m->items[1],NoFunc);
	return m;
}

Menu *LoadCustomMenu(int numItems, char itemNames[6][16]){
	if(numItems > 6)
		numItems = 6;
	if(numItems < 0)
		numItems = 0;
	Menu *m;

	Textbox *textboxes[8];
	SDL_Rect itemRect;
	Vec2i loc;
	SetVec2i(loc,216,158 - (numItems * 18));
	switch(numItems){
	case 6:
		m = LoadMenu(MENU_CUSTOM_6,&loc);
		break;
	case 5:
		m = LoadMenu(MENU_CUSTOM_5,&loc);
		break;
	case 4:
		m = LoadMenu(MENU_CUSTOM_4,&loc);
		break;
	case 3:
		m = LoadMenu(MENU_CUSTOM_3,&loc);
		break;
	default:
		m = LoadMenu(MENU_CUSTOM_2,&loc);
		break;
	}

	SetRect(m->location,loc.x,loc.y,96,16);
	itemRect = m->location;
	for(int i = 0; i < 8; i++)
		textboxes[i] = NULL;
	for(int i = 0; i < numItems; i++){
		textboxes[i] = new Textbox();
		LoadTextbox(textboxes[i],1,10,NULL,itemRect);
		m->items[i] = LoadMenuItem(itemRect,NULL,textboxes[i],itemNames[i]);

		if(i + 1 < numItems){
			itemRect.y += 18;
			m->location.h += 18;
		}
	}
	m->numItems = numItems;
	m->itemsPerRow = 1;
	switch(numItems){
	case 6:
		SetMenuItemAction(m->items[5],SelectAnswer6,itemNames[5]);
	case 5:
		SetMenuItemAction(m->items[4],SelectAnswer5,itemNames[4]);
	case 4:
		SetMenuItemAction(m->items[3],SelectAnswer4,itemNames[3]);
	case 3:
		SetMenuItemAction(m->items[2],SelectAnswer3,itemNames[2]);
	case 2:
		SetMenuItemAction(m->items[1],SelectAnswer2,itemNames[1]);
	default:
		SetMenuItemAction(m->items[0],SelectAnswer1,itemNames[0]);
		break;
	}
	return m;
}

void OpenMenu(Menu *m,vector<struct Menu_T*> *stack){
	m->active = true;
	m->cursor.location = 0;
	if(stack != NULL){
		curMenuStack = stack;
	}else
		curMenuStack = &_MenuStack;
		
	curMenuStack->push_back(m);
}



void CancelMenu(){
	if(GetCurrentState() == OVERWORLD)
		curMenuStack = &_MenuStack;
	
	//if(GetCurrentState() == COMBAT)
	//	return;//fix it so it works in combat
	curMenuStack->back()->active = false;
	if(curMenuStack->back()->type == MENU_PAUSE)
		HideWallet();
	curMenuStack->pop_back();
}

void AdvanceAndCancel(int steps){
	AdvanceText(steps);
	CancelMenu();
}

void SelectAnswer1(){
	AdvanceAndCancel(0);
}
void SelectAnswer2(){
	AdvanceAndCancel(1);
}
void SelectAnswer3(){
	AdvanceAndCancel(2);
}
void SelectAnswer4(){
	AdvanceAndCancel(3);
}
void SelectAnswer5(){
	AdvanceAndCancel(4);
}
void SelectAnswer6(){
	AdvanceAndCancel(5);
}