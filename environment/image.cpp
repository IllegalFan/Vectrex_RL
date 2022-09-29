#include "image.hpp"

extern "C"
{
#include "display.h"
}

#ifndef NDEBUG
#include <iostream>
#endif

using namespace vecx_rl;

#ifndef NDEBUG
/**
 * A SDL_Surface with depth 8 (bit) is automatically created with an empty palette
 * So this function greates a grayscale palette for the Surface
 */
void set_grayscale_palette(SDL_Surface* s)
{
    SDL_Color colors[256];
    int i;

    for (i = 0; i < 256; i++)
    {
        colors[i].r = colors[i].g = colors[i].b = i;
    }

    SDL_SetPaletteColors(s->format->palette, colors, 0, 256);
}
#endif

screenshot_creator::screenshot_creator(std::pair<int, int> target_dimension) : image_resizer(8)
{
    sshot = new uint8_t[display.width * display.height];

    auto [width, height] = target_dimension;

    if (width <= 0 || height <= 0)
    {
        downsampling = false;
        sshot_downsampled = nullptr;
    }
    else
    {
        downsampling = true;
        downsample_dims = target_dimension;
        sshot_downsampled = new uint8_t[width * height];
    }
}

std::vector<uint8_t> screenshot_creator::get_image()
{
    get_pixels(&display, sshot);

    auto [width, height] = downsample_dims;
    if (downsampling)
    {
        image_resizer.resizeImage(
            sshot, display.width, display.height,
            0,
            sshot_downsampled, width, height,
            1, 0, 0);

#ifndef NDEBUG
        SDL_Surface* s_full;
        SDL_Surface* s_down;
        s_full = SDL_CreateRGBSurfaceFrom((void*)sshot, display.width, display.height, 8, display.width, 0, 0, 0, 0);
        s_down = SDL_CreateRGBSurfaceFrom((void*)sshot_downsampled, width, height, 8, width, 0, 0, 0, 0);

        if (s_full == NULL || s_down == NULL)
        {
            fprintf(stderr, "No surface: %s\n", SDL_GetError());
            return nullptr;
        }

        set_grayscale_palette(s_full);
        set_grayscale_palette(s_down);

        if (SDL_SaveBMP(s_full, "sshot.bmp") != 0 || SDL_SaveBMP(s_down, "sshot_down.bmp") != 0)
        {
            fprintf(stderr, "No bitmap: %s\n", SDL_GetError());
            SDL_FreeSurface(s_full);
            SDL_FreeSurface(s_down);
            return nullptr;
        }
        SDL_FreeSurface(s_full);
        SDL_FreeSurface(s_down);
#endif
    }

    std::vector<uint8_t> image;
    if (downsampling)
    {
        image.assign(sshot_downsampled, sshot_downsampled + (width * height));
    }
    else
    {
        image.assign(sshot, sshot + (display.width * display.height));
    }

    return image;
}

screenshot_creator::~screenshot_creator()
{
    delete[] sshot;

    if (downsampling)
    {
        delete[] sshot_downsampled;
    }
}
