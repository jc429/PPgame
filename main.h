#include <stdlib.h>
#include "SDL.h"

#include "graphics.h"
#include "global.h"
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
#include "gamestate.h"
#include "world.h"
 
 
static bool _GamePause;
static bool _DrawPause;
static bool _OverworldPause;
static bool _BattlePause;