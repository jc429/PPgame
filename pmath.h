#ifndef _PMATH_
#define _PMATH_

#include "SDL.h"
#include <math.h>

//Custom 2 dimensional int vector type (feat. operators)
typedef struct Vec2i{
	int x;
	int y;

	Vec2i(int x = 0, int y = 0){
		this->x = x;	this->y = y;
	};
	float Length(){
		return sqrt((float)(x * x) + (float)(y * y));
	};
/*	operator Vec2f() const { 
		Vec2f vf;		vf.x = x;		vf.y = y;		return vf;
	};*/
}Vec2i;

static Vec2i operator + (Vec2i a, Vec2i b){
	Vec2i c;		c.x = a.x + b.x;		c.y = a.y + b.y;		return c;
}
static Vec2i operator - (Vec2i a, Vec2i b){
	Vec2i c;		c.x = a.x - b.x;		c.y = a.y - b.y;		return c;
}
static Vec2i operator * (Vec2i a, Vec2i b){
	Vec2i c;		c.x = a.x * b.x;		c.y = a.y * b.y;		return c;
}
static Vec2i operator / (Vec2i a, Vec2i b){
	Vec2i c;		c.x = a.x / b.x;		c.y = a.y / b.y;		return c;
}

static bool operator == (Vec2i a, Vec2i b){
	return ((a.x == b.x)&&(a.y == b.y));
}


//Custom 2 dimensional float vector 
typedef struct Vec2f{
	float x;
	float y;

	Vec2f(float x = 0, float y = 0){
		this->x = x;	this->y = y;
	};
	float Length(){
		return sqrt((x * x) + (y * y));
	};
	operator Vec2i() const { 
		Vec2i vi; 
		vi.x = (int)x;
		vi.y = (int)y; 
		return vi;
	};
}Vec2f;

static Vec2f operator + (Vec2f a, Vec2f b){
	Vec2f c;		c.x = a.x + b.x;		c.y = a.y + b.y;		return c;
}
static Vec2f operator - (Vec2f a, Vec2f b){
	Vec2f c;		c.x = a.x - b.x;		c.y = a.y - b.y;		return c;
}
static Vec2f operator * (Vec2f a, Vec2f b){
	Vec2f c;		c.x = a.x * b.x;		c.y = a.y * b.y;		return c;
}
static Vec2f operator / (Vec2f a, Vec2f b){
	Vec2f c;		c.x = a.x / b.x;		c.y = a.y / b.y;		return c;
}

static bool operator == (Vec2f a, Vec2f b){
	return ((a.x == b.x)&&(a.y == b.y));
}


//Custom 3 dimensional float vector 
typedef struct Vec3f:Vec2f{		//inherit Vec2f so a 3d vector can function as a 2d vector if needed
	float x;
	float y;
	float z;
	Vec3f(float x = 0, float y = 0, float z = 0){
		this->x = x;
		this->y = y;
		this->z = z;
	}
}Vec3f;

static Vec3f operator + (Vec3f a, Vec3f b){
	Vec3f c;
	c.x = a.x + b.x;
	c.y = a.y + b.y;
	c.z = a.z + b.z;
	return c;
}
static Vec3f operator - (Vec3f a, Vec3f b){
	Vec3f c;
	c.x = a.x - b.x;
	c.y = a.y - b.y;
	c.z = a.z - b.z;
	return c;
}

static bool operator == (Vec3f a, Vec3f b){
	return ((a.x == b.x)&&(a.y == b.y)&&(a.z == b.z));
}




void SetVec2i(Vec2i &v, int x, int y);
void SetVec2i(Vec2i &dest, Vec2i src);

void SetVec2f(Vec2f &v, float x, float y);
void SetVec2f(Vec2f &dest, Vec2f src);

void SetVec3f(Vec3f &v, float x, float y, float z);
void SetVec3f(Vec3f &dest, Vec3f src);

void SetRect(SDL_Rect &dest, Vec2i src);
void SetRect(SDL_Rect &r, int x, int y, int w = 0, int h = 0);
void SetRect(SDL_Rect &dest, SDL_Rect src);

Vec2f Vec2iToVec2f(Vec2i v);
Vec2f VecScale(Vec2f v, float scale);
Vec2f Normalize(Vec2f v);	//sets a vec to a magnitude of 1

bool IsEqual(float a, float b);
bool IntsWithinRange(int a, int b, int range);
bool NumsWithinRange(float a, float b, float range);

float Remainder(float dividend, float divisor);

int Max(int a, int b);
float Max(float a, float b);
int Min(int a, int b);
float Min(float a, float b);

namespace Collision{
	inline bool PointInRect(int x, int y, SDL_Rect rect);
	inline bool PointOnRect(int x, int y, SDL_Rect rect);
	inline bool RectOverlap(SDL_Rect a, SDL_Rect b);
	bool RectTouch(SDL_Rect a, SDL_Rect b);
	inline bool RectFullyInRect(SDL_Rect a, SDL_Rect b);
	inline bool RectInRect(SDL_Rect a, SDL_Rect b);
}

namespace Random{
	int RandomInt(int min, int max);
	int RandomIntInclusive(int min, int max);
}
bool ToggleBool(bool b);

#endif