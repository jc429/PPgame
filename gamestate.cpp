#include "gamestate.h"

#include "world.h"
#include "graphics.h"
//#include "combat.h"
using std::vector;

GameScene *_CurrentScene;
vector<GameScene*> _SceneStack;
GameState _GameState;

extern Camera uiCamera;
extern OverworldCharacter *_Player;


GameScene *CurrentScene(){
	return _SceneStack.back();
}

GameState GetCurrentState(){
	return _SceneStack.back()->type;
}

Overworld::Overworld(){
	type = GS_OVERWORLD;

	LoadLevel();

	_SceneStack.push_back(this);
}

Overworld::~Overworld(){
	_SceneStack.pop_back();
}

StatusScreen::StatusScreen(){
	this->bg = LoadAnimation(LoadSprite("sprites/bg-anim.png",8,8,4),0,0,8);
	this->type = GS_STATUS;
	Sprite *panelspr = LoadSprite("sprites/menu3.png",2,2,3); 

	/*SDL_Rect r = {4,34,44,33};
	Panel *p = new Panel(r,panelspr);
	this->UIcontents.push_back(p);
	*/
	SDL_Rect animrect = {4,4,32,32};
	AnimPanel *ap = new AnimPanel(animrect,panelspr);
	Animation *a = _Player->chardata->portrait;
	Vec2i loc(16,32);
	ap->AddAnimPos(a,loc);
	this->UIcontents.push_back(ap);

	SDL_Rect namerect = {42,4,92,13};
	TextPanel *namep = new TextPanel(namerect,panelspr);
	Textbox *t = new Textbox();
	LoadTextbox(t,1,namerect,2);
	t->SetText(_Player->chardata->name,0);
	namep->textboxes.push_back(t);
	this->UIcontents.push_back(namep);

	SDL_Rect affrect = {GAME_RES_X-20,4,16,16};
	AnimPanel *affp = new AnimPanel(affrect,panelspr);
/*	Animation *affanim = _Player->animlist[_Player->animation][_Player->direction];
	Vec2i affloc(8,8);
	affp->AddAnimPos(affanim,affloc);*/
	this->UIcontents.push_back(affp);

	SDL_Rect inforect = {4,42,32,16};
	AnimPanel *infop = new AnimPanel(inforect,panelspr);
	this->UIcontents.push_back(infop);

	SDL_Rect luvrect = {GAME_RES_X-20,26,16,66};
	AnimPanel *luvp = new AnimPanel(luvrect,panelspr);
/*	Animation *luvanim = _Player->animlist[_Player->animation][_Player->direction];
	Vec2i luvloc(8,8);
	luvp->AddAnimPos(luvanim,luvloc);*/
	this->UIcontents.push_back(luvp);

	SDL_Rect statrect = {42,23,92,93};
	TextPanel *statp = new TextPanel(statrect,panelspr);
	for(int i = 0; i < 10; i++){
		statp->AddTextbox("testo");
	}
	this->UIcontents.push_back(statp);

	_SceneStack.push_back(this);
}
StatusScreen::~StatusScreen(){
	_SceneStack.pop_back();
}
