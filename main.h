#include <stdlib.h>
#include "SDL.h"

#include "graphics.h"
#include "settings.h"
#include "ui.h"
#include "pmath.h"
#include "pfile.h"
#include "player.h"
#include "camera.h"
#include "tile.h"
#include "dialogue.h"
#include "level.h"
#include "audio.h"
#include "combat.h"

static enum{
	MAIN_MENU,
	OPTIONS,
	OVERWORLD,
	COMBAT,				
	DIALOGUE_OVERLAY,	//Maybe unnecessary?
	DIALOGUE_FULL,
	DATE			
}_GameState; 
 
static bool _GamePause;
static bool _DrawPause;
static bool _OverworldPause;
static bool _BattlePause;