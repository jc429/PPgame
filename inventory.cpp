#include "inventory.h"
#include "graphics.h"
#include "gamestate.h"


extern Camera uiCamera;

#define MAX_MONEY 9999999

static Sprite *_ItemIcons;
static Sprite *_PanelSpr;

#define INV_SIZE 64
#define INV_ROW_LEN 8
#define INV_COL_LEN 8

ItemInstance Inventory[INV_SIZE];

Textbox ItemNameBox;
Textbox ItemDescBox;

Menu *itemPrompt;
vector<struct Menu_T*> _InvMenuStack;
//int max_items = 99;

static struct Wallet_T{
	SDL_Rect location;
	Sprite *spr;
	Textbox textbox;
	int money;

	bool visible;
}Wallet;

static struct InvCursor_T{
	int selection;
	Vec2i location;
	Animation *anim;
}InvCursor;


void MoveInvCursor();
void LoadItemPrompt(Uint8 attributes = 0);


void InventoryPage::Update(){
	if(_InvMenuStack.empty())
		UpdateInvCursor();
	else
		UpdateMenu(_InvMenuStack.back());
	if(InputPressed(PPINPUT_B)){
		if(_InvMenuStack.empty())
			CloseInventory();
		else
			CancelMenu();
	}
	if(DEBUG_INFINITE_MONEY){
		if(GetMoney()+10 < 999999)
			AddMoney(10);
	}
}

void InventoryPage::Draw(){
	DrawInvBG();
	DrawInventory();
	DrawItemStats();
	DrawInvCursor();
	if(!_InvMenuStack.empty()){
		for(int i = 0; i < (int)_InvMenuStack.size(); i++){
			DrawMenu(_InvMenuStack[i]);
		}
	}
}

void InitInventory(){
	_ItemIcons = LoadSprite("sprites/itemicons.png",16,16,16);
	_PanelSpr = LoadSprite(SPATH_PANEL_DEF,4,4,3);
	SetVec2i(InvCursor.location,0,0);
	InvCursor.selection = 0;
	InvCursor.anim = LoadAnimation(LoadSprite("sprites/invcursor.png",20,20,1),0,0,2,1,1,18);
	for(int i = 0; i < INV_SIZE; i++){
		Inventory[i].itemID = 0;
		Inventory[i].quantity = 0;
		Inventory[i].properties = NULL;
	}
	SDL_Rect tboxloc;
	SetRect(tboxloc,200,26,120,18);
	LoadTextbox(&ItemNameBox,1,15,NULL,tboxloc);
	tboxloc.y += 22;
	tboxloc.h = 88;
	LoadTextbox(&ItemDescBox,2,15,NULL,tboxloc);
	
	LoadItemPrompt();

	
}

void LoadItemPrompt(Uint8 attributes){
	return; //REMOVE ME

	char itemPromptNames[6][16];
	int num = 2;	//min item prompt actions is 2 (move and cancel)
	int i = 0;
	if(attributes & ITEM_USABLE)
		++num;
	if(attributes & ITEM_EQUIPPABLE)
		++num;
	if(attributes & ITEM_DROPPABLE)
		++num;

	itemPrompt = LoadCustomMenu(num,itemPromptNames);
	if(attributes & ITEM_USABLE){
		SetMenuItemAction(itemPrompt->items[i],UseSelectedItem,"Use       ");
		++i;
	}
	if(attributes & ITEM_EQUIPPABLE){
		SetMenuItemAction(itemPrompt->items[i],EquipSelectedItem,"Equip     ");
		++i;
	}
	if(attributes & ITEM_DROPPABLE){
		SetMenuItemAction(itemPrompt->items[i],DropSelectedItem,"Drop       ");
		++i;
	}
		
	SetMenuItemAction(itemPrompt->items[i],CancelMenu,"Move      ");
	++i;
	SetMenuItemAction(itemPrompt->items[i],CancelMenu,"Cancel    ");

}

void AddItem(int itemID, int amount){
	//currently this just obliterates the last item in the inventory if it's full, whoops
	//ItemInstance *inst = new ItemInstance;
	ItemData *properties = FindItemData(itemID);

	if(properties == NULL) {	//if the item doesn't exist in the database, abort mission
		return; 
	}

	if((properties->attributes & ITEM_STACKABLE)){	//if the item stacks
	//	printf("it stacks! \n");
		for(int i = 0; i < INV_SIZE; i++){	//check the inventory for the item first
			if(Inventory[i].properties == NULL) 
				continue;
			if(Inventory[i].itemID == itemID){
				//if it's already in our inventory, and it stacks, just increase the qty
				Inventory[i].quantity += amount;
				return;
			}
		}
	}
	int i;
	for(i = 0; i < INV_SIZE; i++){
		if(Inventory[i].properties == NULL)
			break;
	}
	Inventory[i].itemID = itemID;
	Inventory[i].quantity = amount;
	Inventory[i].properties = properties;
}

void BuyItem(int itemID, int cost, int amount){
	SubtractMoney(cost);
	AddItem(itemID,amount);
}

void DropSelectedItem(){
	if(Inventory[InvCursor.selection].properties != NULL){
		DropItem(Inventory[InvCursor.selection]);
	}
	CancelMenu();
}

void DropItem(ItemInstance &item){
	if(item.quantity == 1){
		item.properties = NULL;
		item.itemID = 0;
		item.quantity = 0;
	}else{
		--item.quantity;
	}
}

void UseSelectedItem(){
	if(Inventory[InvCursor.selection].properties != NULL){
		UseItem(Inventory[InvCursor.selection]);
	}
}

void UseItem(ItemInstance &item){
	
}

void EquipSelectedItem(){

}


void UpdateInvCursor(){
	MoveInvCursor();
	if(Inventory[InvCursor.selection].properties != NULL){
		if(InputPressed(PPINPUT_A)){
			LoadItemPrompt(Inventory[InvCursor.selection].properties->attributes);
			OpenMenu(itemPrompt,&_InvMenuStack);
		}
	}
}

void UpdateInvText(){
	if(Inventory[InvCursor.selection].properties != NULL){
		SetText(Inventory[InvCursor.selection].properties->name,&ItemNameBox,0);
		SetText(Inventory[InvCursor.selection].properties->flavortext,&ItemDescBox,0);
	}else{
		SetText("------",&ItemNameBox,0);
		SetText("------",&ItemDescBox,0);
	}
}

void MoveInvCursor(){
	int temp = InvCursor.selection;	//save the current selection
	if(InputPressed(PPINPUT_LEFT)&&!InputPressed(PPINPUT_RIGHT)){
		InvCursor.selection -= 1;
	}
	if(InputPressed(PPINPUT_RIGHT)&&!InputPressed(PPINPUT_LEFT)){
		InvCursor.selection += 1;
	}
	if(InputPressed(PPINPUT_UP)&&!InputPressed(PPINPUT_DOWN)){
		InvCursor.selection -= INV_ROW_LEN;
	}
	if(InputPressed(PPINPUT_DOWN)&&!InputPressed(PPINPUT_UP)){
		InvCursor.selection += INV_ROW_LEN;
	}
	while(InvCursor.selection < 0)
		InvCursor.selection += INV_SIZE;
	InvCursor.selection %= INV_SIZE;
	InvCursor.location.x = InvCursor.selection % INV_ROW_LEN;
	InvCursor.location.y = InvCursor.selection / INV_ROW_LEN;

	if(InvCursor.selection != temp){	//if the selection changed this frame
		UpdateInvText();
	}
}

void DrawInvBG(){
	SDL_Rect rect;
	SetRect(rect,14,14,(INV_ROW_LEN) * 20,(INV_COL_LEN) * 20);
	DrawPanel(rect,_PanelSpr);
}

void DrawInventory(){
	Vec2i loc;

	for(int i = 0; i < 64; i++){
		SetVec2i(loc,16 + ((i%INV_ROW_LEN) * 20) , 16 + ((i/INV_ROW_LEN) * 20));
		if(Inventory[i].properties == NULL) continue;
		DrawSprite(_ItemIcons,Inventory[i].properties->icon/*i*/,loc,&uiCamera);
	}
}

void DrawItemStats(){
	SDL_Rect rect;
	SetRect(rect,200,30,112,144);
	DrawPanel(rect,_PanelSpr);
	
	DrawTextbox(&ItemNameBox);
	DrawTextbox(&ItemDescBox);
	//DrawTextbox(&Wallet.textbox,6,-6);
	/* draw the info on the items*/
}

void DrawInvCursor(){
	Vec2i loc;
	SetVec2i(loc,14 + ((InvCursor.location.x) * 20) , 14 + ((InvCursor.location.y) * 20)); 
	DrawAnimation(InvCursor.anim,loc,&uiCamera);
}





/***			 Wallet Stuff				*******************************/

void LoadWallet(){
	Wallet.spr = LoadSprite(SPATH_PANEL_DEF,4,4,3);
	SetRect(Wallet.location,248,4,64,16);
	LoadTextbox(&Wallet.textbox,1,8,NULL,Wallet.location);
	SetText("$000000",&Wallet.textbox,0);
}

void UpdateWallet(){
	char moneystr[8];	//money amount to print 
	
	//printing stuff
	sprintf(moneystr,"$%i",Wallet.money);
	SetText(moneystr,&Wallet.textbox,0);
}

void DrawWallet(){
	if(!Wallet.visible) return;
	DrawPanel(Wallet.location,Wallet.spr);
	DrawTextbox(&Wallet.textbox,6,-6);
}

void AddMoney(int amount){
	Wallet.money += amount;
	if(Wallet.money > MAX_MONEY)
		Wallet.money = MAX_MONEY;
	UpdateWallet();
}

void SubtractMoney(int amount){
	Wallet.money -= amount;
	UpdateWallet();
} 

void SetMoney(int amount){
	Wallet.money = amount;
	UpdateWallet();
}

int GetMoney(){
	return Wallet.money;
}

void ShowWallet(){
	Wallet.visible = true;
}

void HideWallet(){
	Wallet.visible = false;
}