#pragma once

/**
 * Restrictions:
 * - only solo player games
 * - no sound ?
 * 
 * NOTE:
 * - Only one instance of environment can exist since vecx operates on global values!
 */

#include "ROM.hpp"
#include "action.hpp"

#include <string>
#include <memory>
#include <array>

namespace vecx_rl
{
    class environment
    {
    public:
        environment(const std::string& cartfilename, bool enable_sound = false);
        ~environment();
        
        reward step(const action& input);

        void reset();

        inline std::vector<uint8_t> get_legal_actions() const
        {
            return rom->get_legal_actions();
        }

        // get_pixel_array

    private:
        bool sound;
        std::unique_ptr<ROM> rom;
    };

} // namespace vecx_rl

