#include "graphics.h"
#include "dialogue.h"
#include "audio.h"
#include <stdio.h>


extern SDL_Renderer *mainRenderer;
extern Camera uiCamera;

TTF_Font *dialogueFont;
Sound *textBlip;

Textbox mainTextbox; //the main dialogue box for now
Textbox combatTextbox; //combat has a different one

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
	LoadTextbox(t,numLines,lineLen,spr,r);
}

void LoadTextbox(Textbox *t, int numLines, int lineLen,Sprite *spr, SDL_Rect rect){
	if(numLines > LINE_COUNT)
		numLines = LINE_COUNT;
	t->linect = numLines;
	if(lineLen > LINE_LENGTH)
		lineLen = LINE_LENGTH;
	t->linelength = lineLen;
	
	t->spr = spr;
	t->box = rect;

	t->cursor = 0;
	for(int i = 0; i < t->linect; i++){
		t->lines[i] = (char*)malloc(sizeof(char)*(t->linelength + 1));
		t->lines[i][0] = '\0';
	}
	SetText(NULL,t,0);
	t->donewriting = true;
}

void DrawTextbox(Textbox *t){
	Vec2i loc;
	loc.x = t->box.x;
	loc.y = t->box.y;
	DrawSprite(t->spr,t->frame,loc,&uiCamera);

	DrawText(t);
}

void CreateMessage(Message *msg, char* text){
	//msg = new Message;
	msg->text = text;
	msg->next = NULL;
	msg->prompt = NULL;
	msg->numFunctions = 0;
	_MessageStack = &*msg;
}

void SetPrompt(Message *msg, MenuType type, Vec2i loc){
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
}

void SetText(char *text, Textbox *t, bool scroll, bool prompt, Message *msg){
	int ptr = 0;			//what character we're currently drawing up to
	int offset = 0;			//offset for the lines
	int i;
	t->donewriting = false;

	if(scroll){
		t->cursor = 0;
	}
	else{
		t->cursor = -1;
	//	t->donewriting = true;
	}
	
	for(i = 0; i < t->linect; i++){
		if(t->donewriting){
			while(i < t->linect){
				t->lines[i][0]='\0';
				i++;
			}
			break;
		}
		char *line = t->lines[i];
		if(text==NULL){
			t->donewriting = true;
			continue;
		}
		while(ptr<LINE_LENGTH){
			if(text[ptr+offset] == '\n')
				break;
			if(text[ptr+offset] == '\0'){
				t->donewriting = true;
				break;
			}
			line[ptr] = text[ptr+offset];
			ptr++;
		}
		while(ptr<LINE_LENGTH){
			line[ptr] = ' ';
			ptr++;
		}
		offset += t->linelength;
		ptr = 0;
		line[t->linelength] = '\0';
		memcpy(t->lines[i],line,sizeof(char)*(t->linelength+1));
	}
	if((t->donewriting)&&(prompt)){
		msg->active = true;
	}
}

void DrawText(Textbox *t){
	SDL_Rect temp = {t->box.x,2+t->box.y,t->box.w,(int)(t->box.h*0.2)};


	if(t->cursor>=0){
		if(t->cursor < (t->linelength*t->linect)){
			++t->cursor;
		}
		for(int i = 0; i < t->linect; i++){
			if(t->cursor > t->linelength*(i+1)){
				DrawLine(t->lines[i],temp);
				temp.y += 2+temp.h;
			}
			else{
				char *line = (char*)malloc(sizeof(char)*(t->linelength+1));
				memcpy(line,t->lines[i],sizeof(char)*(t->linelength+1));
				for(int j = (t->cursor - (t->linelength*i)); j < t->linelength; j++){
					if(j<0) 
						j = 0;
					if(line[j] == '\0'){
						break;
					}else
						line[j] = ' ';
				}
 				DrawLine(line,temp);
				temp.y += 2+temp.h;
				free(line);
			}
		}
	}else {
		for(int i = 0; i < t->linect; i++){
			DrawLine(t->lines[i],temp);
			temp.y += 2+temp.h;
		}
	}
}

void DrawLine(char *msg,SDL_Rect location){
	if(msg == NULL) return;
	SDL_Color color = {255,255,255,0};
	//targetarea.w = 100;
	SDL_Surface *temp = TTF_RenderText_Blended(dialogueFont,msg,color); 
	SDL_Texture *texture = SDL_CreateTextureFromSurface(mainRenderer,temp);
	SDL_Rect targetarea = {location.x+TEXTAREA_INSET,location.y+TEXTAREA_INSET,location.w-(2*TEXTAREA_INSET),location.h};
	

	SDL_RenderCopy(mainRenderer,texture,NULL,&targetarea);

//	if(DEBUG)
//		SDL_RenderDrawRect(mainRenderer, &targetarea);

	SDL_FreeSurface(temp);

	SDL_DestroyTexture(texture);
}