#ifndef _PMATH_
#define _PMATH_

#include "SDL.h"

//Custom 2 dimensional vector type (feat. operators)
typedef struct Vec2i_T{
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

typedef struct Vec2d_T{
	int x;
	int y;
}Vec2d;

static Vec2d operator + (Vec2d a, Vec2d b){
	Vec2d c;
	c.x = a.x + b.x;
	c.y = a.y + b.y;
	return c;
}
static Vec2d operator - (Vec2d a, Vec2d b){
	Vec2d c;
	c.x = a.x - b.x;
	c.y = a.y - b.y;
	return c;
}



inline bool PointInRect(int x, int y, SDL_Rect rect);
inline bool PointOnRect(int x, int y, SDL_Rect rect);
inline bool RectOverlap(SDL_Rect a, SDL_Rect b);
inline bool RectTouch(SDL_Rect a, SDL_Rect b);
inline bool RectFullyInRect(SDL_Rect a, SDL_Rect b);
inline bool RectInRect(SDL_Rect a, SDL_Rect b);
bool Toggle(bool b);

#endif