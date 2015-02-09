#include "SDL.h"

typedef struct {	/**< Mouse struct, tells us the state of the mouse on every frame*/
	int x;
	int y;
	int prev;
	Uint8 clicked;		/* presently down, last frame up */
	Uint8 held;			/* presently down, last frame down */
	Uint8 released;		/* presently up, last frame down */
} Mouse;

void UpdateMouse();