#ifndef _INVENTORY_
#define _INVENTORY_

#include "item.h"



void InitInventory();

void AddItem(int itemID, int amount = 1);
void BuyItem(int itemID, int cost, int amount = 1);

void UpdateInvCursor();
void UpdateInvText();

void DrawInvBG();
void DrawInventory();
void DrawItemStats();
void DrawInvCursor();

void DropSelectedItem();
void DropItem(ItemInstance &item);
void UseSelectedItem();
void UseItem(ItemInstance &item);
void EquipSelectedItem();



void LoadWallet();
void UpdateWallet();
void DrawWallet();
void AddMoney(int amount);
void SubtractMoney(int amount);
void SetMoney(int amount);
int GetMoney();
void ShowWallet();
void HideWallet();

#endif