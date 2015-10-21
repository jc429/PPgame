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

#define pCol_White			0xFEFEFE00
#define pCol_Black			0x01010100
#define pCol_Gray			0x66666600
#define pCol_DarkGray		0x22222200
#define pCol_Red			0xDD333300
#define pCol_Green			0x33DD3300
#define pCol_Blue			0x3333DD00
#define pCol_Yellow			0xDDDD3300
#define pCol_Magenta		0xDD33DD00
#define pCol_Cyan			0x33DDDD00
#define pCol_DarkRed		0x99222200
#define pCol_DarkGreen		0x22992200
#define pCol_DarkBlue		0x22229900
#define pCol_DarkYellow		0x99992200
#define pCol_LightRed		0xFF888800
#define pCol_LightGreen		0x88FF8800
#define pCol_LightBlue		0x8888FF00
#define pCol_LightYellow	0xFFFF8800

void InitWindow();

namespace Graphics{
	void RenderCurrentFrame();
	void NextFrame();
	void FrameDelay(Uint32 delay);
	//void InitTG();
	//void TestGraphics(int x);
}

namespace Renderer{
	void DrawRect(SDL_Rect rect, Camera *c, Uint32 color = pCol_White);
	void DrawRectFill(SDL_Rect rect, Camera *c, Uint32 color = pCol_White);
	void DrawPixel(int x, int y, Camera *c, Uint32 color = pCol_White);
	void DrawLine(int x1, int y1, int x2, int y2, Camera *c, Uint32 color = pCol_White);

}


#endif