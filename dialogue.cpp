#include "graphics.h"
#include "dialogue.h"
#include "pstring.h"
#include "audio.h"

#include <stdio.h>
#include <iostream>

///for parsing -- see below
//#include "combat_ent.h"
//

extern SDL_Renderer *mainRenderer;
extern Camera uiCamera;


TextboxEX mainTextbox;	//the main dialogue box where all the text is drawn during conversations and stuff

////externs for text parsing -- i dont like doing this and will probably change it 
//extern CombatEnt *CombatParty[MAX_PARTY_COMBAT];
//extern CombatEnt *Enemies[MAX_ENEMIES];



Message *NewMessage(){
	Message *msg = new Message;
	msg->text[0] = '\0';
	msg->numChildren = 0;
	for(int i = 0; i < MAX_PROMPT_CHOICES; i++)
		msg->next[i] = NULL;	
	msg->hasPrompt = 0;
	msg->prompt = NULL;
	msg->numFunctions = 0;
	return msg;
}

void CreateMessage(Message *msg, char* text, OverworldEnt *speaker){
	if(msg != NULL)
		delete(msg);
	msg = NewMessage();

	copy_string(msg->text,text);
	msg->hasPrompt = 0;
	msg->prompt = NULL;
	msg->numFunctions = 0;
//	_MessageStack = msg;

	SetSpeaker(msg,speaker->chardata);
}

void SetSpeaker(Message *msg, CharData *speaker){
	if(speaker != NULL){
		msg->speaker = speaker;
		msg->hasSpeaker = true;
	}else{
		msg->speaker = NULL;
		msg->hasSpeaker = false;
	}
	for(int i = 0; i < 6; i++){
		if(msg->next[i] != NULL){
			SetSpeaker(msg->next[i], speaker);
		}	
	}
}

void CreateMonologue(Message *msg, OverworldEnt *speaker, int numMessages, ...){
	va_list args;
	char* text;
	speaker->talks = true;
	if(msg == NULL)
		msg = NewMessage();
	va_start(args,numMessages);
	for(int i = 0; i < numMessages; i++){
		text = va_arg(args,char*);
		CreateMessage(msg,text,speaker);
		if(i + 1 < numMessages){
			msg->next[0] = NewMessage();
			msg = msg->next[0];
		}
	}
	va_end(args);
}

void SetPrompt(Message *msg, MenuType type, Vec2i *loc){
	msg->hasPrompt = 1;
	if(type == MENU_YES_NO)
		msg->prompt = LoadMenuYesNo(loc);
	else 
		msg->prompt = LoadMenu(type,loc);
	
}

void SetAnswers(Message *msg, int num,  void(*func1)(), void(*func2)(), void(*func3)(), void(*func4)(), void(*func5)(), void(*func6)()){
	msg->numFunctions = num;

	msg->promptFunctions[0] = func1;	
	msg->promptFunctions[1] = func2;
	msg->promptFunctions[2] = func3;
	msg->promptFunctions[3] = func4;
	msg->promptFunctions[4] = func5;
	msg->promptFunctions[5] = func6;
	for(int i = 5; i >= num; i--)
		msg->promptFunctions[i] = NULL;
}

void SetSpeakerbox(Textbox *sb, char *speaker){
	
	string speakerstr(speaker);
	sb->box.w = sb->buf*4 + GetStringWidth(speakerstr);
	SetText(speaker,sb,0);
}

void SetText(char *text, Textbox *t, bool scroll, bool prompt, Message *msg){
	//sets a textbox up for display

	int i;
	bool done = false;

	if(text == NULL) return;
	string parsed_text = string(text); 
	//Longterm goal: add a function that will properly parse and rebuild the text (e.g. "[VAR-NAME]" will be replaced with a name) 
	
//	if(DEBUG)
//		printf("%s \n",parsed_text.c_str());
	t->curline = 0;
	if(scroll){
		t->cursor = 0;
	}
	else{
		t->cursor = -1;
	}
	t->text = parsed_text;

	t->donewriting = false;
}

void SetTextEX(char *text, TextboxEX *t, bool scroll, bool prompt, Message *msg){
	//sets a textbox up for display
	SetText(text,t,scroll,prompt);

	if(msg != NULL){
		t->msg = msg;
		if(msg->hasSpeaker)
			SetSpeakerbox(t->speakerbox,t->msg->speaker->name); 
	}
}

char* ParseText(char *text){

	// DONT USE THIS -- BROKEN AND BAD
	if(text == NULL) return NULL;
	static char parse_key[32];
	memset(&parse_key[0], 0, sizeof(parse_key));
	int parsecursor = 0;
	int textcursor = 0;
	char newText[255];
	copy_string(newText,text);

	for(;;textcursor++){
		if(newText[textcursor] == '%'){
			int subcursor = 0;
			do{
				parse_key[subcursor] = text[subcursor+textcursor];
				subcursor++;
			}while(newText[subcursor+textcursor] != '%');
			parse_key[subcursor]= '%';
			subcursor++;
			copy_string(newText,CutString(newText,textcursor,subcursor));
//			if(strcmp(parse_key,"%ENAME%")==0)
//				copy_string(newText,InjectString(newText,Enemies[0]->chardata->name,textcursor));
//			if(strcmp(parse_key,"%BNAME%")==0)
//				copy_string(newText,InjectString(newText,"ENRAGED EGG",textcursor));
			return ParseText(newText);
		}
		if(newText[textcursor] == '\0') {
			printf(parse_key);
			break;
		}
	}
	return newText;
}

char *InjectString(char *text,char *str, int location){
	char newText[255];
	strncpy(newText,text,location);
	newText[location] = '\0';
	strcat(newText,str);
	strcat(newText,text+location);

	return newText;
}

char *CutString(char *text, int location, int length){
	while(text[location] != '\0') {
		text[location] = text[location + length];
		location++;
	}
	text[location] = '\0';

	return text;
}


int GetNextWordLength(std::string str){
	int ct = 0;
	while((ct < str.length())&&!isspace(str.at(ct))){
		ct++;
	}
	return ct;
}


void SetMessagePrompts(Message *msg){
	for (int i = 0; i < msg->numFunctions; i++){ 
		msg->prompt->items[i]->action = msg->promptFunctions[i] ;
	}
}

void SetMessageEndFunction(Message *msg, void (*func)()){
	// not sure if this works yet
	msg->atEnd = func;
}

void AdvanceText(int steps){
	if(mainTextbox.msg->next[steps] != NULL){
		mainTextbox.msg = mainTextbox.msg->next[steps];
		SetTextEX(mainTextbox.msg->text,&mainTextbox,1,mainTextbox.msg->hasPrompt,mainTextbox.msg);
	}
}

Message *AskQuestion(char* q, char* a1, char* a2){
	Message *msg = NewMessage();
	CreateMessage(msg,q);
	msg->next[0] = NewMessage();
	CreateMessage(msg->next[0],a1);
	msg->next[1] = NewMessage();
	CreateMessage(msg->next[1],a2);
	SetPrompt(msg,MENU_YES_NO);
	return msg;
}

void LoadDialogue(){		//I don't like loading all of the possible dialogue on the map at once, I want to replace this

	

	Vec2i promptloc;
	promptloc.x = LOC_DEFAULT_PROMPT_X;
	promptloc.y = LOC_DEFAULT_PROMPT_Y;

	
	
//	Message *massage = OpenDialogue("testfiles/test.json");

	NPC **npclist;
	npclist = LoadEntitiesCFG("testfiles/chunk1-npc.json");


	/*NPC *npc2 = new NPC(4,8,"Zach");
	NPC *npc3 = new NPC(9,8,"Pete");

	std::cout<<OUTPUT(npc2->name);
//	GiveNPCMessage(npclist[0],massage);

	npc2->msg = NewMessage();
	CreateMonologue(npc2->msg,npc2,2,"Ball...","...is life.");
//	GiveNPCMessage(npc2,npc2->msg);
//	GiveNPCMessage(npc2,massage);

	Message *massage = AskQuestion("Where are you from?","Yes? You're from Yes???","No? You have to be from somewhere...");
//	SetAnswers(massage,2,SelectAnswer1,SelectAnswer2);

	GiveNPCMessage(npc3,massage);*/


	InteractableObject *sign = LoadSign(3,5);

	CreateMonologue(sign->flavortext,sign,2,"Press Shift (the left one) to open the  menu!","You can't use items yet though...");
	
	/*InteractableObject *sign2 = LoadSign(6,3);


	CreateMessage(sign2->flavortext,"Am I... A sign?",sign2);
	sign2->flavortext->next[0] = NewMessage();
	CreateMessage(sign2->flavortext->next[0],"...What the hell!?",sign2);
	sign2->flavortext->next[1] = NewMessage();
	CreateMessage(sign2->flavortext->next[1],"Oh, okay. Thank God.",sign2);
	SetPrompt(sign2->flavortext,MENU_YES_NO);*/
//	SetAnswers(sign2->flavortext,2,SelectAnswer1,SelectAnswer2);

	/*
	InteractableObject *egg = LoadEgg(6,1);
	CreateMessage(egg->flavortext,". . .",egg);*/
//	SetMessageEndFunction(egg->flavortext, FightBoss);

}