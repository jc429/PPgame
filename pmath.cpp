#include "pmath.h"
#include <random>

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


int RandomInt(int min, int max){ //returns a random integer, from min to max (non-inclusive)
	double r;		// a decimal between 0 and 1
	int range;		//the range between the two numbers
	int random;		//our random number

	range = (max) - min;

	r = ((double) rand() / (RAND_MAX));

	random = (r * range) + min;

	return random; 
}

int RandomIntInclusive(int min, int max){//returns a random integer, from min to max (inclusive)
	return RandomInt(min, max+1);
}

bool Toggle(bool b){
	if(b != 0)
		b = 0;
	else
		b = 1;
	return b;
}