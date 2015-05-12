#ifndef _ITEM_
#define _ITEM_

#include "global.h"
#include "sprites.h"
#include "entity.h"


#define ITEM_USABLE		1<<5
#define ITEM_CONSUMABLE	1<<4
#define ITEM_EQUIPPABLE	1<<3
#define ITEM_DROPPABLE	1<<2
#define ITEM_SELLABLE	1<<1
#define ITEM_STACKABLE	1


typedef enum ItemType{
	IT_DEFAULT,
	IT_RESTORATION,
	IT_EQUIPMENT,
	IT_WEAPON,
	IT_KEY,
	IT_SPELL,
	IT_BOOST
};

typedef struct ItemData_T{
	int itemID;
	char name[32];
	char flavortext[255];
	int icon; //all item icons are stored in a single sprite sheet, this int represents what frame of the sheet the item uses

	ItemType type;
	Uint8 attributes;
	int sellvalue;

	void (*Effect)(struct Item *item,Entity *user, Entity *target);

}ItemData;

typedef struct ItemInstance_T{
	int itemID;
	ItemData *properties;

	int quantity;

}ItemInstance;

void InitItemTable();
void LoadItemCFG(char *path);
void LoadItemData(int ID, char *name, char *desc, int icon, int type, Uint8 attributes, int value = 0);

ItemData *FindItemData(int itemID);

#endif