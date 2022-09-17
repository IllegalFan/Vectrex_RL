#pragma once

#include "avir.h"

#include <iostream>
#include <utility>

extern "C"
{
#include "display.h"
}

namespace vecx_rl
{
    template <typename T>
    struct vector_2D
    {
        T width;
        T height;
    };

    class screenshot_creator
    {
    public:
        /**
         * Create a screenshot and downsample the image to the size given in <target_dimension>
         * @param target_dimension: Dimensions of the downsampled image. Downsampling is disabled if any dimensions is <= 0
         */
        screenshot_creator(vector_2D<uint16_t> target_dimension = {0, 0});
        ~screenshot_creator();

        /**
         * Return image as pixel array of size <width * height> and values in [0, 254]
         */
        uint8_t* get_image();

#if DEBUG
    private:
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
    private:
        uint8_t* sshot;
        uint8_t* sshot_downsampled;
        bool downsampling;
        vector_2D<uint16_t> downsample_dims;
        avir::CImageResizer<> image_resizer;
    };

} // namespace vecx_rl
