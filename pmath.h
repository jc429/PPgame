#ifndef _PMATH_
#define _PMATH_

#include "SDL.h"

//Custom 2 dimensional vector type (feat. operators)
typedef struct Vec2_T{
	int x;
	int y;
}Vec2i;

static Vec2i operator + (Vec2i a, Vec2i b){
	Vec2i c;
	c.x = a.x + b.x;
	c.y = a.y + b.y;
	return c;
}
static Vec2i operator - (Vec2i a, Vec2i b){
	Vec2i c;
	c.x = a.x - b.x;
	c.y = a.y - b.y;
	return c;
}
static Vec2i operator * (Vec2i a, Vec2i b){
	Vec2i c;
	c.x = a.x * b.x;
	c.y = a.y * b.y;
	return c;
}
static Vec2i operator / (Vec2i a, Vec2i b){
	Vec2i c;
	c.x = a.x / b.x;
	c.y = a.y / b.y;
	return c;
}

bool PointInRect(int x, int y, SDL_Rect rect);
bool RectOverlap(SDL_Rect a, SDL_Rect b);
bool RectInRect(SDL_Rect a, SDL_Rect b);


#endif