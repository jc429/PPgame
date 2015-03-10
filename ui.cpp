#include "ui.h"
#include "dialogue.h"
#include "graphics.h"

//bool inMenu;		//are we currently in a menu?
Mouse mouse;
extern Camera uiCamera;
extern InputNode *_Inputs;
extern Menu *currentMenu;

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
	delete(m->text);
//	delete(m);
}

void SetMenuItemAction(MenuItem *m, void (*func)() ){
	m->action = func;
}

Menu *LoadMenu(MenuType type,Vec2i loc){
	Menu *m = new Menu;
	Textbox *yes,*no;
	SDL_Rect itemRect;
	
	m->cursor.location = 0;
	m->location.x = loc.x;
	m->location.y = loc.y;

	switch(type){
	case MENU_YES_NO:
		m->location.x = 250;
		m->location.y = 100;
		m->numItems = 2;
		m->itemsPerRow = 1;
		itemRect.x = m->location.x;
		itemRect.y = m->location.y;
		itemRect.w = 30;
		itemRect.h = 80;
		m->location = itemRect;
		yes = new Textbox;
		no = new Textbox;
		LoadTextbox(yes,1,3,NULL,itemRect);
		m->items[0] = LoadMenuItem(itemRect,NULL,yes,"Yes");
		itemRect.y += 14;
		LoadTextbox(no,1,3,NULL,itemRect);
		m->items[1] = LoadMenuItem(itemRect,NULL,no,"No");
		break;
	case MENU_DEBUG:
		m->location.x = 180;
		m->location.y = 100;
		m->numItems = 12;
		m->itemsPerRow = 4;
		itemRect.x = m->location.x;
		itemRect.y = m->location.y;
		itemRect.w = 30;
		itemRect.h = 80;
		m->location = itemRect;
		for(int i = 0; i < 4; i++){
			for(int j = 0; j < 4; j++){
				yes = new Textbox;
				itemRect.x = m->location.x + j*30;
				itemRect.y = m->location.y + i*15;
				LoadTextbox(yes,1,3,NULL,itemRect);
				m->items[j + (4*i)] = LoadMenuItem(itemRect,NULL,yes,"Yes");
			}
		}
		m->cursor.location = 0;
		break;
	default:
		break;
	}
	return m;
}

void FreeMenu(Menu *m){
	for(int i = 0; i < m->numItems; i++){
		FreeMenuItem(m->items[i]);
	}
	delete(m);
}

void UpdateMenu(Menu *m){
	if((_Inputs->input & PPINPUT_A)&&!(_Inputs->prev->input & PPINPUT_A)){
		m->items[m->cursor.location]->action();
	}
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
	currentMenu = m;
	return m;
}

void CancelMenu(){
	_InMenu = false;
	FreeMenu(currentMenu);
	currentMenu = NULL;
}