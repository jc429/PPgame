#ifndef _PTEXT_
#define _PTEXT_

#include "settings.h"
#include "sprites.h"

typedef struct Textbox_T{
	Sprite *spr;
	int frame;
	SDL_Rect box; 

	int linect;						//the number of lines this specific textbox holds
	int linelength;					//the number of chars per line this textbox holds (default 40)
	char *lines[LINE_COUNT];		//The textbox can hold a max of 4 lines of 40 characters each
	int cursor;			//For drawing text one character at a time - set to -1 to draw all text 
	bool donewriting;
	struct Message_T *msg;
}Textbox;

typedef enum MenuType{
	MENU_YES_NO = 0,
	MENU_PAUSE = 1,
	MENU_BATTLE = 2,

	MENU_DEBUG = -1
};

typedef struct MenuItem_T{
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
}MenuCursor;

typedef struct Menu_T{
	bool used;

	bool active;

	MenuType type;
	SDL_Rect location;

	int numItems;				//number of items in the menu list
	int itemsPerRow;			//how many menu items are in each row? 
	MenuItem *items[MAX_MENU_ITEMS];

	MenuCursor cursor;			
}Menu;



MenuItem *LoadMenuItem(SDL_Rect box, Sprite *spr, Textbox *t, char* msg);
void FreeMenuItem(MenuItem *m);
void SetMenuItemAction(MenuItem *m, void (*func)() );
Menu *LoadMenu(MenuType type, Vec2i loc);
void FreeMenu(Menu *m);
void UpdateMenu(Menu *m);
void DrawMenu(Menu *m);
void IncrementCursor(Menu *m);
void DecrementCursor(Menu *m);



Menu *OpenMenuYesNo(Vec2i loc, void (*YesFunc)(),void (*NoFunc)());


void CancelMenu();


static bool _InMenu;

#endif