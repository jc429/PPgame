#ifndef _DIALOGUE_
#define _DIALOGUE_

#include <SDL.h>
#include <SDL_ttf.h>
#include "global.h"
#include "menu.h"
#include "chardata.h"
#include "pfile.h"


typedef struct Conversation_T{
	//Sprite *speakers[5]; //idk
	Textbox *textbox;
}Conversation;

typedef struct Message_T{
	char text[255];
		
	bool hasSpeaker;			
	CharData *speaker;		//for the little name above a text box

	bool hasPrompt;
	Menu *prompt;
	int numFunctions;
	void (*promptFunctions[MAX_PROMPT_CHOICES])();

	void (*atEnd)();

	int numChildren;
	struct Message_T *next[MAX_PROMPT_CHOICES]; 

	bool active;

	Message_T(){
		text[0] = NULL;
		prompt = NULL;
		hasSpeaker = false;
		speaker = NULL;
		for(int i = 0; i < MAX_PROMPT_CHOICES; i++){
			next[i] = NULL;
			promptFunctions[i] = NULL;
		}
		atEnd = NULL;
	};
}Message;


Message *NewMessage();
void CreateMessage(Message *msg = NULL, char* text = NULL, class OverworldEnt *speaker = NULL);
void SetSpeaker(Message *msg, CharData *speaker);
void CreateMonologue(Message *msg, OverworldEnt *speaker, int numMessages, ...);
void SetPrompt(Message *msg, MenuType type, Vec2i *loc = NULL);
void SetAnswers(Message *msg, int num,  void(*func1)()=SelectAnswer1, void(*func2)()=SelectAnswer2, void(*func3)()=SelectAnswer3, 
	void(*func4)()=SelectAnswer4, void(*func5)()=SelectAnswer5, void(*func6)()=SelectAnswer6);
void SetMessagePrompts(Message *msg);
void SetMessageEndFunction(Message *msg, void (*func)());

void SetText(char *text, Textbox *t, bool scroll, bool prompt = 0, Message *msg = NULL);
void SetTextEX(char *text, TextboxEX *t, bool scroll, bool prompt = 0, Message *msg = NULL);
void SetSpeakerbox(Textbox *t, char *speaker);
char *ParseText(char *text);
char *InjectString(char *text,char *str, int location);
char *CutString(char *text, int location, int length);
int GetNextWordLength(std::string str);



void LoadDialogue();
Message *OpenDialogue(char *path);

Message *AskQuestion(char* q, char* a1, char* a2);

static Message *_MessageStack;
static int numMenus;



#endif