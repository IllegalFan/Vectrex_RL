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
         * Create a screenshot and downsample the image to the size given in <target_dimensions>
         * @param target_dimensions: Dimensions of the downsampled image. Downsampling is disabled if any dimensions is <= 0
         */
        screenshot_creator(vector_2D<uint16_t> target_dimensions = {0, 0});

        ~screenshot_creator();

        inline void set_target_dimensions(vector_2D<uint16_t> target_dimensions);

        /**
         * Return image as pixel array of size <width * height> and values in [0, 254]
         */
        uint8_t* get_image();

    private:
        vector_2D<uint16_t> downsample_dims;
        bool downsampling;
        uint8_t* sshot;
        uint8_t* sshot_downsampled;
        avir::CImageResizer<> image_resizer;
    };

    void screenshot_creator::set_target_dimensions(vector_2D<uint16_t> target_dimensions)
    {
        downsample_dims = target_dimensions;
    }

} // namespace vecx_rl
