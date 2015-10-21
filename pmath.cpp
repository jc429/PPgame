#include "pmath.h"
#include <random>		// eventually build my own seeded RNG 

//Checks if a point is within a Rectangle (excluding touching edges)
inline bool PointInRect(int x, int y, SDL_Rect rect){
	return((x>rect.x)&&(x<(rect.x+rect.w))
			&&(y>rect.y)&&(y<(rect.y+rect.h)));
}

//Checks if a point is touching a Rectangle (including touching edges)
inline bool PointOnRect(int x, int y, SDL_Rect rect){
	return((x>=rect.x)&&(x<=(rect.x+rect.w))
			&&(y>=rect.y)&&(y<=(rect.y+rect.h)));
}

//Checks if two Rectangles are overlapping (more than tangentially)
inline bool RectOverlap(SDL_Rect a, SDL_Rect b){
	return(((a.x+a.w) > b.x) && (a.x < (b.x+b.w)) &&
			(a.y < (b.y+b.h)) && ((a.y+a.h) > b.y));
}

//Checks if two Rectangles are touching (including tangentially)
bool RectTouch(SDL_Rect a, SDL_Rect b){
	return(((a.x+a.w) >= b.x) && (a.x <= (b.x+b.w)) &&
			(a.y <= (b.y+b.h)) && ((a.y+a.h) >= b.y));
}

//Checks if a Rectangle is completely within another Rectangle (touching edges NOT okay)
inline bool RectFullyInRect(SDL_Rect a, SDL_Rect b){
	return ((a.x>b.x)&&((a.x+a.w)<(b.x+b.w))&&
			(a.y>b.y)&&((a.y+a.h)<(b.y+b.h)));
}

//Checks if a Rectangle is completely within another Rectangle (touching edges okay)
inline bool RectInRect(SDL_Rect a, SDL_Rect b){
	return ((a.x>=b.x)&&((a.x+a.w)<=(b.x+b.w))&&
			(a.y>=b.y)&&((a.y+a.h)<=(b.y+b.h)));
}

void SetVec2i(Vec2i &v, int x, int y){
	v.x = x;
	v.y = y;
}
void SetVec2i(Vec2i &dest, Vec2i src){
	dest.x = src.x;
	dest.y = src.y;
}

void SetVec2f(Vec2f &v, float x, float y){
	v.x = x;
	v.y = y;
}
void SetVec2f(Vec2f &dest, Vec2f src){
	dest.x = src.x;
	dest.y = src.y;
}

void SetVec3f(Vec3f &v, float x, float y, float z){
	v.x = x;
	v.y = y;
	v.z = z;
}
void SetVec3f(Vec3f &dest, Vec3f src){
	dest.x = src.x;
	dest.y = src.y;
	dest.z = src.z;
}

void SetRect(SDL_Rect &r, int x, int y, int w, int h){
	r.x = x;
	r.y = y;
	r.w = w;
	r.h = h;
}

void SetRect(SDL_Rect &dest, SDL_Rect src){
	dest.x = src.x;
	dest.y = src.y;
	dest.w = src.w;
	dest.h = src.h;
}
void SetRect(SDL_Rect &dest, Vec2i src){
	dest.x = src.x;
	dest.y = src.y;
}

///////////////////////////

Vec2f Vec2iToVec2f(Vec2i v){
	Vec2f vf;
	SetVec2f(vf,v.x,v.y);
	return vf;
}

Vec2f VecScale(Vec2f v, float scale){
	v.x *= scale;
	v.y *= scale;
	return v;
}

Vec2f Normalize(Vec2f v){	//sets a vec to a magnitude of 1
	if(IsEqual(v.Length(), 0.0))
		return v;
	Vec2f vn = VecScale(v,1/v.Length());
	return vn;
}

///////////////////////////

bool IsEqual(float a, float b){
#define EPSILON 0.0001
	return (std::fabs(a - b) <= EPSILON);
}

bool IntsWithinRange(int a, int b, int range){	//checks if a is within range of b
	if(std::abs(a - b) < range)
		return true;
	return false;
}

bool NumsWithinRange(float a, float b, float range){	//checks if a is within range of b
	if(std::abs(a - b) <= std::abs(range))
		return true;
	return false;
}

float Remainder(float dividend, float divisor){	//idk if i trust this 
	if(dividend == 0 || divisor == 0)
		return 0;
	if(dividend < 0)
		dividend *= -1;
	if(divisor < 0)
		divisor *= -1;

	while(dividend - divisor > 0){
		dividend -= divisor;
	}
	return dividend;
}


int Max(int a, int b){
	return (a > b ? a : b);
}

float Max(float a, float b){
	return (a > b ? a : b);
}

int Min(int a, int b){
	return (a <= b ? a : b);
}

float Min(float a, float b){
	return (a <= b ? a : b);
}
///////////////////////////

int Random::RandomInt(int min, int max){ //returns a random integer, from min to max (non-inclusive)
	float r;		// a decimal between 0 and 1
	int range;		//the range between the two numbers
	int random;		//our random number

	range = (max) - min;

	r = ((float) rand() / (RAND_MAX));

	random = (r * range) + min;

	return random; 
}

int Random::RandomIntInclusive(int min, int max){//returns a random integer, from min to max (inclusive)
	return RandomInt(min-1, max+1);
}

bool ToggleBool(bool b){
	if(b != 0)
		b = 0;
	else
		b = 1;
	return b;
}