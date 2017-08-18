#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <limits.h>

extern unsigned SCREEN_W;
extern unsigned SCREEN_H;

#define BOARD_DEPTH 40
#define BOARD_PADDING 45
#define BOARD_SIZE (SCREEN_H - BOARD_DEPTH - BOARD_PADDING)

#define STAR_SIZE 8 
#define DARK_COLOR al_map_rgb(76, 60, 22)
#define LIGHT_COLOR al_map_rgb(223, 189, 109)
#define INDICATOR_COLOR al_map_rgb(50, 131, 252)

// used for permanent board state
#define NOPLAYER 0
#define SLATE 1
#define SHELL 2
// used for temporary board state
#define LIBERTY 3
#define SLATE_CHECKED 4
#define SHELL_CHECKED 5
#define INVALID UINT_MAX

#endif
