#ifndef _INPUT_
#define _INPUT_


#include "SDL.h"
#include "pmath.h"
#include "ptext.h"


//Game inputs 
#define PPKEY_LEFT		SDL_SCANCODE_A
#define PPKEY_RIGHT		SDL_SCANCODE_D
#define PPKEY_UP		SDL_SCANCODE_W
#define PPKEY_DOWN		SDL_SCANCODE_S
#define PPKEY_LEFT2		SDL_SCANCODE_LEFT
#define PPKEY_RIGHT2	SDL_SCANCODE_RIGHT
#define PPKEY_UP2		SDL_SCANCODE_UP
#define PPKEY_DOWN2		SDL_SCANCODE_DOWN
#define PPKEY_A			SDL_SCANCODE_SPACE
#define PPKEY_B			SDL_SCANCODE_LSHIFT
#define PPKEY_X			SDL_SCANCODE_Z
#define PPKEY_Y			SDL_SCANCODE_X
#define PPKEY_START		SDL_SCANCODE_RETURN

#define PPKEY_DEBUG1	SDL_SCANCODE_1
#define PPKEY_DEBUG2	SDL_SCANCODE_2

//debug pause idk 
#define PPKEY_PAUSE SDL_SCANCODE_P

#define PPINPUT_LEFT	1<<7
#define PPINPUT_RIGHT	1<<6
#define PPINPUT_UP		1<<5
#define PPINPUT_DOWN	1<<4
#define PPINPUT_A		1<<3
#define PPINPUT_B		1<<2
#define PPINPUT_X		1<<1
#define PPINPUT_Y		1
#define PPINPUT_START	1<<8
#define PPINPUT_SELECT	1<<9
#define PPINPUT_ESC		1<<15

#define PPINPUT_DEBUG1	1<<13
#define PPINPUT_DEBUG2	1<<14

#define PPINPUT_FACE_ANY (PPINPUT_A|PPINPUT_B|PPINPUT_X|PPINPUT_Y)
#define PPINPUT_DIR_ANY (PPINPUT_UP|PPINPUT_DOWN|PPINPUT_LEFT|PPINPUT_RIGHT)

#define PPINPUT_TEXT_ADVANCE (PPINPUT_A|PPINPUT_B)


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

struct InputNode {
  Uint16 input;
  InputNode *prev;
};

void DeleteInputNode(InputNode *node,int steps);




bool InputPressed(Uint16 input,InputNode *node = NULL);
bool InputReleased(Uint16 input,InputNode *node = NULL);
bool InputBuffered(InputNode *input, int button, int buf);

#endif