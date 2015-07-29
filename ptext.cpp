#include "ptext.h"
#include "graphics.h"
#include "audio.h"

Sprite *mainFontSprite;	//the font sprite
bool useFontSprite;
TTF_Font *dialogueFont;
Sound *textBlip;

extern Camera uiCamera;

void InitFont(){
	if(TTF_Init()!=0) 
		return;
	/*dialogueFont = TTF_OpenFont("fonts/font2.ttf",FONT_RES);
	if(dialogueFont == NULL)
		return;*/
	mainFontSprite = LoadSprite("fonts/text7x7.png",8,16,32);
	if(mainFontSprite != NULL)
		useFontSprite = true;

}

void InitMainTextbox(Textbox *t,int numLines, int lineLen,Sprite *spr){
	textBlip = LoadSound("sounds/blip.wav",30);
	SDL_Rect r;
	r.x = 0;
	r.y = GAME_RES_Y - spr->h;
	r.w = TEXTAREA_W;
	r.h = TEXTAREA_H;
	LoadTextbox(t,numLines,lineLen,spr,r,2);
	TextboxSettings(t,2,0,1,0,true);
	SDL_Rect s = {4,r.y-10,72,10};		//width doesn't matter it'll be adjusted dynamically. x technically wont matter either
	t->speakerbox = new Textbox();
	LoadTextbox(t->speakerbox,1,72,NULL,s,1);

}

void LoadTextbox(Textbox *t, int numLines, int lineLen,Sprite *spr, SDL_Rect rect, int buffer){
	if(!t) return;

	if(numLines > LINE_COUNT)
		numLines = LINE_COUNT;
	t->linect = numLines;
/*	if(lineLen > LINE_LENGTH)
		lineLen = LINE_LENGTH;*/
	t->linelength = lineLen;
	
	t->spr = spr;
	t->box = rect;
	t->buf = buffer;

	t->justification = JUSTIFY_LEFT;
	t->kerning = DEFAULT_KERNING;
	t->vscroll = 0;
	t->curline = 0;

	t->speakerbox = NULL;
	
	t->usesArrow = false;
	t->arrow = NULL;

	t->cursor = 0;
	for(int i = 0; i < t->linect; i++){
//		t->lines[i] = (char*)malloc(sizeof(char)*(t->linelength + 1));
//		t->lines[i][0] = '\0';
	}
	SetText(NULL,t,0);
	t->donewriting = false;
	t->msg = NULL;
}

void TextboxSettings(Textbox *t, int buffer, int just, int vscroll, int kerning, bool useArrow){
	if(!t) return;

	t->buf = buffer;
	t->justification = (Justify)just;
	t->vscroll = vscroll;
	t->kerning = kerning;

	if(useArrow){
		t->usesArrow = true;
		t->arrow = LoadAnimation(LoadSprite(SPATH_TEXTBOX_ARROW,8,8,2),0,0,2,1,1,18);	
		t->arrowpos.x = t->box.x + t->box.w - 12;
		t->arrowpos.y = t->box.y + t->box.h - 12;
	}else
		t->usesArrow = false;
}

void DrawTextbox(Textbox *t, int offset_x, int offset_y){
	if(t->msg && t->msg->hasSpeaker){
		if(t->msg->speaker != NULL){
			Vec2i speakerpos;
			speakerpos.x = t->speakerbox->box.x + (t->speakerbox->box.w>>1);	
			speakerpos.y = t->speakerbox->box.y + t->speakerbox->box.h;
			DrawAnimation(t->msg->speaker->portrait,speakerpos,&uiCamera);
		}
	}
	t->box.x += offset_x;
	t->box.y += offset_y;
	//DrawRect(&t->box,&uiCamera,pCol_Blue);
	Vec2i loc;
	loc.x = t->box.x;
	loc.y = t->box.y;
	DrawSprite(t->spr,t->frame,loc,&uiCamera);
	
	DrawText(t);

	if(t->msg && t->msg->hasSpeaker){
		if(t->msg->speaker != NULL){
			DrawPanel(t->speakerbox->box,LoadSprite(SPATH_PANEL_DEF,2,2,3));
			DrawTextbox(t->speakerbox);
		}
	}

	t->box.x -= offset_x;
	t->box.y -= offset_y;
}


void DrawText(Textbox *t){
	//draws the text in a textbox

	if(t->text.empty()) return;

	int buf = t->buf;	//the buffer around all edges of the textbox
	SDL_Rect temp = {t->box.x+buf,t->box.y+buf,t->box.w-(2*buf),t->box.h-(2*buf)};	//the rectangle that text will actually be drawn in

	if(t->donewriting == true){			//if the whole message has been displayed to the screen
		if(t->usesArrow)				//draw the arrow sprite if we have one 
			DrawAnimation(t->arrow,t->arrowpos,&uiCamera);		
		if((t->msg != NULL)&&(t->msg->hasPrompt)){		//if a prompt is supposed to come up 
			if(t->msg->prompt != NULL){
				SetMessagePrompts(t->msg);				//Set the actions -- ideally should not be done here bc it's called every frame
				if(!t->msg->prompt->active)				//reopen the prompt if it's not already open 
					OpenMenu(t->msg->prompt);			//(prevents escaping from prompts without answering)
			}
		}
	}

	if((t->cursor >= 0)&&(t->cursor < t->text.length())){	//if the text is being written letter by letter
															//and we haven't reached the end of it yet
		++t->cursor;		//increment the cursor first. No point in drawing nothing for the first frame.	
			
		if(t->cursor < t->text.length()){	//double check that we haven't gone too far
			while(t->text.at(t->cursor) == ' '){ //skip white space and go to the next letter -- this is kinda optional tbh
				++t->cursor;
				if(t->cursor >= t->text.length()) //if we reach the end, get out
					break;
			}
		}
	}else{		//if the text is NOT being written letter by letter, we're done writing on frame 1 
		t->donewriting = true;
	}

	


	/**** Variables ****/	//put here because the stuff above really doesn't relate to them at all
 	string line;
	int linewidth = 0;		//the current width of the line	(pixels)		

	int start = 0;			//start of the current line
	int curpos = 0;			//current char
	Letter curletter;		
			
	int drawnlines = 0;			//how many lines we've drawn - stop if we pass the max lines the textbox supports
	bool donedrawing = false;	//whether or not we're done drawing text

	curletter = GetLetter(t->text.at(curpos));		//start off by getting the letter we're at right now

	while(!donedrawing){
		if((drawnlines - t->curline) >= t->linect){	//if we've already drawn the maximum number of lines allowed by the textbox
			if(!t->donewriting)
				t->curline++;
		//	donedrawing = true;
		//	printf("Done \n");
			break;
		}

		start = curpos;		//set the start to the position of the current character 
		
		bool line_done = false;		//this variable doesn't /need/ to exist, but I prefer it to writing while(true) loops
		while(!line_done){	//this loops while we work on an individual line in the textbox

			if(curpos >= t->text.length()){		//if there's nothing left to write at all we're done
				donedrawing = true;
				break;
			}
			if((t->cursor >= 0)&&(curpos >= t->cursor)){	//or if we've moved past our current cursor we're done
				donedrawing = true;
				break;
			}

			if(isspace(t->text.at(curpos))){	//if the current character is a space, check if it fits on the line - if not, discard it	
				if((linewidth + GetCharWidth(' ')+ t->kerning) > temp.w){
						//get rid of the space so it doesn't indent the next line
					break;
				}
				linewidth += (GetCharWidth(' ')+ t->kerning);
				curpos++;
				continue;		//nothing below this affects spaces so just go back to the top
			}

			int nextlen = GetNextWordLength(t->text.substr(curpos));	
			int nextwidth = GetWordWidth(t->text.substr(curpos,nextlen),t->kerning);

			//check that the next word fits
			if((linewidth + nextwidth) < temp.w){	//if the word fits
			//	printf("%i \n", linewidth+nextwidth);
				bool wordbyword = false;		//maybe you want to draw word by word idk but here's the code to do it
				if(wordbyword){
					linewidth += nextwidth;
					curpos += nextlen;
				}else{
					linewidth += (GetCharWidth(t->text.at(curpos)) + t->kerning);
					curpos ++;
				}
			}else{									//if the word does not fit
				line_done = true;
				break;
			}
		}

		if(drawnlines >= t->curline){
			line = t->text.substr(start,curpos-start);
			if(t->justification == JUSTIFY_RIGHT){
				temp.x = temp.w - linewidth;	//this looks silly with scrolling on
			}
			DrawLine(line,temp,t->kerning);
			temp.y += 10;
		}
		linewidth = 0;		//reset line size to 0	
		drawnlines++;
	}
	//scrolling up and down for longer textboxes
	if(t->vscroll > 0 && t->donewriting){			//if the textbox has vertical scrolling enabled
		if(InputPressed(PPINPUT_UP)){
			if(t->curline > 0)
				t->curline--;
		}
		
		if(InputPressed(PPINPUT_DOWN)){
			if((t->curline >= 0) && (t->curline+t->linect <= drawnlines))
				t->curline++;
		}
	}

}
	
void DrawLine(string msg,SDL_Rect location, int kerning){
//	DrawRect(location,&uiCamera,pCol_DarkGray);


	//draws a line of text... should probably be renamed...
	if(msg.empty()) return;

	if(useFontSprite){
		Vec2i pos;
		SetVec2i(pos,location.x,location.y);
		for(int i = 0; i < msg.length(); i++){
			Letter l = GetLetter(msg.at(i));
			DrawSprite(mainFontSprite,l.chr,pos,&uiCamera);
			pos.x += (l.width+kerning);
		}
	}
	else{
		/*
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
		*/
	}
}