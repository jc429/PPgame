#ifndef _MENU_
#define _MENU_

#include "ptext.h"
#include <vector>
using  std::vector;



typedef enum MenuType{
	MENU_EMPTY = 0,
	MENU_PAUSE = 1,
	MENU_BATTLE = 2,
	MENU_YES_NO = 3,
	
	MENU_CUSTOM_2,
	MENU_CUSTOM_3,
	MENU_CUSTOM_4,
	MENU_CUSTOM_5,
	MENU_CUSTOM_6,

	MENU_DEBUG = -1
};

typedef struct MenuItem_T{
	SDL_Rect bounds;		//a menuitem is always a member of a menu, so these coordinates are relative
	Sprite *bgsprite;
	Textbox *tbox;

	bool highlighted;
	bool selected;
	void (*action)();
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

typedef struct Menu_T{
	bool used;

	bool active;

	Sprite *panel;

	MenuType type;
	SDL_Rect location;
	AnchorPoint anchor;			//which side of the screen to anchor the menu to
	int padding;				//how far away from the anchor point we want the menu to be

	int numItems;				//number of items in the menu list
	int itemsPerRow;			//how many menu items are in each row? 
	MenuItem *items[MAX_MENU_ITEMS];

	MenuCursor cursor;			
}Menu;



MenuItem *LoadMenuItem(SDL_Rect box, Sprite *spr, Textbox *t, char* msg = NULL);
void FreeMenuItem(MenuItem *m);
void SetMenuItemAction(MenuItem *m, void (*func)(), char* name = NULL);
Menu *LoadMenu(MenuType type, Vec2i *loc = NULL, CursorType ct = CURSTYPE_ARROW_LEFT, AnchorPoint a = ANCHOR_NONE_NONE, int padding = 0);
void FreeMenu(Menu *m);
void UpdateMenu(Menu *m);
void DrawMenu(Menu *m);
void DrawMenuPanel(Menu *m);
void IncrementCursor(Menu *m);
void DecrementCursor(Menu *m);
void SetAnchor(Menu *m, AnchorPoint a, int padding = 0);
Vec2i GetAnchorLocation(AnchorPoint a);
SDL_Rect AnchorRect(SDL_Rect src, AnchorPoint a, int padding = 0);

Menu *LoadPauseMenu();
Menu *LoadMenuYesNo(Vec2i *loc = NULL, char* yes = NULL, char* no = NULL, void (*YesFunc)() = NULL,void (*NoFunc)() = NULL);
Menu *LoadCustomMenu(int numItems, char itemNames[6][16] = NULL);


void OpenMenu(Menu *m,vector<struct Menu_T*> *stack = NULL);

void CancelMenu();
void AdvanceText(int steps = 0);
void AdvanceAndCancel(int steps = 0);
void SelectAnswer1();
void SelectAnswer2();
void SelectAnswer3();
void SelectAnswer4();
void SelectAnswer5();
void SelectAnswer6();

static bool _InMenu;


#endif