#ifndef _SPRITES_
#define _SPRITES_

#include "sprite_paths.h"
#include "global.h"
#include "pmath.h"

typedef struct Sprite_T{
	SDL_Texture *image;			/*pointer to the actual image in memory*/
	char filename[40];			/*the name of the file, keep them short*/
	int w, h;						/*the width and height of the frames of the sprites, not the file itself*/
	int framesperline;			/*default is 16*/
	//int color1,color2,color3;	/*if the file was palette swapped these are the colors that were changed*/
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

#define NUM_ANIM_DIRS 8
typedef enum AnimDir{
	ANIM_DIR_S = 0,
	ANIM_DIR_SW = 1,
	ANIM_DIR_W = 2,
	ANIM_DIR_NW = 3,
	ANIM_DIR_N = 4,
	ANIM_DIR_NE = 5,
	ANIM_DIR_E = 6,
	ANIM_DIR_SE = 7,
};

void InitSpriteList();
Sprite *LoadSprite(char *filename,int sizex, int sizey, int fpl);
void FreeSprite(Sprite *spr);

Animation *LoadAnimation(Sprite *spr, int curFrame, int seed, int len, bool play, bool loop = true, int delay = FRAMESPERDRAW, void(*finish)() = NULL);
void FreeAnimation(Animation *a);

void AdvanceAnimFrame(Animation *a);

void DrawTile(Vec2i pos);
void DrawFacingCursor(Vec2i pos);
void DrawPanel(SDL_Rect rect, Sprite *spr);


#endif