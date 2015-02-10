#include "pmath.h"

bool PointInRect(int x, int y, SDL_Rect rect){
	return((x>rect.x)&&(x<(rect.x+rect.w))
			&&(y>rect.y)&&(y<(rect.y+rect.h)));
}

bool RectOverlap(SDL_Rect a, SDL_Rect b){
	return(((a.x+a.w) > b.x) && (a.x < (b.x+b.w)) &&
			(a.y < (b.y+b.h)) && ((a.y+a.h) > b.y));
}

bool RectInRect(SDL_Rect a, SDL_Rect b){
	return ((a.x>b.x)&&((a.x+a.w)<(b.x+b.w))&&
			(a.y>b.y)&&((a.y+a.h)<(b.y+b.h)));
}