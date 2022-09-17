#include "image.hpp"

using namespace vecx_rl;

screenshot_creator::screenshot_creator(vector_2D<uint16_t> target_dimension) : image_resizer(8)
{
    sshot = (uint8_t*)malloc(display.width * display.height);

    if (target_dimension.width <= 0 || target_dimension.height <= 0)
    {
        downsampling = false;
    }
    else
    {
        downsampling = true;
        downsample_dims = target_dimension;
        sshot_downsampled = (uint8_t*)malloc(target_dimension.width * target_dimension.height);
    }
}

uint8_t* screenshot_creator::get_image()
{
    get_pixels(&display, sshot);

    if (downsampling)
    {
        image_resizer.resizeImage(
            sshot, display.width, display.height,
            0,
            sshot_downsampled, downsample_dims.width, downsample_dims.height,
            1, 0, 0);

#if DEBUG
        SDL_Surface* s_full;
        SDL_Surface* s_down;
        s_full = SDL_CreateRGBSurfaceFrom((void*)sshot, display.width, display.height, 8, display.width, 0, 0, 0, 0);
        s_down = SDL_CreateRGBSurfaceFrom((void*)sshot_downsampled, downsample_dims.width, downsample_dims.height, 8, downsample_dims.width, 0, 0, 0, 0);

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

    return ((downsampling) ? sshot_downsampled : sshot);
}

screenshot_creator::~screenshot_creator()
{
    free((void*)sshot);

    if (downsampling)
    {
        free((void*)sshot_downsampled);
    }
}
