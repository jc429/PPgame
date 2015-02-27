#include <stdlib.h>
#include "SDL.h"

#include "graphics.h"
#include "settings.h"
#include "ui.h"
#include "pmath.h"
#include "player.h"
#include "camera.h"
#include "tile.h"
#include "dialogue.h"
#include "level.h"
#include "audio.h"

static enum{
	MAIN_MENU,
	OPTIONS,
	OVERWORLD,
	COMBAT,				
	DIALOGUE_OVERLAY,	//Maybe unnecessary?
	DIALOGUE_FULL,
	DATE			
}GameState; 
 