#ifndef _PTEXT_
#define _PTEXT_

#include "global.h"
#include "sprites.h"
#include "pstring.h"

#include <string>
using std::string;

enum Justify{
	JUSTIFY_LEFT = 0,	
	JUSTIFY_RIGHT = 1,
	JUSTIFY_MIDDLE = 2,
	JUSTIFY_FULL = 3
};

typedef struct Textbox_T{
	Sprite *spr;					//the sprite associated with the textbox (if any)
	int frame;						
	SDL_Rect box;					//the rectangle the text is drawn in

	int buf;						//inset from the box edges where drawing text happens
	int kerning;					//space between each letter sprite
	Justify justification;			//ooh we fancy

	struct Textbox_T *speakerbox;	//the little speaker textbox above the main textbox -- maybe shouldnt be here 
	
	Animation *arrow;				//little arrow at the bottom that signifies there's more text
	Vec2i arrowpos;					//position of arrow, relative to textbox
	bool usesArrow;					//whether or not we use the arrow at all 

	int vscroll;					//if the textbox scrolls vertically, this is its scroll speed. 0 means no scroll
	int curline;					//the line number of the top line drawn in the textbox currently - for vscroll

	int linect;						//the number of lines this specific textbox holds
	int linelength;					//the number of chars per line this textbox holds (default 40)

	std::string text;				//the actual text currently in the textbox

	int cursor;						//For drawing text one character at a time - set to -1 to draw all text at once
	bool donewriting;				//checks if we have finished writing text
	struct Message_T *msg;			//if the textbox's text came from a message, it will be referenced here 

}Textbox;



void InitFont();
void InitMainTextbox(Textbox *t,int numLines,int lineLen, Sprite *spr);
void LoadTextbox(Textbox *t,int numLines,int lineLen, Sprite *spr, SDL_Rect r, int buffer = 0);

void TextboxSettings(Textbox *t, int buffer = 0, int just = 0, int vscroll = 0, int kerning = 0, bool useArrow = false);

void DrawTextbox(Textbox *t, int offset_x = 0, int offset_y = 0);
void DrawText(Textbox *t);
void DrawLine(string msg,SDL_Rect location, int kerning);

#endif