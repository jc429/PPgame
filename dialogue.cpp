#include "graphics.h"
#include "dialogue.h"
#include "pstring.h"
#include "audio.h"

#include <stdio.h>
#include <iostream>

///for parsing -- see below
#include "combat_ent.h"
//

extern SDL_Renderer *mainRenderer;
extern Camera uiCamera;

TTF_Font *dialogueFont;
Sound *textBlip;

Textbox mainTextbox; //the main dialogue box for now
Textbox combatTextbox; //combat has a different one

////externs for text parsing -- i dont like doing this and will probably change it 
extern CombatEnt *CombatParty[MAX_PARTY_COMBAT];
extern CombatEnt *Enemies[MAX_ENEMIES];


void InitFont(){
	if(TTF_Init()!=0) 
		return;
	dialogueFont = TTF_OpenFont("fonts/font2.ttf",FONT_RES);
	if(dialogueFont == NULL)
		return;
}

void InitMainTextbox(Textbox *t,int numLines, int lineLen,Sprite *spr){
	textBlip = LoadSound("sounds/blip.wav",30);
	SDL_Rect r;
	r.x = 0;
	r.y = GAME_RES_Y - spr->h;
	r.w = TEXTAREA_W;
	r.h = TEXTAREA_H;
	LoadTextbox(t,numLines,lineLen,spr,r,1);
	
	SDL_Rect s = {10,(GAME_RES_Y - spr->h)-18,128,16};
	t->speakerbox = new Textbox();
	LoadTextbox(t->speakerbox,1,16,NULL,s,0);

}

void LoadTextbox(Textbox *t, int numLines, int lineLen,Sprite *spr, SDL_Rect rect, bool hasArrow){
	if(numLines > LINE_COUNT)
		numLines = LINE_COUNT;
	t->linect = numLines;
	if(lineLen > LINE_LENGTH)
		lineLen = LINE_LENGTH;
	t->linelength = lineLen;
	
	t->spr = spr;
	t->box = rect;

	t->speakerbox = NULL;
	
	if(hasArrow){
		t->usesArrow = true;
		t->arrow = LoadAnimation(LoadSprite(SPATH_TEXTBOX_ARROW,8,8,2),0,0,2,1,1,18);	
		t->arrowpos.x = t->box.x + t->box.w - 16;
		t->arrowpos.y = t->box.y + t->box.h - 16;
	}else
		t->usesArrow = false;

	t->cursor = 0;
	for(int i = 0; i < t->linect; i++){
//		t->lines[i] = (char*)malloc(sizeof(char)*(t->linelength + 1));
//		t->lines[i][0] = '\0';
	}
	SetText(NULL,t,0);
	t->donewriting = false;
	t->msg = NULL;
}



void DrawTextbox(Textbox *t, int offset_x, int offset_y){
	t->box.x += offset_x;
	t->box.y += offset_y;
	Vec2i loc;
	loc.x = t->box.x;
	loc.y = t->box.y;
	DrawSprite(t->spr,t->frame,loc,&uiCamera);
	
	DrawText(t);

	if(t->msg && t->msg->hasSpeaker){
		if(t->msg->speaker != NULL){
			DrawPanel(t->speakerbox->box,LoadSprite(SPATH_PANEL_DEF,4,4,3));
			DrawTextbox(t->speakerbox,0,-6);
		}
	}

	t->box.x -= offset_x;
	t->box.y -= offset_y;
}

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

void SetText(char *text, Textbox *t, bool scroll, bool prompt, Message *msg){
	//sets a textbox up for display

	int ptr = 0;			//what character we're currently drawing up to
	int offset = 0;			//offset for the lines
	int i;
	bool done = false;

	if(text == NULL) return;
	string parsed_text = string(text); //gotta do this better 
	
	if(DEBUG)
		printf("%s \n",parsed_text.c_str());

	if(scroll){
		t->cursor = 0;
	}
	else{
		t->cursor = -1;
	//	done = true;
	}
	
	for(i = 0; i < t->linect; i++){
		//string line = t->lines[i];
		if(parsed_text.empty()){
			done = true;
		}

		t->text = parsed_text;//.substr(0,t->linelength);
		//parsed_text = parsed_text.substr(t->linelength,t->linelength);

		/*if(done){
			while(i < t->linect){
				for(int j = 0; j < t->linelength; j++)
					t->lines[i].append(" ");
				i++;
			}
			break;
		}*/
		
	/*	while(ptr < t->linelength){
			if(parsed_text.at(ptr+offset) == '\n')
				break;
			if(parsed_text.at(ptr+offset) == '\0'){
				done = true;
				break;
			}
			line.at(ptr) = parsed_text.at(ptr+offset);
			ptr++;
		}*/


		/*while(ptr < t->linelength){
			line[ptr] = ' ';
			ptr++;
		}*/
		offset += t->linelength;
		ptr = 0;
	//	t->lines[i] = line;
		//memcpy(t->lines[i],line,sizeof(char)*(t->linelength));
	}
//	if((t->donewriting)&&(prompt))
//	if(prompt){
		if(msg != NULL){
			t->msg = msg;
			if(msg->hasSpeaker)
				SetText( t->msg->speaker->name,t->speakerbox,0); 
		}
	t->donewriting = false;
}

char* ParseText(char *text){
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
			if(strcmp(parse_key,"%ENAME%")==0)
				copy_string(newText,InjectString(newText,Enemies[0]->chardata->name,textcursor));
			if(strcmp(parse_key,"%BNAME%")==0)
				copy_string(newText,InjectString(newText,"ENRAGED EGG",textcursor));
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

void DrawText(Textbox *t){
	//draws the text in a textbox
	/*if(t->cursor >= 0 && t->cursor < 159)
		printf(" %i ,", t->cursor);*/

	SDL_Rect temp = {t->box.x,2+t->box.y,t->box.w,(int)(t->box.h*0.2)};
	if(t->linect == 1){
		temp.h = t->box.h;
	}

	if(t->donewriting == true){
		if(t->usesArrow)
			DrawAnimation(t->arrow,t->arrowpos,&uiCamera);
		if(t->msg != NULL)
			if(t->msg->hasPrompt)
				if(t->msg->prompt != NULL){
					SetMessagePrompts(t->msg);
					if(!t->msg->prompt->active)
						OpenMenu(t->msg->prompt);
				}
	}

	if((t->cursor >= 0)&&(t->cursor < t->text.length())){

		++t->cursor;		
			
		if(t->cursor < t->text.length()){
			while(t->text.at(t->cursor) == ' '){ //skip white space
				++t->cursor;
				if(t->cursor >= t->text.length()) 
					break;
			}
		}
		

		for(int i = 0; i < t->linect; i++){

			string line;
			
			if((i * t->linelength) > t->text.length()) //if we're already past the last line with text
				break;
			
			int textlength = t->cursor - (t->linelength*i); //get how far the cursor is from the most recent line ending

			if(textlength > t->linelength) //if the cursor is more than a line away
				line = t->text.substr((i * t->linelength),t->linelength); //use linelength
			else 
				line = t->text.substr((i * t->linelength),textlength); //use where the cursor is now

			while(line.length() < t->linelength)
				line.append(" ");		//add spaces until the line reaches the specified line length (for SDL text rendering)

			if(t->cursor >= t->linelength*(i+1)){ //if the cursor is already past this line, just draw it 
				DrawLine(line,temp);
				temp.y += 2+temp.h;
			}
			else{
				
				
				if(t->cursor >= (t->text.length())){		
					t->donewriting = true;
				//	break;
				}

				
 				DrawLine(line,temp);
				temp.y += 2+temp.h;
			}
		}
	}else {
		for(int i = 0; i < t->linect; i++){
			string line;
			if((i * t->linelength) > t->text.length()) //if we're already past the last line with text
				break;
			line = t->text.substr((i * t->linelength),t->linelength); 
			DrawLine(line,temp);
			temp.y += 2+temp.h;
		}
		t->donewriting = true;
	}
}

void DrawLine(string msg,SDL_Rect location){

	//draws a line of text... should probably be renamed...


	if(msg.empty()) return;
	SDL_Color color = {255,255,255,0};
	//targetarea.w = 100;
	SDL_Surface *temp = TTF_RenderText_Blended(dialogueFont,msg.c_str(),color); 
	SDL_Texture *texture = SDL_CreateTextureFromSurface(mainRenderer,temp);
	SDL_Rect targetarea = {location.x+TEXTAREA_INSET,location.y+TEXTAREA_INSET,location.w-(2*TEXTAREA_INSET),location.h};
	

	SDL_RenderCopy(mainRenderer,texture,NULL,&targetarea);
	
//	if(DEBUG_DRAW_RECTS)
//		SDL_RenderDrawRect(mainRenderer, &targetarea);

	SDL_FreeSurface(temp);

	SDL_DestroyTexture(texture);
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
		SetText(mainTextbox.msg->text,&mainTextbox,1,mainTextbox.msg->hasPrompt,mainTextbox.msg);
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


	InteractableObject *egg = LoadEgg(6,1);
	CreateMessage(egg->flavortext,". . .",egg);
	SetMessageEndFunction(egg->flavortext, FightBoss);

}