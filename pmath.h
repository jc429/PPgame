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

static bool operator == (Vec2i a, Vec2i b){
	return ((a.x == b.x)&&(a.y == b.y));
}


typedef struct Vec2d_T{
	double x;
	double y;
	operator Vec2i() const { 
		Vec2i vi; 
		vi.x = (int)x;
		vi.y = (int)y; 
		return vi;
	}
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
static Vec2d operator * (Vec2d a, Vec2d b){
	Vec2d c;
	c.x = a.x * b.x;
	c.y = a.y * b.y;
	return c;
}
static Vec2d operator / (Vec2d a, Vec2d b){
	Vec2d c;
	c.x = a.x / b.x;
	c.y = a.y / b.y;
	return c;
}



static bool operator == (Vec2d a, Vec2d b){
	return ((a.x == b.x)&&(a.y == b.y));
}

void SetVec2i(Vec2i &v, int x, int y);
void SetVec2i(Vec2i &dest, Vec2i src);
void SetRect(SDL_Rect &dest, Vec2i *src);

void SetRect(SDL_Rect &r, int x, int y, int w = 0, int h = 0);
void SetRect(SDL_Rect &dest, SDL_Rect *src);

inline bool PointInRect(int x, int y, SDL_Rect rect);
inline bool PointOnRect(int x, int y, SDL_Rect rect);
inline bool RectOverlap(SDL_Rect a, SDL_Rect b);
bool RectTouch(SDL_Rect a, SDL_Rect b);
inline bool RectFullyInRect(SDL_Rect a, SDL_Rect b);
inline bool RectInRect(SDL_Rect a, SDL_Rect b);

int RandomInt(int min, int max);
int RandomIntInclusive(int min, int max);

bool Toggle(bool b);

#endif