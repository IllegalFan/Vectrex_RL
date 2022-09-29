#pragma once

#include "avir.h"

#include <utility>

extern "C"
{
#include "display.h"
}

namespace vecx_rl
{
    class screenshot_creator
    {
    public:
        /**
         * Create a screenshot and downsample the image to the size given in <target_dimensions>
         * @param target_dimensions: Dimensions of the downsampled image. Downsampling is disabled if any dimensions is <= 0.
         * Format is: {width, height}
         */
        screenshot_creator(std::pair<int, int> target_dimensions = {0, 0});

        ~screenshot_creator();

        inline void set_target_dimensions(std::pair<int, int> target_dimensions);

        /**
         * Return image as pixel array of size <width * height> and values in [0, 254]
         */
        uint8_t* get_image();

    private:
        std::pair<int, int> downsample_dims;
        bool downsampling;
        uint8_t* sshot;
        uint8_t* sshot_downsampled;
        avir::CImageResizer<> image_resizer;
    };

    void screenshot_creator::set_target_dimensions(std::pair<int, int> target_dimensions)
    {
        downsampling = true;
        downsample_dims = target_dimensions;
        auto [width, height] = target_dimensions;
        sshot_downsampled = new uint8_t[width * height];
    }

} // namespace vecx_rl
