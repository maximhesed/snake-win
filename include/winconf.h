#ifndef WINCONF_H
#define WINCONF_H

#include <stdbool.h>
#include <windows.h>
#include <conio.h>

#define WIN_SIZE_X 60
#define WIN_SIZE_Y 28
#define WIN_POS_X 300
#define WIN_POS_Y 150

/* Thank you, HaSeeB MiR. */
enum fg_colors {
    FG_BLACK,
    FG_BLUE,
    FG_GREEN,
    FG_CYAN,
    FG_RED,
    FG_MAGENTA,
    FG_BROWN,
    FG_LIGHTGRAY,
    FG_GRAY,
    FG_LIGHTBLUE,
    FG_LIGHTGREEN,
    FG_LIGHTCYAN,
    FG_LIGHTRED,
    FG_LIGHTMAGENTA,
    FG_YELLOW,
    FG_WHITE
};

enum bg_colors {
    BG_NAVYBLUE = 16,
    BG_GREEN = 32,
    BG_TEAL = 48,
    BG_MAROON = 64,
    BG_PURPLE = 80,
    BG_OLIVE = 96,
    BG_SILVER = 112,
    BG_GRAY = 128,
    BG_BLUE = 144,
    BG_LIME = 160,
    BG_CYAN = 176,
    BG_RED = 192,
    BG_MAGENTA = 208,
    BG_YELLOW = 224,
    BG_WHITE = 240
};

HANDLE h;
HWND hWnd;

#endif /* WINCONF_H */
