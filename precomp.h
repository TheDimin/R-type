// add your external includes to this file instead of to individual .cpp files

#define SCRWIDTH		700
#define SCRHEIGHT		612
// #define FULLSCREEN
// #define ADVANCEDGL	// faster if your system supports it

//DebugModes Disabled in release
#if _DEBUG

//#define PERF_DEBUG
//#define DISABLE_AUTO_SCROLL

//#define VELOCITY_DEBUG

//#define TILE_COORDINATE
//#define COLLISION_DEBUG
#define NO_CLIP // No collision with tiles
//#define NO_COLLISION

//#define STATE_DEBUG
//#define DEBUG_ENEMYCREATION
//#define DEBUG_BULLET_CREATION
#define SHOW_RESPAWN

#define SKIP_MAIN_MENU
//#define OBJECT_DESTRUCTOR
#endif

#include <cinttypes>
#include <cmath>
#include <cstdlib>
#include <cassert>
#include <fstream>
#include <cstdio>

extern "C"
{
#include "glew.h" 
}
#include "gl.h"
#include "wglext.h"

#include "SDL.h"

#include "FreeImage.h"
#include "template.h"
#include "surface.h"

//SIMD Intrinsics headers.
//#include "emmintrin.h"
//#include "immintrin.h"
using namespace Tmpl8;