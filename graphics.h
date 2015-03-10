#ifndef _GRAPHICS_
#define _GRAPHICS_

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

#include "sprites.h"
#include "settings.h"
#include "pmath.h"
#include "camera.h"





void InitWindow();

void RenderCurrentFrame();
void NextFrame();
void FrameDelay(Uint32 delay);

void DrawRect(SDL_Rect *rect, Camera *c);

void InitTG();
void TestGraphics(int x);


#endif