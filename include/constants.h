#ifndef CONSTANTS_H
#define CONSTANTS_H

extern unsigned SCREEN_W;
extern unsigned SCREEN_H;

#define BOARD_DEPTH 40
#define BOARD_PADDING 45
#define BOARD_SIZE (SCREEN_H - BOARD_DEPTH - BOARD_PADDING)

#define STAR_SIZE 7
#define DARK_COLOR al_map_rgb(76, 60, 22)
#define LIGHT_COLOR al_map_rgb(223, 189, 109)

#define SLATE 1
#define SHELL 2

#endif
