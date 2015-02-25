#include "graphics.h"
#include "dialogue.h"

#include <stdio.h>

extern SDL_Renderer *mainRenderer;
extern Camera uiCamera;
TTF_Font *dialogueFont;

Textbox testbox; //the main dialogue box for now
bool dialogue;	//are we currently talking?

void InitFont(){
	if(TTF_Init()!=0) 
		return;
	dialogueFont = TTF_OpenFont("fonts/font2.ttf",FONT_RES);
	if(dialogueFont == NULL)
		return;
}

void InitTextbox(Textbox *t){
	t->spr = LoadSprite("sprites/textbox.png",320,80,1);
	t->box.x = 0;
	t->box.y = GAME_RES_Y - t->spr->h;
	t->box.w = 320;
	t->box.h = 80;
	for(int i = 0; i < 4; i++){
		t->lines[i] = 	(char*)malloc(sizeof(char)*41);
		t->lines[i][0] = '\0';
	}
	SetMessage(NULL,t);
}

void DrawTextbox(Textbox *t){
	Vec2i loc;
	loc.x = t->box.x;
	loc.y = t->box.y;
	DrawSprite(t->spr,t->frame,loc,&uiCamera);
	
//	SetMessage("This is a test message34567890123456780.1234567890123456789\\234567890123456789.12345 Hey chump. Does everything work? 9.1234567890123 hella. 123456789012345",t);

	DrawMessage(t);
}

void SetMessage(char *msg, Textbox *t){
	int ptr = 0;
	int offset = 0;
	bool done = false;
	
	for(int i = 0; i < 4; i++){
		if(done)
			break;
		char *line = t->lines[i];
		if(msg==NULL){
			done = true;
			line[0]='\0';
			continue;
		}
		while(ptr<40){
			if(msg[ptr+offset] == '\n')
				break;
			if(msg[ptr+offset] == '\0'){
				done = true;
				break;
			}
			line[ptr] = msg[ptr+offset];
			ptr++;
		}
		while(ptr<40){
			line[ptr] = ' ';
			ptr++;
		}
		offset += 40;
		ptr = 0;
		line[40] = '\0';
		memcpy(t->lines[i],line,sizeof(char[41]));
	}

}

void DrawMessage(Textbox *t){
	SDL_Rect temp = {t->box.x,2+t->box.y,t->box.w,t->box.h*0.2};
	for(int i = 0; i < 4; i++){
		DrawLine(t->lines[i],temp);
		temp.y += 2+temp.h;
	}
}

void DrawLine(char *msg,SDL_Rect location){
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