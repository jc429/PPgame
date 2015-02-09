#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "settings.h"



typedef struct Sprite_T
{
  SDL_Surface *image;		/*pointer to the actual image in memory*/
  char filename[40];			/*the name of the file, keep them short*/
  int w, h;					/*the width and height of the frames of the sprites, not the file itself*/
  int framesperline;			/*default is 16*/
  //int color1,color2,color3;		/*if the file was palette swapped these are the colors that were changed*/
  int used;					/*used by the maintanenc functions to keep track of how many times a single sprite is being used*/
}Sprite;

/*typedef struct BGLayer_T{
	double movescale;		//how much this layer moves in relation to the platforms - 0 is no motion relative to the camera, 1 is no motion relative to the game level
	int x,y,w,h;
	int offx,offy;
	char* layerpath;		//location of the bg layer image
	SDL_Texture *layerIMG;

} BGLayer;*/

void InitWindow();

void NextFrame();
void FrameDelay(Uint32 delay);


void UpdateCamera();

void TestGraphics(int x);
