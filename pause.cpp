#include "menu.h"
#include "gamestate.h"
#include "graphics.h"

extern GameScene *_CurrentScene;
extern vector<GameScene*> _SceneStack;
extern GameState _GameState;
//pause variables
Menu *_PauseMenu;

//status screen variables
//vector<struct Menu_T*> _StatusMenuStack;

void LoadPauseMenu(){
	_PauseMenu = LoadMenu(MENU_CUSTOM,0,NULL,CURSTYPE_RECT,ANCHOR_TOP_LEFT,3);
	Menu *m = _PauseMenu;

	/*for(int i = 0; i < m->numItems; i++){
		SetMenuItemAction(m->items.at(i),CancelMenu,names[i]);
	};*/
	AddMenuItem(m,"Status",OpenStatus);
	AddMenuItem(m,"Characters",OpenEncyclopedia);
	AddMenuItem(m,"Inventory",OpenInventory);
	AddMenuItem(m,"Options",OpenOptions);
	AddMenuItem(m,"Save",OpenSaveLoad);
	AddMenuItem(m,"Exit",CancelMenu);

}

void OpenPauseMenu(int val){
	OpenMenu(_PauseMenu);
}

void OpenStatus(int val){
	_CurrentScene = new StatusScreen();
}
void StatusScreen::Update(){
	if(InputPressed(PPINPUT_B))
		delete this;
}
void StatusScreen::Draw(){
	DrawBGAnimation(this->bg);
	for(int i = 0; i < (int)this->UIcontents.size(); i++){
		this->UIcontents.at(i)->Draw();
	}
	if(!this->MenuStack.empty()){
		for(int i = 0; i < (int)this->MenuStack.size(); i++){
			DrawMenu(this->MenuStack.at(i));
		}
	}
}

void OpenEncyclopedia(int val){

}

void OpenInventory(int val){
	
}
void OpenOptions(int val){

}
void OpenSaveLoad(int val){

}
