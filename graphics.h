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
void InitWorld();

void NextFrame();
void FrameDelay(Uint32 delay);

void InitSpriteList();





void InitTG();
void TestGraphics(int x);


#endif