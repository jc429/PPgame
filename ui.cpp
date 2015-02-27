#include "ui.h"

Mouse mouse;
extern Camera uiCamera;

//Updates the mouse state (both buttons act as left for now)
void UpdateMouse(){
	if(SDL_GetMouseState(&mouse.pos.x,&mouse.pos.y))
		UpdateInput(&mouse.left,1);
	else
		UpdateInput(&mouse.left,0);
	
}

//Updates an Input struct based on what happened this frame and last frame
void UpdateInput(Input *input, bool current){
	input->pressed=0;
	input->held=0;
	input->released=0;
	if(current){
		if(input->prev==0){
			input->pressed=1;
		}else if(input->prev==1){
			input->held=1;
		}
		input->prev = 1;
	}else{
		if(input->prev==1){
			input->released=1;
		}
		input->prev = 0;
	}
}

void DrawOverworldUI(){

}

void DrawCombatUI(){

}