
#include "dialogue.h"
#include "graphics.h"
#include "gamestate.h"

extern Camera uiCamera;
extern InputNode *_Inputs;
Menu *_CurrentMenu;
//vector<struct Menu_T*> _OverworldMenuStack;
//vector<struct Menu_T*> *curMenuStack = NULL;
//extern vector<struct Menu_T*> _CombatMenuStack;


MenuItem *AddMenuItem(Menu *m, char *text, void (*action)(int val)){
	SDL_Rect itemRect;
	SetRect(itemRect,0,0,30,15);
	if(m->numItems > 0)
		itemRect.y = m->items.back()->bounds.y + 10;	//10 is default spacing 
	if(!m->scrolling)
		m->location.h += 10;

	Textbox *tbox = new Textbox();
	LoadTextbox(tbox,1,itemRect,3);
	MenuItem *mi = LoadMenuItem(itemRect,NULL,tbox,text);
	if(action != NULL)
		SetMenuItemAction(mi,action,text);
	else
		SetMenuItemAction(mi,CancelMenu);
	
	m->items.push_back(mi);
	m->numItems++;
	return mi;
}

MenuItem *LoadMenuItem(SDL_Rect box, Sprite *spr, Textbox *t, char* text){
	MenuItem *mi = new MenuItem;
	mi->bounds.x = box.x;
	mi->bounds.y = box.y;
	mi->bounds.w = box.w;
	mi->bounds.h = box.h;
	mi->bgsprite = spr;
	mi->tbox = t;
	mi->tbox->box = mi->bounds;
/*	m->tbox->box.y -= 6;*/
	mi->tbox->SetText(text,0);

	return mi;
}

void FreeMenuItem(MenuItem *mi){
	FreeSprite(mi->bgsprite);
//	delete(m->text);
//	delete(m);
}

void SetMenuItemAction(MenuItem *mi, void (*func)(int val), char* text){
	mi->action = func;
	if(text){
		mi->tbox->SetText(text,0);
	}
}

Menu *LoadMenu(MenuType mtype, int numitems,Vec2i *loc, CursorType ct, AnchorPoint a, int padding){	
	//Loads a menu, with special conditions based on type

	SDL_Rect itemRect;
	Textbox *tbox;

	Menu *m = new Menu();

	m->type = mtype;
	
	m->item_offset_y = 0;
	m->parent = NULL;
	m->active = false;
	m->cursor.location = 0;
	if(loc != NULL){
		SetRect(m->location,*loc);
		m->location.w = 0;
		m->location.h = 5;
	}else
		SetRect(m->location,0,0,0,5);

	m->anchor = a;
	m->padding = padding;
	
	m->panel = LoadSprite(SPATH_PANEL_DEF,2,2,3);
	
	LoadCursor(m,ct);
	m->itemsPerRow = 1;
	m->numItems = numitems;

	m->scrolling = false;
	SetRect(itemRect,0,0,30,15);
	//m->location = itemRect;
	m->item_offset_y = 0;
	switch(mtype){
	case MENU_SCROLLING:
		//if scrolling, numitems describes the number of items displayed on screen at one time 
		m->numItems = 0;
		m->location.h += 10*(numitems-1);

		m->scrolling = true;
		m->scrollspeed = 1;
		m->cursor_buf_top = 0;
		m->cursor_buf_bottom = 0;
		break;

	case MENU_YES_NO:
		if(loc == NULL){
			SetRect(m->location,125,58);
		}
		m->numItems = 2;
		m->itemsPerRow = 1;

		SetRect(itemRect,0,0,30,15);
		m->location = itemRect;

		tbox = new Textbox();
		LoadTextbox(tbox,1,itemRect,3);
		m->items.push_back(LoadMenuItem(itemRect,NULL,tbox,"Yes"));

		itemRect.y += 10;
		m->location.h += 10;

		tbox = new Textbox();
		LoadTextbox(tbox,1,itemRect);
		m->items.push_back(LoadMenuItem(itemRect,NULL,tbox,"No"));
		return m;
		break;
	
	default:
		break;
	}

	for(int i = 0; i < m->numItems; i++){
		tbox = new Textbox();
		LoadTextbox(tbox,1,itemRect,3);
		m->items.push_back(LoadMenuItem(itemRect,NULL,tbox));
		if(i + 1 < m->numItems){
			itemRect.y += 10;
			m->location.h += 10;
		}
	}
	return m;
}

void LoadCursor(Menu *m, CursorType ct){
	Sprite *cursorSpr = NULL;
	int cframes;
	m->cursor.type = ct;
	
	switch(ct){
	case CURSTYPE_ARROW_LEFT:
	case CURSTYPE_ARROW_TOP:
		cframes = 2; //num frames in cursor animation
		cursorSpr = LoadSprite("sprites/menuarrow.png",8,8,2,4,4);
		for(int i = 0; i < 4; i++){
			m->cursor.anim_active[i] = LoadAnimation(cursorSpr,i*cframes,i*cframes,2,1,1,18);
			m->cursor.anim_inactive[i] = LoadAnimation(cursorSpr,i*cframes,i*cframes,1,1,1,18); //should be different eventually
			m->cursor.anim[i] = m->cursor.anim_active[i];
		}
		break;
	case CURSTYPE_RECT:
	default:
		cframes = 2; //num frames in cursor animation
		cursorSpr = LoadSprite("sprites/menucursor.png",8,8,4);
		for(int i = 0; i < 4; i++){
			m->cursor.anim_active[i] = LoadAnimation(cursorSpr,i*cframes,i*cframes,2,1,1,18);
			m->cursor.anim_inactive[i] = LoadAnimation(cursorSpr,i*cframes,i*cframes,1,1,1,18); //should be different eventually
			m->cursor.anim[i] = m->cursor.anim_active[i];
		}
		break;
	}
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
		if(m->items.at(m->cursor.location)->action != NULL)
			m->items.at(m->cursor.location)->action(m->cursor.location);
	}
}

void IncrementCursor(Menu *m){
	m->cursor.location = (m->cursor.location+1)%m->numItems;
}

void DecrementCursor(Menu *m){
	m->cursor.location = (m->cursor.location+(m->numItems-1))%m->numItems;
}

void SetAnchor(Menu *m, AnchorPoint a, int padding){
	m->anchor = a;
	m->padding = padding;
}

Vec2i GetAnchorLocation(AnchorPoint a){
//	extern Textbox mainTextbox;
	Vec2i loc;
	switch(a){
	case ANCHOR_NONE_NONE:
		SetVec2i(loc,0,0);
		break;
	case ANCHOR_TOP_LEFT:
		SetVec2i(loc,0,0);
		break;
	case ANCHOR_TOP_CENTER:
		SetVec2i(loc,0.5*GAME_RES_X,0);
		break;
	case ANCHOR_TOP_RIGHT:
		SetVec2i(loc,GAME_RES_X,0);
		break;

	case ANCHOR_CENTER_LEFT:
		SetVec2i(loc,0,0.5*GAME_RES_Y);
		break;
	case ANCHOR_CENTER_CENTER:
		SetVec2i(loc,0.5*GAME_RES_X,0.5*GAME_RES_Y);
		break;
	case ANCHOR_CENTER_RIGHT:
		SetVec2i(loc,GAME_RES_X,0.5*GAME_RES_Y);
		break;

	case ANCHOR_BOTTOM_LEFT:
		SetVec2i(loc,0,GAME_RES_Y);
		break;
	case ANCHOR_BOTTOM_CENTER:
		SetVec2i(loc,0.5*GAME_RES_X,GAME_RES_Y);
		break;
	case ANCHOR_BOTTOM_RIGHT:
		SetVec2i(loc,GAME_RES_X,GAME_RES_Y);
		break;

	case ANCHOR_TEXTBOX_LEFT:
		SetVec2i(loc,0,GAME_RES_Y - TEXTAREA_H);
		break;
	case ANCHOR_TEXTBOX_CENTER:
		SetVec2i(loc,0.5*GAME_RES_X,GAME_RES_Y - TEXTAREA_H);
		break;
	case ANCHOR_TEXTBOX_RIGHT:
		SetVec2i(loc,GAME_RES_X,GAME_RES_Y - TEXTAREA_H);
		break;
	}
	return loc;
}
SDL_Rect AnchorRect(SDL_Rect src, AnchorPoint a, int padding){
	//takes a given rect and "docks" it to a location
	Vec2i anchorbase = GetAnchorLocation(a);

	switch(a){		//x positioning
	case ANCHOR_TOP_LEFT:
	case ANCHOR_CENTER_LEFT:
	case ANCHOR_BOTTOM_LEFT:
	case ANCHOR_TEXTBOX_LEFT:
		src.x = anchorbase.x + padding;
		break;
	case ANCHOR_TOP_CENTER:
	case ANCHOR_CENTER_CENTER:
	case ANCHOR_BOTTOM_CENTER:
	case ANCHOR_TEXTBOX_CENTER:
		src.x = anchorbase.x - (0.5*src.w);
		break;
	case ANCHOR_TOP_RIGHT:
	case ANCHOR_CENTER_RIGHT:
	case ANCHOR_BOTTOM_RIGHT:
	case ANCHOR_TEXTBOX_RIGHT:
		src.x = anchorbase.x - (src.w + padding);
		break;
	}
	switch(a){		//y positioning
	case ANCHOR_TOP_LEFT:
	case ANCHOR_TOP_CENTER:
	case ANCHOR_TOP_RIGHT:
		src.y = anchorbase.y + padding;
		break;
	case ANCHOR_CENTER_LEFT:
	case ANCHOR_CENTER_CENTER:
	case ANCHOR_CENTER_RIGHT:
		src.y = anchorbase.y + (0.5*src.h);
		break;
	case ANCHOR_BOTTOM_LEFT:
	case ANCHOR_BOTTOM_CENTER:
	case ANCHOR_BOTTOM_RIGHT:
	case ANCHOR_TEXTBOX_LEFT:
	case ANCHOR_TEXTBOX_CENTER:
	case ANCHOR_TEXTBOX_RIGHT:
		src.y = anchorbase.y - (src.h + padding);
		break;
	}
	return src;
}

void ClearMenus(){

}


void Menu::Draw(){
	if(this->scrolling)
		DrawScrollingMenu(this);
	else
		DrawMenu(this);
};

void DrawMenu(Menu *m){
	if(m==NULL) return;
	Vec2i loc;
	loc.x = m->location.x;
	loc.y = m->location.y;
	DrawMenuPanel(m);

/*	m->location.x += RandomIntInclusive(-1,1);
	m->location.y += RandomIntInclusive(-1,1);*/

	for(int i = 0; i < m->numItems; i++){
		loc.x = m->location.x + m->items.at(i)->bounds.x;
		loc.y = m->location.y + m->items.at(i)->bounds.y;
		DrawSprite(m->items.at(i)->bgsprite,0,loc,&uiCamera);

		
		m->items.at(i)->tbox->box.x = loc.x;
		m->items.at(i)->tbox->box.y = loc.y;
		m->items.at(i)->tbox->Draw(0,-1*m->item_offset_y);
//		DrawRect(m->items[i]->tbox->box,&uiCamera);
	}
	DrawMenuCursor(m);
	
}

void DrawScrollingMenu(Menu *m){
	if(m==NULL) return;
	Vec2i loc;
	loc.x = m->location.x;
	loc.y = m->location.y;
	DrawMenuPanel(m);

	//first make sure the cursor is still on screen
	if((m->items.at(m->cursor.location)->bounds.y + m->items.at(m->cursor.location)->bounds.h) 
		> (m->location.h + m->item_offset_y)){
			m->item_offset_y++;
		//	printf("%i \n",m->item_offset_y);
	}
	if(m->items.at(m->cursor.location)->bounds.y < (m->item_offset_y)){
			m->item_offset_y--;
		//	printf("%i \n",m->item_offset_y);
	}
	
	for(int i = 0; i < (int)m->items.size(); i++){
		if(((m->items.at(i)->bounds.y + m->items.at(i)->bounds.h) <= (m->location.h + m->item_offset_y))
			&&(m->items.at(i)->bounds.y >= (m->item_offset_y))){
			loc.x = m->location.x + m->items.at(i)->bounds.x;
			loc.y = m->location.y + m->items.at(i)->bounds.y - m->item_offset_y;
			DrawSprite(m->items.at(i)->bgsprite,0,loc,&uiCamera);
			m->items.at(i)->tbox->box.x = loc.x;
			m->items.at(i)->tbox->box.y = loc.y;
			m->items.at(i)->tbox->Draw(0,0);
		}

	/*	
		m->items.at(i)->tbox->box.x = loc.x;
		m->items.at(i)->tbox->box.y = loc.y;*/
//		DrawRect(m->items.at(i)->bounds,&uiCamera);
	}

	DrawScrollingMenuCursor(m);
}


void DrawMenuPanel(Menu *m){
	SDL_Rect rect = m->location;
//	Vec2i loc = m->GetPosition();
	DrawPanel(m->location,m->panel);
}

void DrawMenuCursor(Menu *m){
	Vec2i loc;
	switch(m->cursor.type){
	case CURSTYPE_ARROW_LEFT:
		loc.x = m->location.x + m->items.at(m->cursor.location)->bounds.x - 6; 
		loc.y = m->location.y + m->items.at(m->cursor.location)->bounds.y + m->items.at(m->cursor.location)->bounds.h*0.5;
		DrawAnimation(m->cursor.anim[DIR_RIGHT],loc,&uiCamera);
		break;
	case CURSTYPE_RECT:
	default:
		loc.x = m->location.x + m->items.at(m->cursor.location)->bounds.x; 
		loc.y = m->location.y + m->items.at(m->cursor.location)->bounds.y; 
		DrawAnimation(m->cursor.anim[0],loc,&uiCamera);		//upper left corner
		loc.x += m->items.at(m->cursor.location)->bounds.w - m->cursor.anim[0]->sprite->w;
		DrawAnimation(m->cursor.anim[1],loc,&uiCamera);		//upper right corner
		loc.x = m->location.x + m->items.at(m->cursor.location)->bounds.x;
		loc.y += m->items.at(m->cursor.location)->bounds.h - m->cursor.anim[0]->sprite->h;
		DrawAnimation(m->cursor.anim[2],loc,&uiCamera);		//lower left corner
		loc.x += m->items.at(m->cursor.location)->bounds.w - m->cursor.anim[0]->sprite->w;	
		DrawAnimation(m->cursor.anim[3],loc,&uiCamera);		//lower right corner
		break;
	}
}
void DrawScrollingMenuCursor(Menu *m){
	Vec2i loc;
	switch(m->cursor.type){
	case CURSTYPE_ARROW_LEFT:
		loc.x = m->location.x + m->items.at(m->cursor.location)->bounds.x - 6; 
		loc.y = m->location.y - m->item_offset_y + m->items.at(m->cursor.location)->bounds.y + m->items.at(m->cursor.location)->bounds.h*0.5;
		DrawAnimation(m->cursor.anim[DIR_RIGHT],loc,&uiCamera);
		break;
	case CURSTYPE_RECT:
	default:
		loc.x = m->location.x + m->items.at(m->cursor.location)->bounds.x; 
		loc.y = m->location.y + m->items.at(m->cursor.location)->bounds.y - m->item_offset_y; 
		DrawAnimation(m->cursor.anim[0],loc,&uiCamera);		//upper left corner
		loc.x += m->items.at(m->cursor.location)->bounds.w - m->cursor.anim[0]->sprite->w;
		DrawAnimation(m->cursor.anim[1],loc,&uiCamera);		//upper right corner
		loc.x = m->location.x + m->items.at(m->cursor.location)->bounds.x;
		loc.y += m->items.at(m->cursor.location)->bounds.h - m->cursor.anim[0]->sprite->h;
		DrawAnimation(m->cursor.anim[2],loc,&uiCamera);		//lower left corner
		loc.x += m->items.at(m->cursor.location)->bounds.w - m->cursor.anim[0]->sprite->w;	
		DrawAnimation(m->cursor.anim[3],loc,&uiCamera);		//lower right corner
		break;
	}
}


Menu *LoadMenuYesNo(Vec2i *loc, char *yes, char *no, void (*YesFunc)(int val),void (*NoFunc)(int val)){
	//_InMenu = true;
	Menu *m = LoadMenu(MENU_YES_NO,2,loc);
	SetMenuItemAction(m->items.at(0),SelectAnswer,yes);
	SetMenuItemAction(m->items.at(1),SelectAnswer,no);
	if(*YesFunc != NULL)
		SetMenuItemAction(m->items.at(0),YesFunc);
	if(*NoFunc != NULL)
		SetMenuItemAction(m->items.at(1),NoFunc);
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
	SetVec2i(loc,92, 72 - (numItems * 10));

	m = LoadMenu(MENU_CUSTOM,numItems,&loc);
	
	SetRect(m->location,loc.x,loc.y,60,16);
	SetRect(itemRect,0,0,60,16);
	

	for(int i = 0; i < 8; i++)
		textboxes[i] = NULL;
	for(int i = 0; i < numItems; i++){
		textboxes[i] = new Textbox();
		LoadTextbox(textboxes[i],1,itemRect,3);

		if(itemNames != NULL)
			m->items.at(i) = LoadMenuItem(itemRect,NULL,textboxes[i],itemNames[i]);
		else
			m->items.at(i) = LoadMenuItem(itemRect,NULL,textboxes[i]);

		if(i + 1 < numItems){
			itemRect.y += 10;
			m->location.h += 10;
		}
	}
	m->numItems = numItems;
	m->itemsPerRow = 1;
	
	if(itemNames != NULL){
		for(int i = 0; i < numItems; i++){
			SetMenuItemAction(m->items.at(i),SelectAnswer,itemNames[i]);
		}
	}
	else{
		for(int i = 0; i < numItems; i++){
			SetMenuItemAction(m->items.at(i),SelectAnswer);
		}
	}
	return m;
}

void OpenMenu(Menu *m,vector<Menu*> *stack){
	int maxwidth = 0;	//the width of the widest item in the menu
	for(int i = 0; i < m->numItems; i++){
		m->items.at(i)->bounds.w = (2*m->items.at(i)->tbox->buf) + GetStringWidth(m->items.at(i)->tbox->text);
		maxwidth = Max(maxwidth,m->items.at(i)->bounds.w);
	}
	m->location.w = maxwidth;
	m->location = AnchorRect(m->location,m->anchor,m->padding);


		//FIXME: add buffer amounts or something idk
//	printf("%i\n",m->location.w);

	m->active = true;
	m->cursor.location = 0;
	m->item_offset_y = 0;
	if(stack != NULL){
		stack->push_back(m);
	}else
		CurrentScene()->MenuStack.push_back(m);
		
}



void CancelMenu(int val){

	CurrentScene()->MenuStack.back()->active = false;

	CurrentScene()->MenuStack.pop_back();
}

void AdvanceAndCancel(int steps){
	AdvanceText(steps);
	CancelMenu();
}

void SelectAnswer(int answer){
	//maybe fill this out better
	AdvanceAndCancel(answer);
}

void SelectAnswer1(int val){
	AdvanceAndCancel(0);
}
void SelectAnswer2(int val){
	AdvanceAndCancel(1);
}
void SelectAnswer3(int val){
	AdvanceAndCancel(2);
}
void SelectAnswer4(int val){
	AdvanceAndCancel(3);
}
void SelectAnswer5(int val){
	AdvanceAndCancel(4);
}
void SelectAnswer6(int val){
	AdvanceAndCancel(5);
}