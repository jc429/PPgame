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

static enum{
	MAIN_MENU,
	OPTIONS,
	OVERWORLD,
	DIALOGUE_OVERLAY,
	DIALOGUE_FULL,
	DATE
}GameState;
 