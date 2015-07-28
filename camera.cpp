#include "camera.h"


void InitCamera(Camera *c,int w, int h, bool tracks, OverworldEnt *targ, SDL_Rect *tr, SDL_Rect *sr){
	c->viewport.w = w;
	c->viewport.h = h;
	c->trackTarget = tracks;
	c->target = targ;
	SetRect(c->trackingRect, tr);
	SetRect(c->smoothingRect, sr);
	c->smoothspeed = 1;
	if(c->target != NULL)
		c->position = targ->worldposition;
}

//Maybe you want to move the camera during your game - Do that here
void UpdateCamera(Camera *c){
	if(c->trackTarget){
		if(c->hardTracking)
			c->position = c->target->worldposition;
		else{
		

		//	printf("%i \n",c->target->worldposition.y-c->position.y);
			//tracking rect
			if(c->target->worldposition.x >= c->position.x + c->trackingRect.x + (c->trackingRect.w>>1))		//target too far east
				c->position.x = c->target->worldposition.x - c->trackingRect.x - (c->trackingRect.w>>1);
			if(c->target->worldposition.x <= c->position.x + c->trackingRect.x - (c->trackingRect.w>>1))		//target too far west
				c->position.x = c->target->worldposition.x - c->trackingRect.x + (c->trackingRect.w>>1);
			if(c->target->worldposition.y >= c->position.y + c->trackingRect.y + (c->trackingRect.h>>1))		//target too far south
				c->position.y = c->target->worldposition.y - c->trackingRect.y - (c->trackingRect.h>>1);
			if(c->target->worldposition.y <= c->position.y + c->trackingRect.y - (c->trackingRect.h>>1))		//target too far north
				c->position.y = c->target->worldposition.y - c->trackingRect.y + (c->trackingRect.h>>1);
			
				//smoothing rect
			if(c->target->moving == false){
				if(c->target->worldposition.x > c->position.x + c->smoothingRect.x + (c->smoothingRect.w>>1))	//target too far east
					c->position.x += c->smoothspeed;
				if(c->target->worldposition.x < c->position.x + c->smoothingRect.x  - (c->smoothingRect.w>>1))	//target too far west
					c->position.x -= c->smoothspeed;
				if(c->target->worldposition.y > c->position.y + c->smoothingRect.y  + (c->smoothingRect.h>>1))	//target too far south
					c->position.y += c->smoothspeed;
				if(c->target->worldposition.y < c->position.y + c->smoothingRect.y  - (c->smoothingRect.h>>1))	//target too far north
					c->position.y -= c->smoothspeed;
			}
		}
	}

	c->viewport.x = c->position.x - (c->viewport.w>>1);
	c->viewport.y = c->position.y - (c->viewport.h>>1);
}