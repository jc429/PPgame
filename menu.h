#ifndef _MENU_
#define _MENU_

#include "ptext.h"
#include "ui.h"
#include <vector>
using  std::vector;



typedef enum MenuType{
	MENU_EMPTY = 0,		//a blank menu. probably never used.

	MENU_YES_NO,		//a simple yes/no menu. occurs frequently enough that it probably deserves its own "express" build.
	
	MENU_CUSTOM,		//the most common menu type (probably), not much is preset on one of these

	MENU_SCROLLING,		//scrolls through a large number of items rather than trying to cram them all in the panel at once

	MENU_DEBUG = -1		//debug menus will only exist in debug mode? maybe? idk
};

typedef struct MenuItem_T{
	SDL_Rect bounds;		//a menuitem is always a member of a menu, so these coordinates are relative
	Sprite *bgsprite;
	Textbox *tbox;

	bool highlighted;
	bool selected;
	void (*action)(int val);
}MenuItem;

enum CursorType{
	CURSTYPE_RECT,			//a rectangular cursor that wraps around the selected item
	CURSTYPE_ARROW_LEFT,	//an arrow that points at the selected item from the left
	CURSTYPE_ARROW_TOP		//an arrow that rests on the top of the selected item
};

enum Directions{
	DIR_UP = 0,
	DIR_DOWN = 1,
	DIR_LEFT = 2,
	DIR_RIGHT = 3
};

enum AnchorPoint{
	ANCHOR_NONE_NONE = 0,
	ANCHOR_TOP_LEFT,
	ANCHOR_TOP_CENTER,
	ANCHOR_TOP_RIGHT,
	ANCHOR_CENTER_LEFT,
	ANCHOR_CENTER_CENTER,
	ANCHOR_CENTER_RIGHT,
	ANCHOR_BOTTOM_LEFT,
	ANCHOR_BOTTOM_CENTER,
	ANCHOR_BOTTOM_RIGHT,
	ANCHOR_TEXTBOX_LEFT,
	ANCHOR_TEXTBOX_CENTER,
	ANCHOR_TEXTBOX_RIGHT,
};

typedef struct MenuCursor_T{
	int location;		//what item is the cursor currently selecting?
	SDL_Rect size;

	bool active;
	CursorType type;

	Animation *anim[4];
	Animation *anim_active[4];
	Animation *anim_inactive[4];
}MenuCursor;

typedef struct Menu:public Panel{
	/******scrolling menu things******/
	bool scrolling;
	int item_offset_y;			
	int scrollspeed;
	int cursor_buf_top;		//how far from the top the cursor should rest when scrolling up (in pixels)
	int cursor_buf_bottom;	//how far from the bottom the cursor should rest when scrolling down (in pixels)
	/*********************************/


	bool used;

	bool active;



	MenuType type;
	SDL_Rect location;
	AnchorPoint anchor;			//which side of the screen to anchor the menu to
	int padding;				//how far away from the anchor point we want the menu to be

	int numItems;				//number of items in the menu list
	int itemsPerRow;			//how many menu items are in each row? 
	//MenuItem *items[MAX_MENU_ITEMS];
	vector<MenuItem *> items;
	
	MenuCursor cursor;	

	Menu(){};
	void Draw();
}Menu;


MenuItem *AddMenuItem(Menu *m,  char* text = NULL, void (*action)(int val) = NULL);
MenuItem *LoadMenuItem(SDL_Rect box, Sprite *spr, Textbox *t, char* text = NULL);
void FreeMenuItem(MenuItem *mi);
void SetMenuItemAction(MenuItem *mi, void (*func)(int val), char* text = NULL);

Menu *LoadMenu(MenuType type, int numitems = 0, Vec2i *loc = NULL, 
	CursorType ct = CURSTYPE_ARROW_LEFT, AnchorPoint a = ANCHOR_NONE_NONE, int padding = 0);
void LoadCursor(Menu *m, CursorType ct);
void FreeMenu(Menu *m);
void UpdateMenu(Menu *m);
void DrawMenu(Menu *m);
void DrawScrollingMenu(Menu *m);
void DrawMenuPanel(Menu *m);
void DrawMenuCursor(Menu *m);
void DrawScrollingMenuCursor(Menu *m);
void IncrementCursor(Menu *m);
void DecrementCursor(Menu *m);
void SetAnchor(Menu *m, AnchorPoint a, int padding = 0);
Vec2i GetAnchorLocation(AnchorPoint a);
SDL_Rect AnchorRect(SDL_Rect src, AnchorPoint a, int padding = 0);


Menu *LoadMenuYesNo(Vec2i *loc = NULL, char* yes = NULL, char* no = NULL, void (*YesFunc)(int val) = NULL,void (*NoFunc)(int val) = NULL);
Menu *LoadCustomMenu(int numItems, char itemNames[6][16] = NULL);


void OpenMenu(Menu *m,vector<Menu*> *stack = NULL);

void CancelMenu(int val = 0);
void AdvanceText(int steps = 0);
void AdvanceAndCancel(int steps = 0);
void SelectAnswer(int answer);

static bool _InMenu;

void LoadPauseMenu();
void OpenPauseMenu(int val = 0);
void OpenStatus(int val);
void OpenEncyclopedia(int val);
void OpenInventory(int val);
void OpenOptions(int val);
void OpenSaveLoad(int val);

#endif