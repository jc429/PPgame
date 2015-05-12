#include "item.h"
#include <glib.h>

//static GHashTable *_ItemTable; 

static ItemData _ItemTable[256]; //get rid of this
static int numItems = 0;

void InitItemTable(){
//	_ItemTable = g_hash_table_new(NULL,g_int_equal);
	//GLib doesn't wanna work :(

	LoadItemCFG("testfiles/itemdef.json");
}

void LoadItemData(int ID, char *name, char *desc, int icon, int type, Uint8 attributes, int value){
	ItemData data;
	data.itemID = ID;
	copy_string(data.name,name);
	copy_string(data.flavortext,desc);
	data.icon = icon;
	data.type = (ItemType)type;
	data.attributes = attributes;
	data.sellvalue = value;
	//add the itemdata to the table
	_ItemTable[numItems] = data;	// using a vector for now...
	numItems++;
	//but there is no table rn....
}

ItemData *FindItemData(int itemID){
	for(int i = 0; i < numItems; i++){
		if(_ItemTable[i].itemID == itemID)
			return &_ItemTable[i];
	}
	return NULL;
}