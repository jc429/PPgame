#ifndef _DIALOGUE_
#define _DIALOGUE_

#include <SDL.h>
#include <SDL_ttf.h>
#include "settings.h"
#include "ptext.h"



typedef struct Conversation_T{
	Sprite *speakers[5]; //idk
	Textbox *textbox;
}Conversation;

typedef struct Message_T{
	char *text;
	
	bool hasPrompt;
	Menu *prompt;
	int numFunctions;
	void (*promptFunctions[MAX_PROMPT_CHOICES])();
	struct Message_T *next;

	bool active;

	Message_T(){
		text = NULL;
		prompt = NULL;
		next = NULL;
		for(int i = 0; i < MAX_PROMPT_CHOICES; i++){
			promptFunctions[i] = NULL;
		}
	};
}Message;

void InitFont();
void InitMainTextbox(Textbox *t,int numLines,int lineLen, Sprite *spr);
void LoadTextbox(Textbox *t,int numLines,int lineLen, Sprite *spr, SDL_Rect r);
void DrawTextbox(Textbox *t);

void CreateMessage(Message *msg, char* text);
void SetPrompt(Message *msg, MenuType type, Vec2i loc);
void SetAnswers(Message *msg, int num,  void(*func1)()=NULL, void(*func2)()=NULL, void(*func3)()=NULL, void(*func4)()=NULL, void(*func5)()=NULL, void(*func6)()=NULL);
void SetMessagePrompts(Message *msg);

void SetText(char *text, Textbox *t, bool scroll, bool prompt = 0, Message *msg = NULL);

void DrawText(Textbox *t);
void DrawLine(char *msg,SDL_Rect location);
void RenderText();


static bool _Dialogue;	//are we currently talking?

static Message *_MessageStack;
static int numMenus;



#endif