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
	void (*promptFunctions[MAX_PROMPT_CHOICES])(int val);

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

	//functions
	void SetSpeaker(CharData *speaker);
	void SetMessagePrompts();
	void SetMessageEndFunction(void (*func)());


}Message;


Message *NewMessage();
void CreateMessage(Message *msg = NULL, char* text = NULL, class OverworldEntity *speaker = NULL);
void CreateMonologue(Message *msg, OverworldEntity *speaker, int numMessages, ...);
void SetPrompt(Message *msg, MenuType type, int numitems = 0,Vec2i *loc = NULL);

//this function is bad and probably pointless tbh
void SetAnswers(Message *msg, int num,  void(*func1)(int val)=SelectAnswer, void(*func2)(int val)=SelectAnswer, 
	void(*func3)(int val)=SelectAnswer, void(*func4)(int val)=SelectAnswer, 
	void(*func5)(int val)=SelectAnswer, void(*func6)(int val)=SelectAnswer);

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