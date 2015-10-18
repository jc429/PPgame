#include "dialogue.h"
#include "graphics.h"
#include "input.h"

//bool inMenu;		//are we currently in a menu?
Mouse mouse;
extern Camera uiCamera;
extern InputNode *_Inputs;


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


void DeleteInputNode(InputNode *node,int steps){
	if(steps <= 0)
		delete(node);
	else{
		if(node->prev != NULL)
			DeleteInputNode(node->prev, steps-1);
	}
}


bool InputPressed(Uint16 input,InputNode *node){
	if(node == NULL)
		node = _Inputs;
	return ((node->input & input)&&!(node->prev->input & input));
}

bool InputReleased(Uint8 input,InputNode *node){
	if(node == NULL)
		node = _Inputs;
	return (!(node->input & input)&&(node->prev->input & input));
}

bool InputBuffered (InputNode *input, int button, int buf){
	if(buf > INPUTS_HISTORY)
		buf = INPUTS_HISTORY;
	if(input->input & button){
		if(buf == 0)
			return 1;
		else{
			if(input->prev != NULL)
				return (InputBuffered(input->prev,button,buf-1));
			else
				return 0;
		}
	}else
		return 0;
}

