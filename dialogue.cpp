#include "graphics.h"
#include "dialogue.h"
#include "audio.h"
#include <stdio.h>


extern SDL_Renderer *mainRenderer;
extern Camera uiCamera;

TTF_Font *dialogueFont;
Sound *textBlip;

Textbox mainTextbox; //the main dialogue box for now
bool dialogue;	//are we currently talking?

void InitFont(){
	if(TTF_Init()!=0) 
		return;
	dialogueFont = TTF_OpenFont("fonts/font2.ttf",FONT_RES);
	if(dialogueFont == NULL)
		return;
}

void InitTextbox(Textbox *t){
	textBlip = LoadSound("sounds/blip.wav",30);
	t->spr = LoadSprite("sprites/textbox.png",320,80,1);
	t->box.x = 0;
	t->box.y = GAME_RES_Y - t->spr->h;
	t->box.w = TEXTAREA_W;
	t->box.h = TEXTAREA_H;
	t->cursor = 0;
	for(int i = 0; i < LINE_COUNT; i++){
		t->lines[i] = (char*)malloc(sizeof(char)*(LINE_LENGTH + 1));
		t->lines[i][0] = '\0';
	}
	SetMessage(NULL,t);
}

void DrawTextbox(Textbox *t){
	Vec2i loc;
	loc.x = t->box.x;
	loc.y = t->box.y;
	DrawSprite(t->spr,t->frame,loc,&uiCamera);

	DrawMessage(t);
}

void SetMessage(char *msg, Textbox *t){
	int ptr = 0;
	int offset = 0;
	bool done = false;
	t->cursor = 0;
	for(int i = 0; i < LINE_COUNT; i++){
		if(done)
			break;
		char *line = t->lines[i];
		if(msg==NULL){
			done = true;
			line[0]='\0';
			continue;
		}
		while(ptr<LINE_LENGTH){
			if(msg[ptr+offset] == '\n')
				break;
			if(msg[ptr+offset] == '\0'){
				done = true;
				break;
			}
			line[ptr] = msg[ptr+offset];
			ptr++;
		}
		while(ptr<LINE_LENGTH){
			line[ptr] = ' ';
			ptr++;
		}
		offset += LINE_LENGTH;
		ptr = 0;
		line[LINE_LENGTH] = '\0';
		memcpy(t->lines[i],line,sizeof(char[LINE_LENGTH+1]));
	}

}

void DrawMessage(Textbox *t){
	SDL_Rect temp = {t->box.x,2+t->box.y,t->box.w,(int)(t->box.h*0.2)};
	
	if(t->cursor>=0){
		if(t->cursor < (LINE_LENGTH*LINE_COUNT)){
			++t->cursor;
		}
		for(int i = 0; i < LINE_COUNT; i++){
			if(t->cursor > LINE_LENGTH*(i+1)){
				DrawLine(t->lines[i],temp);
				temp.y += 2+temp.h;
			}
			else{
				char *line = (char*)malloc(sizeof(char)*(LINE_LENGTH+1));
				memcpy(line,t->lines[i],sizeof(char)*(LINE_LENGTH+1));
				for(int j = (t->cursor - (LINE_LENGTH*i)); j < LINE_LENGTH; j++){
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
	}else 


	{
 		for(int i = 0; i < LINE_COUNT; i++){
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