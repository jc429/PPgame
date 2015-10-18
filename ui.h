#ifndef _UI_
#define _UI_

#include "pmath.h"
#include "sprites.h"
#include "ptext.h"
#include <vector>
using std::vector;

typedef struct UIElement{
public:
	int layer;				//UI elements are drawn in order from lowest layer to highest
	SDL_Rect position;

	UIElement *parent;

	UIElement(){
		this->parent = NULL;
	};
	UIElement(SDL_Rect rect){
		this->parent = NULL;
		this->position = rect;
	};
	UIElement(int x, int y, int w, int h){
		this->parent = NULL;
		this->position.x = x;
		this->position.y = y;
		this->position.w = w;
		this->position.h = h;
	};
	SDL_Rect GetPosition(){
		if(parent != NULL){
			SDL_Rect r;
			SDL_Rect parect = parent->GetPosition();
			r.x = this->position.x + parect.x;
			r.y = this->position.y + parect.y;
			r.w = this->position.w;
			r.h = this->position.h;
			return(r);
		}
		return position;
	};
	virtual void Draw(){};
}UIElement;

typedef struct Panel:UIElement{	//A simple Panel
	Sprite *panel;

	Panel(){};
	Panel(SDL_Rect rect,Sprite *panelspr):UIElement(rect){
		this->panel = panelspr;
	};
	void Draw(){
		SDL_Rect rect = GetPosition();
		DrawPanel(rect,panel);	
	};
}Panel;

typedef struct TextPanel:Panel{ //one or more simple textboxes inside of a panel
	vector<Textbox *>textboxes;		//if textboxes extend outside of this panel, they will not be drawn

	TextPanel(){
		this->text_offset_y = 0;
	};
	TextPanel(SDL_Rect rect,Sprite *panelspr):Panel(rect,panelspr){
		this->text_offset_y = 0;
	};
	void Draw();
	void AddTextbox(char *contents);	//adds a textbox at the top of the panel or underneath the most recently added one
private:
	int text_offset_y;	//used to keep track of where the textboxes should be drawn
}TextPanel;

typedef struct AnimPanel:Panel{	//one or more simple animations inside of a panel
	vector<AnimPos>animations;

	AnimPanel(){};
	AnimPanel(SDL_Rect rect,Sprite *panelspr):Panel(rect,panelspr){};
	void Draw();
	void AddAnimPos(Animation *a, Vec2i p);
}AnimPanel;

#endif