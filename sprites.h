#ifndef _SPRITES_
#define _SPRITES_

#include "sprite_paths.h"
#include "global.h"
#include "pmath.h"
#include "pdirection.h"

typedef struct Sprite_T{
	SDL_Texture *image;			/*pointer to the actual image in memory*/
	char filename[40];			/*the name of the file, keep them short*/
	int w, h;						/*the width and height of the frames of the sprites, not the file itself*/
	int framesperline;			/*default is 16*/
	//int color1,color2,color3;	/*if the file was palette swapped these are the colors that were changed*/

	int s_offset_x;				/*x and y offsets, where to draw the sprite from*/
	int s_offset_y;

	int used;						/*used by the maintanence functions to keep track of how many times a single sprite is being used*/
}Sprite;

typedef struct Animation_T{
	Sprite *sprite;
	int curFrame;
	int seed;
	int length;
	bool playing;
	bool looping;

	Vec2i center;
	int rotation;
	Vec2i mirror;

	void (*onFinish)();	//finish function called when an animation reaches its end (non-looping) or enters a new cycle (looping)
	int delay; //amount of draw frames to wait before updating to next frame - higher number = slower
}Animation;

typedef struct AnimPos_T{		//an animation and vec2i paired together, useful for some things probably
	Animation *anim;
	Vec2i position;
}AnimPos;



void InitSpriteList();
Sprite *LoadSprite(char *filename,int sizex, int sizey, int fpl, int off_x = 0, int off_y = 0);
Sprite *GetSprite(char *filename);
void FreeSprite(Sprite *spr);

Animation *LoadAnimation(Sprite *spr, int curFrame, int seed, int len, bool play = true, bool loop = true, int delay = FRAMESPERDRAW, void(*finish)() = NULL);
void FreeAnimation(Animation *a);

int AdvanceFrame(Sprite *spr, int frame);
void AdvanceAnimFrame(Animation *a);

void DrawTile(Vec2i pos);
void DrawFacingCursor(Vec2i pos);
void DrawPanel(SDL_Rect rect, Sprite *spr);


#endif