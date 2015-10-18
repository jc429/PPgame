#include "camera.h"

//temp
#include "graphics.h"
extern Camera mainCamera;

void InitCamera(Camera *c,int w, int h, bool tracks, OverworldEnt *targ, SDL_Rect *tr, SDL_Rect *sr){
	c->viewport.w = w;
	c->viewport.h = h;
	c->trackTarget = tracks;
	c->target = targ;
	if(tr != NULL)
		SetRect(c->trackingRect, *tr);
	if(sr != NULL)
		SetRect(c->smoothingRect, *sr);
	c->smoothspeed = 1;
	if(c->target != NULL)
		c->position = targ->WorldPosition();
}

//Maybe you want to move the camera during your game - Do that here
void UpdateCamera(Camera *c){
	if(c->trackTarget){
		if(c->hardTracking)
			c->position = c->target->WorldPosition();
		else{
			Vec2f targetPos = c->target->WorldPosition();

			/***semi-working code for a camera that focuses ahead of the player: 
			//float targScale = 1.4f*TILE_W;	//we want the camera centered 1.4 tiles ahead of where the player currently is 
			targetPos = targetPos + VecScale(Normalize(Vec2iToVec2f(c->target->facing)),targScale);
			SDL_Rect r = {targetPos.x,targetPos.y,2,2};
			DrawRect(r,&mainCamera);
		//	printf("%i \n",c->target->worldposition.y-c->position.y);
		*******************************/

			//tracking rect
			if(c->target->WorldPosition().x >= c->position.x + c->trackingRect.x + (c->trackingRect.w>>1))		//target too far east
				c->position.x = c->target->WorldPosition().x - c->trackingRect.x - (c->trackingRect.w>>1);
			if(c->target->WorldPosition().x <= c->position.x + c->trackingRect.x - (c->trackingRect.w>>1))		//target too far west
				c->position.x = c->target->WorldPosition().x - c->trackingRect.x + (c->trackingRect.w>>1);
			if(c->target->WorldPosition().y >= c->position.y + c->trackingRect.y + (c->trackingRect.h>>1))		//target too far south
				c->position.y =c->target->WorldPosition().y - c->trackingRect.y - (c->trackingRect.h>>1);
			if(c->target->WorldPosition().y <= c->position.y + c->trackingRect.y - (c->trackingRect.h>>1))		//target too far north
				c->position.y = c->target->WorldPosition().y - c->trackingRect.y + (c->trackingRect.h>>1);
			
				//smoothing rect
			if(c->target->movex == false){
				if(targetPos.x > c->position.x + c->smoothingRect.x + (c->smoothingRect.w>>1))	//target too far east
					c->position.x += c->smoothspeed;
				if(targetPos.x < c->position.x + c->smoothingRect.x  - (c->smoothingRect.w>>1))	//target too far west
					c->position.x -= c->smoothspeed;
			}
			if(c->target->movey == false){
				if(targetPos.y > c->position.y + c->smoothingRect.y  + (c->smoothingRect.h>>1))	//target too far south
					c->position.y += c->smoothspeed;
				if(targetPos.y < c->position.y + c->smoothingRect.y  - (c->smoothingRect.h>>1))	//target too far north
					c->position.y -= c->smoothspeed;
			}
		}
	}

	c->viewport.x = c->position.x - (c->viewport.w>>1);
	c->viewport.y = c->position.y - (c->viewport.h>>1);
}