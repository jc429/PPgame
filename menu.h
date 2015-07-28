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
	SDL_Rect bounds;
	Sprite *bgsprite;
	Textbox *tbox;

	bool highlighted;
	bool selected;
	void (*action)();
}MenuItem;

typedef struct MenuCursor_T{
	int location;		//what item is the cursor currently selecting?
	SDL_Rect size;

	bool active;

	Animation *anim[4];
	Animation *anim_active[4];
	Animation *anim_inactive[4];
}MenuCursor;

typedef struct Menu_T{
	bool used;

	bool active;

	Sprite *spr;

	MenuType type;
	SDL_Rect location;

	int numItems;				//number of items in the menu list
	int itemsPerRow;			//how many menu items are in each row? 
	MenuItem *items[MAX_MENU_ITEMS];

	MenuCursor cursor;			
}Menu;



MenuItem *LoadMenuItem(SDL_Rect box, Sprite *spr, Textbox *t, char* msg = NULL);
void FreeMenuItem(MenuItem *m);
void SetMenuItemAction(MenuItem *m, void (*func)(), char* name = NULL);
Menu *LoadMenu(MenuType type, Vec2i *loc = NULL);
void FreeMenu(Menu *m);
void UpdateMenu(Menu *m);
void DrawMenu(Menu *m);
void DrawMenuSprite(Menu *m);
void IncrementCursor(Menu *m);
void DecrementCursor(Menu *m);


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