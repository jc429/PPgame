#include "party.h"
#include "graphics.h"
#include "gamestate.h"

extern Camera uiCamera;

vector<struct Menu_T*> _PartyMenuStack;

#define MAX_PARTY 16
#define NUMANIMS 8 //number of animations each party sprite can have (prob 1 per status)

static int _PartyMembers;
//Player Party[MAX_PARTY];

void DrawPartyBG();
void DrawPartyMembers();

static Animation *partybg;
static Sprite *_PanelSpr;
static Sprite *_PartySpr;
static Animation *_PartyAnims[MAX_PARTY][NUMANIMS];

static struct PartyCursor_T{
	int selection;
	Vec2i location;
	Animation *anim;
}PartyCursor;


void InitParty(){
	partybg = LoadAnimation(LoadSprite("sprites/grid.png",320,240,1),0,0,1,1);
	_PanelSpr = LoadSprite(SPATH_PANEL_DEF,2,2,3);
	_PartySpr = LoadSprite("sprites/partyicons.png",20,20,4);
	_PartyAnims[0][0] = LoadAnimation(_PartySpr,0,0,1,1);
}

void PartyView::Update(){

}

void PartyView::Draw(){
	DrawPartyBG();
	DrawPartyMembers();
}

void DrawPartyBG(){
	Vec2i loc = {0,0};
	DrawAnimation(partybg,loc,&uiCamera);
	SDL_Rect rect;
	for(int i = 0; i < MAX_PARTY>>2; i++){
		SetRect(rect,14 + i*76,10,60,20);
		DrawPanel(rect,_PanelSpr);
		rect.y += 32;
		DrawPanel(rect,_PanelSpr);
		rect.y += 32;
		DrawPanel(rect,_PanelSpr);
		rect.y += 32;
		DrawPanel(rect,_PanelSpr);
	}
}

void DrawPartyMembers(){
	for(int i = 0; i < MAX_PARTY>>2; i++){
		Vec2i loc = {14 + i*76,10};
		DrawAnimation(_PartyAnims[0][0],loc,&uiCamera);
		loc.y += 32;
		DrawAnimation(_PartyAnims[0][0],loc,&uiCamera);
		loc.y += 32;
		DrawAnimation(_PartyAnims[0][0],loc,&uiCamera);
		loc.y += 32;
		DrawAnimation(_PartyAnims[0][0],loc,&uiCamera);
	}
}