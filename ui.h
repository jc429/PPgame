#ifndef _UI_
#define _UI_

#include "SDL.h"
#include "pmath.h"
#include "ptext.h"



//This could apply to anything really; a key, a mouse button, a gamepad button, etc
typedef struct Input_T{
	bool prev;			/* was the button down last frame? */
	bool pressed;		/* presently down, last frame up */
	bool held;			/* presently down, last frame down */
	bool released;		/* presently up, last frame down */
} Input;

typedef struct Mouse_T{	/**< Mouse struct, tells us the state of the mouse on every frame*/
	Vec2i pos;
	Input left;
	Input right;	//Right click doesn't work yet
} Mouse;



void UpdateMouse();
void UpdateInput(Input *input, bool current);

void DrawOverworldUI();



#endif