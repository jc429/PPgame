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

	char *lines[LINE_COUNT];		//The textbox can hold 4 lines of 40 characters each
	int cursor;			//For drawing text one character at a time - set to -1 to draw all text immediately
}Textbox;

typedef struct Conversation_T{
	Sprite *speakers[5]; //idk
	Textbox *textbox;
}Conversation;

void InitFont();
void InitTextbox(Textbox *t);
void DrawTextbox(Textbox *t);

void SetMessage(char *msg, Textbox *t);

void DrawMessage(Textbox *t);
void DrawLine(char *msg,SDL_Rect location);
void RenderText();

#endif