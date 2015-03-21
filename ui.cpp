#include "ui.h"
#include "dialogue.h"
#include "graphics.h"

//bool inMenu;		//are we currently in a menu?
Mouse mouse;
extern Camera uiCamera;
extern InputNode *_Inputs;

extern Menu _MenuStack[MAX_MENUS];

//Updates the mouse state (both buttons act as left for now)
void UpdateMouse(){
	if(SDL_GetMouseState(&mouse.pos.x,&mouse.pos.y))
		UpdateInput(&mouse.left,1);
	else
		UpdateInput(&mouse.left,0);
	
}

//Updates an Input struct based on what happened this frame and last frame
void UpdateInput(Input *input, bool current){
	input->pressed=0;
	input->held=0;
	input->released=0;
	if(current){
		if(input->prev==0){
			input->pressed=1;
		}else if(input->prev==1){
			input->held=1;
		}
		input->prev = 1;
	}else{
		if(input->prev==1){
			input->released=1;
		}
		input->prev = 0;
	}
}

MenuItem *LoadMenuItem(SDL_Rect box, Sprite *spr, Textbox *t, char* text){
	MenuItem * m = new MenuItem();
	SDL_Rect itemBox;
	itemBox.x = box.x;
	itemBox.y = box.y;
	itemBox.w = box.w;
	itemBox.h = box.h;
	m->bounds = itemBox;
	m->bgsprite = spr;
	m->text = t;
	SetText(text,m->text,0);
	return m;
}

void FreeMenuItem(MenuItem *m){
	FreeSprite(m->bgsprite);
//	delete(m->text);
//	delete(m);
}

void SetMenuItemAction(MenuItem *m, void (*func)() ){
	m->action = func;
}

Menu *LoadMenu(MenuType type,Vec2i loc){
	if(numMenus >= MAX_MENUS) 
		return NULL;
	int i;
	for(i = 0; i < MAX_MENUS; i++){
		if(!_MenuStack[i].used)
			break;
		else
			if(i+1 == MAX_MENUS)
				return NULL;
	}

	_MenuStack[i].used = true;
	_MenuStack[i].active = false;

	Textbox *yes,*no;
	SDL_Rect itemRect;
	Animation *cursorAnim = LoadAnimation(LoadSprite("sprites/menucursor.png",32,16,1),0,0,2,1,1);	
	_MenuStack[i].cursor.anim = cursorAnim;
	_MenuStack[i].cursor.location = 0;
	_MenuStack[i].location.x = loc.x;
	_MenuStack[i].location.y = loc.y;

	switch(type){
	case MENU_YES_NO:
		_MenuStack[i].location.x = 250;
		_MenuStack[i].location.y = 100;
		_MenuStack[i].numItems = 2;
		_MenuStack[i].itemsPerRow = 1;
		itemRect.x = _MenuStack[i].location.x;
		itemRect.y = _MenuStack[i].location.y;
		itemRect.w = 30;
		itemRect.h = 80;
		_MenuStack[i].location = itemRect;
		yes = new Textbox;
		no = new Textbox;
		LoadTextbox(yes,1,3,NULL,itemRect);
		_MenuStack[i].items[0] = LoadMenuItem(itemRect,NULL,yes,"Yes");
		itemRect.y += 14;
		LoadTextbox(no,1,3,NULL,itemRect);
		_MenuStack[i].items[1] = LoadMenuItem(itemRect,NULL,no,"No");
		break;
	case MENU_DEBUG:
		_MenuStack[i].location.x = 180;
		_MenuStack[i].location.y = 100;
		_MenuStack[i].numItems = 12;
		_MenuStack[i].itemsPerRow = 4;
		itemRect.x = _MenuStack[i].location.x;
		itemRect.y = _MenuStack[i].location.y;
		itemRect.w = 30;
		itemRect.h = 80;
		_MenuStack[i].location = itemRect;
		for(int k = 0; k < 4; k++){
			for(int j = 0; j < 4; j++){
				yes = new Textbox;
				itemRect.x = _MenuStack[i].location.x + j*30;
				itemRect.y = _MenuStack[i].location.y + k*15;
				LoadTextbox(yes,1,3,NULL,itemRect);
				_MenuStack[i].items[j + (4*k)] = LoadMenuItem(itemRect,NULL,yes,"Yes");
			}
		}
		_MenuStack[i].cursor.location = 0;
		break;
	default:
		break;
	}
	return &_MenuStack[i];
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
	if(((_Inputs->input&PPINPUT_LEFT)&&!(_Inputs->prev->input & PPINPUT_LEFT))||(InputBuffered(_Inputs,PPINPUT_LEFT,6*INPUT_BUFFER))){
		DecrementCursor(m);	
	}
	if(((_Inputs->input&PPINPUT_RIGHT)&&!(_Inputs->prev->input & PPINPUT_RIGHT))||(InputBuffered(_Inputs,PPINPUT_RIGHT,6*INPUT_BUFFER))){
		IncrementCursor(m);
	}
	if(((_Inputs->input&PPINPUT_UP)&&!(_Inputs->prev->input & PPINPUT_UP))||(InputBuffered(_Inputs,PPINPUT_UP,6*INPUT_BUFFER))){
		int numRows = m->numItems/m->itemsPerRow;
		if((m->cursor.location - m->itemsPerRow) < 0){
			m->cursor.location += m->itemsPerRow*(numRows-1);
		}
		else
			m->cursor.location -= m->itemsPerRow;		
	}
	if(((_Inputs->input&PPINPUT_DOWN)&&!(_Inputs->prev->input & PPINPUT_DOWN))||(InputBuffered(_Inputs,PPINPUT_DOWN,6*INPUT_BUFFER))){
		int numRows = m->numItems/m->itemsPerRow;
		if((m->cursor.location + m->itemsPerRow) >= m->numItems){
			m->cursor.location -= m->itemsPerRow*(numRows-1);
		}
		else
			m->cursor.location += m->itemsPerRow;
	}
	if((_Inputs->input & PPINPUT_B)&&!(_Inputs->prev->input & PPINPUT_B)){
		if(m->items[m->cursor.location]->action != NULL)
			m->items[m->cursor.location]->action();
	}
}

void ClearMenus(){

}

void DrawMenu(Menu *m){
	if(m==NULL) return;
	Vec2i loc;
	loc.x = m->location.x;
	loc.y = m->location.y;
	for(int i = 0; i < m->numItems; i++){
		loc.x = m->items[i]->bounds.x;
		loc.y = m->items[i]->bounds.y;
		DrawSprite(m->items[i]->bgsprite,0,loc,&uiCamera);
		DrawTextbox(m->items[i]->text);
	}
	loc.x = m->items[m->cursor.location]->text->box.x;
	loc.y = m->items[m->cursor.location]->text->box.y + 6;
	DrawAnimation(m->cursor.anim,loc,&uiCamera);
}

void IncrementCursor(Menu *m){
	m->cursor.location = (m->cursor.location+1)%m->numItems;
}

void DecrementCursor(Menu *m){
	m->cursor.location = (m->cursor.location+(m->numItems-1))%m->numItems;
}

void DrawOverworldUI(){

}

Menu *OpenMenuYesNo(Vec2i loc, void (*YesFunc)(),void (*NoFunc)()){
	_InMenu = true;
	Menu *m = LoadMenu(MENU_YES_NO,loc);
	SetMenuItemAction(m->items[0],YesFunc);
	SetMenuItemAction(m->items[1],NoFunc);
	//currentMenu = m;
	return m;
}

void CancelMenu(){
	int i;
	for(i = MAX_MENUS-1; i >= 0; i--)
		if(_MenuStack[i].used&&_MenuStack[i].active)
			break;
	FreeMenu(&_MenuStack[i]);
//	_MenuStack[i].used = false;
}