#ifndef _CAMERA_
#define _CAMERA_
#include "pmath.h"
#include "player.h"

typedef struct Camera_T{
	Vec2i position;
	SDL_Rect viewport;

	bool trackPlayer;
	Player *target;
} Camera;


void UpdateCamera(Camera *c);

int DrawSprite(Sprite* spr, int frame, Vec2i pos,Camera *c);
int AdvanceFrame(Sprite *spr, int frame);

#endif