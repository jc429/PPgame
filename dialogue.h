#ifndef _DIALOGUE_
#define _DIALOGUE_

#include <SDL.h>
#include <SDL_ttf.h>
#include "settings.h"

typedef struct Textbox_T{
	Sprite *spr;
	int frame;
	SDL_Rect box; 

	//Some sort of text struct

	char *lines[4];
}Textbox;

void InitFont();
void InitTextbox(Textbox *t);
void DrawTextbox(Textbox *t);

void SetMessage(char *msg, Textbox *t);

void DrawMessage(Textbox *t);
void DrawLine(char *msg,SDL_Rect location);
void RenderText();

#endif