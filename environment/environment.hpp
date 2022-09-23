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
#include "image.hpp"
#include "unsupported_rom.hpp"

#include <memory>
#include <string>

namespace vecx_rl
{
    class environment
    {
    public:
        /**
         * Construct an environment which controls the vecx-emulator and the game.
         * @param frames_per_step: Number of frames for which an input is emulated
         * @param image_dims: Target dimensions of the image for the rl-agent
         * @param enable_sound: Control sound
         */
        environment(uint64_t frames_per_step = 1,
                    const vector_2D<uint16_t>& image_dims = {0, 0},
                    bool enable_sound = false);
        ~environment();

        /* Configuration */

        /**
         * Check if rom with given name is supported. Throws `unsupported_rom` if not.
         * @param carfilename: Name of game that should be emulated
         */
        void load_rom(const std::string& carfilename);

        /*  */

        /**
         * Emulate the configured number of frames with the given input
         * @param input: Input to the game in the emulator
         */
        reward_t step(const action& input);

        /**
         * Reset the environment and the emulator
         */
        void reset();

        /**
         * Return true if game has finished
         */
        inline bool is_game_finished() const;

        /**
         * Get the current reward
         */
        inline reward_t get_reward() const;

        /**
         * Get the actions that are legal for the current game
         */
        inline std::vector<uint8_t> get_legal_actions() const;

        /**
         * Get current frame as input for the rl-agent
         */
        inline uint8_t* get_image();

    private:
        // configuration flags
        bool sound;
        uint64_t emu_frames;
        // current game
        std::shared_ptr<ROM> rom;
        screenshot_creator sc;
    };

    /* Implementation of wrapper methods */

    bool environment::is_game_finished() const
    {
        return (rom->is_terminal());
    }

    std::vector<uint8_t> environment::get_legal_actions() const
    {
        return rom->get_legal_actions();
    }

    reward_t environment::get_reward() const
    {
        return rom->get_reward();
    }

    uint8_t* environment::get_image()
    {
        return sc.get_image();
    }
} // namespace vecx_rl
