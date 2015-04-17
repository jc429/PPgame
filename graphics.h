#ifndef _GRAPHICS_
#define _GRAPHICS_

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

#include "sprites.h"
#include "global.h"
#include "pmath.h"
#include "camera.h"

#define COL_MASK_R 0xFF000000
#define COL_MASK_G 0x00FF0000
#define COL_MASK_B 0x0000FF00
#define R_SHIFT 24
#define G_SHIFT 16
#define B_SHIFT 8

#define pCol_White		0xFEFEFE00
#define pCol_Black		0x01010100
#define pCol_DarkGray	0x22222200
#define pCol_Red		0xDD333300
#define pCol_Green		0x33DD3300
#define pCol_Blue		0x3333DD00

void InitWindow();

void RenderCurrentFrame();
void NextFrame();
void FrameDelay(Uint32 delay);

void DrawRect(SDL_Rect *rect, Camera *c, Uint32 color = pCol_White);
void DrawRectFill(SDL_Rect *rect, Camera *c, Uint32 color = pCol_White);

void InitTG();
void TestGraphics(int x);


#endif