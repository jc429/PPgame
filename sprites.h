#ifndef _SPRITES_
#define _SPRITES_

#include "pmath.h"

typedef struct Sprite_T{
	SDL_Texture *image;			/*pointer to the actual image in memory*/
	char filename[40];			/*the name of the file, keep them short*/
	int w, h;						/*the width and height of the frames of the sprites, not the file itself*/
	int framesperline;			/*default is 16*/
	//int color1,color2,color3;	/*if the file was palette swapped these are the colors that were changed*/
	int used;						/*used by the maintanence functions to keep track of how many times a single sprite is being used*/
}Sprite;


Sprite *LoadSprite(char *filename,int sizex, int sizey, int fpl);

void DrawTile(Vec2i pos);
void DrawCursor(Vec2i pos);

#endif