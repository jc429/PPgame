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

void InitMainTextbox(TextboxEX *t,int numLines,Sprite *spr){
	textBlip = LoadSound("sounds/blip.wav",30);
	SDL_Rect r;
	int buf = 2; //the indent to compensate for the main textbox edges
	r.x = 0;
	r.y = GAME_RES_Y - spr->h;
	r.w = TEXTAREA_W;
	r.h = TEXTAREA_H;
	LoadTextbox(t,numLines,r,buf);
	t->bgspr = spr;
	SetTextboxSettings(t,2,0,1,0,true);
	SDL_Rect s = {4,r.y-10,72,10};		//width doesn't matter it'll be adjusted dynamically. x technically wont matter either
	t->speakerbox = new Textbox();
	LoadTextbox(t->speakerbox,1,s,1);

}

void LoadTextbox(Textbox *t, int numLines, SDL_Rect rect, int buffer){
	if(!t) return;

	if(numLines > LINE_COUNT)
		numLines = LINE_COUNT;
	t->linect = numLines;
/*	if(lineLen > LINE_LENGTH)
		lineLen = LINE_LENGTH;*/
//	t->linelength = lineLen;
	
	t->box = rect;
	t->buf = buffer;

	t->justification = JUSTIFY_LEFT;
	t->kerning = DEFAULT_KERNING;
	t->vscroll = 0;
	t->curline = 0;
	
	t->usesArrow = false;
	t->arrow = NULL;

	t->cursor = 0;
	for(int i = 0; i < t->linect; i++){
//		t->lines[i] = (char*)malloc(sizeof(char)*(t->linelength + 1));
//		t->lines[i][0] = '\0';
	}
	t->SetText(NULL,0);
	t->donewriting = false;
}

void SetTextboxSettings(Textbox *t, int buffer, int just, int vscroll, int kerning, bool useArrow){
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

//void DrawTextbox(Textbox *t, int offset_x, int offset_y){
void Textbox::Draw(int offset_x,int offset_y){
	box.x += offset_x;
	box.y += offset_y;

//	DrawRect(t->box,&uiCamera);
	
	DrawText();

	box.x -= offset_x;
	box.y -= offset_y;
}

//void DrawTextboxEX(TextboxEX *t, int offset_x, int offset_y){
void TextboxEX::Draw(int offset_x,int offset_y){
	if(msg && msg->hasSpeaker){
		if(msg->speaker != NULL){
			Vec2i speakerpos;
			speakerpos.x = speakerbox->box.x + (speakerbox->box.w>>1);	
			speakerpos.y = speakerbox->box.y + speakerbox->box.h;
			DrawAnimation(msg->speaker->portrait,speakerpos,&uiCamera);
		}
	}
	box.x += offset_x;
	box.y += offset_y;
	//DrawRect(&t->box,&uiCamera,pCol_Blue);
	Vec2i loc;
	loc.x = box.x;
	loc.y = box.y;
	DrawSprite(bgspr,frame,loc,&uiCamera);
//	DrawRect(t->box,&uiCamera);
	
	DrawText();
	if(donewriting == true){			//if the whole message has been displayed to the screen
		if(usesArrow)				//draw the arrow sprite if we have one 
			DrawAnimation(arrow,arrowpos,&uiCamera);		
		if((msg != NULL)&&(msg->hasPrompt)){		//if a prompt is supposed to come up 
			if(msg->prompt != NULL){
				msg->SetMessagePrompts();				//Set the actions -- ideally should not be done here bc it's called every frame
				if(!msg->prompt->active)				//reopen the prompt if it's not already open 
					OpenMenu(msg->prompt);			//(prevents escaping from prompts without answering)
			}
		}
	}
	if(msg && msg->hasSpeaker){
		if(msg->speaker != NULL){
			DrawPanel(speakerbox->box,LoadSprite(SPATH_PANEL_DEF,2,2,3));
			speakerbox->Draw();
		}
	}

	box.x -= offset_x;
	box.y -= offset_y;
}


void Textbox::DrawText(){
	//draws the text in a textbox

	if(text.empty()) return;

	//int buf = buf;	//the buffer around all edges of the textbox
	SDL_Rect temp = {box.x+buf,box.y+buf,box.w-(2*buf),box.h-(2*buf)};	//the rectangle that text will actually be drawn in

	
	if((cursor >= 0)&&(cursor < (int)text.length())){	//if the text is being written letter by letter
															//and we haven't reached the end of it yet
		++cursor;		//increment the cursor first. No point in drawing nothing for the first frame.	
			
		if(cursor < (int)text.length()){	//double check that we haven't gone too far
			while(text.at(cursor) == ' '){ //skip white space and go to the next letter -- this is kinda optional tbh
				++cursor;
				if(cursor >= (int)text.length()) //if we reach the end, get out
					break;
			}
		}
	}else{		//if the text is NOT being written letter by letter, we're done writing on frame 1 
		donewriting = true;
	}

	


	/**** Variables ****/	//put here because the stuff above really doesn't relate to them at all
 	string line;
	int linewidth = 0;		//the current width of the line	(pixels)		

	int start = 0;			//start of the current line
	int curpos = 0;			//current char
	Letter curletter;		
			
	int drawnlines = 0;			//how many lines we've drawn - stop if we pass the max lines the textbox supports
	bool donedrawing = false;	//whether or not we're done drawing text

	curletter = GetLetter(text.at(curpos));		//start off by getting the letter we're at right now

	while(!donedrawing){
		if((drawnlines - curline) >= linect){	//if we've already drawn the maximum number of lines allowed by the textbox
			if(!donewriting)
				curline++;
		//	donedrawing = true;
		//	printf("Done \n");
			break;
		}

		start = curpos;		//set the start to the position of the current character 
		
		bool line_done = false;		//this variable doesn't /need/ to exist, but I prefer it to writing while(true) loops
		while(!line_done){	//this loops while we work on an individual line in the textbox

			if(curpos >= (int)text.length()){		//if there's nothing left to write at all we're done
				donedrawing = true;
				break;
			}
			if((cursor >= 0)&&(curpos >= cursor)){	//or if we've moved past our current cursor we're done
				donedrawing = true;
				break;
			}

			if(isspace(text.at(curpos))){	//if the current character is a space, check if it fits on the line - if not, discard it	
				if((linewidth + GetCharWidth(' ')+ kerning) > temp.w){
						//get rid of the space so it doesn't indent the next line
					break;
				}
				linewidth += (GetCharWidth(' ')+ kerning);
				curpos++;
				continue;		//nothing below this affects spaces so just go back to the top
			}

			int nextlen = GetNextWordLength(text.substr(curpos));	
			int nextwidth = GetStringWidth(text.substr(curpos,nextlen),kerning);

			//check that the next word fits
			if((linewidth + nextwidth) < temp.w){	//if the word fits
			//	printf("%i \n", linewidth+nextwidth);
				bool wordbyword = false;		//maybe you want to draw word by word idk but here's the code to do it
				if(wordbyword){
					linewidth += nextwidth;
					curpos += nextlen;
				}else{
					linewidth += (GetCharWidth(text.at(curpos)) + kerning);
					curpos ++;
				}
			}else{									//if the word does not fit
				line_done = true;
				break;
			}
		}

		if(drawnlines >= curline){
			if(curpos == start)
				line = text.substr(start);
			else
				line = text.substr(start,curpos-start);
			if(justification == JUSTIFY_RIGHT){
				temp.x = temp.w - linewidth;	//this looks silly with scrolling on
			}
			DrawTextLine(line,temp,kerning);
			temp.y += 10;
		}
		linewidth = 0;		//reset line size to 0	
		drawnlines++;
	}
	//scrolling up and down for longer textboxes
	if(vscroll > 0 && donewriting){			//if the textbox has vertical scrolling enabled
		if(InputPressed(PPINPUT_UP)){
			if(curline > 0)
				curline--;
		}
		
		if(InputPressed(PPINPUT_DOWN)){
			if((curline >= 0) && (curline+linect <= drawnlines))
				curline++;
		}
	}

}
	
void DrawTextLine(string msg,SDL_Rect location, int kerning){
//	DrawRect(location,&uiCamera,pCol_DarkGray);


	//draws a line of text... should probably be renamed...
	if(msg.empty()) return;

	if(useFontSprite){
		Vec2i pos;
		SetVec2i(pos,location.x,location.y);
		for(int i = 0; i < (int)msg.length(); i++){
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