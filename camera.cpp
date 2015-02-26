#include "camera.h"


void InitCamera(Camera *c,int w, int h){

}

//Maybe you want to move the camera during your game - Do that here
void UpdateCamera(Camera *c){
	if(c->trackPlayer)
		c->position = c->target->worldposition;

	c->viewport.x = c->position.x - (c->viewport.w>>1);
	c->viewport.y = c->position.y - (c->viewport.h>>1);
}