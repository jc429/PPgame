#ifndef _CAMERA_
#define _CAMERA_
#include "pmath.h"
#include "player.h"

typedef struct Camera_T{
	Vec2i position;
	SDL_Rect viewport;
	
	bool trackTarget;		//does this camera follow a specified target in the game?
	OverworldEnt *target;	//overworldent for now, hopefully i'll change this
	bool hardTracking;		//if you want the camera to literally follow the target every pixel - absolutely precise, but looks bad
	SDL_Rect trackingRect;	//if you want the camera to only move when the target leaves a predefined border - much prettier	
	SDL_Rect smoothingRect;	//helps the camera smoothly reach the target rather than jerking to it 
	int smoothspeed;		//if the target is outside the smoothing rect but inside the tracking rect, 
							//the camera will move incrementally each frame until the target is within both rects;
	
} Camera;

void InitCamera(Camera *c,int w, int h, bool tracks = false, OverworldEnt *targ = NULL, SDL_Rect *tr = NULL, SDL_Rect *sr = NULL);
void UpdateCamera(Camera *c);

int DrawSprite(Sprite* spr, int frame, Vec2i pos, Camera *c);
int DrawPartialSprite(Sprite* spr, int frame, SDL_Rect subrect, Vec2i pos, Camera *c);

void DrawAnimation(Animation *anim, Vec2i pos, Camera *c = NULL);
void DrawPartialAnimation(Animation *anim, SDL_Rect subrect, Vec2i pos, Camera *c = NULL);
void DrawBGAnimation(Animation *anim,Camera *c = NULL);
void DrawAnimFrame(Animation *anim, Vec2i pos, Camera *c);	//identical to drawanimation but does not advance the frame


#endif