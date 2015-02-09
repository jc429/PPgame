#include "ui.h"

Mouse mouse;
void UpdateMouse(){
	mouse.clicked=0;
	mouse.held=0;
	mouse.released=0;
	if(SDL_GetMouseState(&mouse.x,&mouse.y)){
		if(mouse.prev==0){
			mouse.clicked=1;
		}else if(mouse.prev==1){
			mouse.held=1;
		}
		mouse.prev = 1;
	}else{
		if(mouse.prev==1){
			mouse.released=1;
		}
		mouse.prev = 0;
	}

}