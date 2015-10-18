#include "ui.h"
#include "graphics.h"


void TextPanel::Draw(){
	SDL_Rect rect = GetPosition();
	DrawPanel(rect,panel);	

//	this->text_offset_y++;

	
};

void TextPanel::AddTextbox(char *contents){	//adds a textbox at the top of the panel or underneath the most recently added one
	SDL_Rect location;
	Textbox *t = new Textbox(); 
	location.x = this->position.x;
	location.w = this->position.w;
	location.h = 13;
	if(this->textboxes.empty()){
		location.y = this->position.y;
	}else{
		location.y = this->textboxes.back()->box.y + 13;
	}
	LoadTextbox(t,1,location,2);
	t->SetText(contents,0);
	this->textboxes.push_back(t);
}

void AnimPanel::Draw(){
	SDL_Rect rect = GetPosition();
	DrawPanel(rect,panel);	
	for(int i = 0; i < (int)animations.size(); i++){
		Vec2i pos = animations.at(i).position;
		pos.x += rect.x;
		pos.y += rect.y;
		DrawAnimation(animations.at(i).anim,pos);
	}
};

void AnimPanel::AddAnimPos(Animation *a, Vec2i p){
	AnimPos ap;
	ap.anim = a;
	ap.position = p;
	this->animations.push_back(ap);
}