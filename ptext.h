#ifndef _PTEXT_
#define _PTEXT_

#include "global.h"
#include "sprites.h"

#include <vector>
using  std::vector;

typedef struct Textbox_T{
	Sprite *spr;
	int frame;
	SDL_Rect box; 

	struct Textbox_T *speakerbox;
	
	Animation *arrow;				//little arrow at the bottom that signifies there's more text
	Vec2i arrowpos;					//position of arrow, relative to textbox
	bool usesArrow;


	int linect;						//the number of lines this specific textbox holds
	int linelength;					//the number of chars per line this textbox holds (default 40)
	char *lines[LINE_COUNT];		//The textbox can hold a max of 4 lines of 40 characters each
	int cursor;			//For drawing text one character at a time - set to -1 to draw all text 
	bool donewriting;
	struct Message_T *msg;
}Textbox;

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
//	SDL_Rect FIXME;		
/* I have no idea why, but something about calling SetText inside of LoadMenuItem overwrites the first member of a MenuItem
	This FIXME rect is here for now to create garbage data to be overwritten while I look into why it's happening.
*/
	SDL_Rect bounds;
	Sprite *bgsprite;
	Textbox *text;

	bool highlighted;
	bool selected;
	void (*action)();
}MenuItem;

typedef struct MenuCursor_T{
	int location;		//what item is the cursor currently selecting?
	Animation *anim;
	Animation *anim_active;
	Animation *anim_inactive;
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
Menu *LoadCustomMenu(int numItems, char *op1 = NULL, char *op2 = NULL, char *op3 = NULL, char *op4 = NULL, char *op5 = NULL, char *op6 = NULL);


void OpenMenu(Menu *m);

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