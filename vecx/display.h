#ifndef __DISPLAY_H
#define __DISPLAY_H

/**
 * Data and functions to control rendering with SDL
 */

#include "SDL2/SDL.h"

typedef struct
{
    const uint16_t height;
    const uint16_t width;

    uint16_t scl_factor;
    uint16_t offx;
    uint16_t offy;

    SDL_Window* screen;
    SDL_Renderer* renderer;
    SDL_Texture* overlay;
} Display;

extern Display display;

void open_window(Display* d);

void close_window();

void load_overlay(Display* d, const char* filename);

void render(const Display* d);

void get_pixels(const Display* d, uint8_t* inout_pixels);

void resize(Display* d, int width, int height);

#endif
