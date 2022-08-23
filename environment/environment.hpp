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
#include "unsupported_rom.hpp"

#include <string>
#include <memory>

namespace vecx_rl
{
    class environment
    {
    public:
        /**
         * Construct an environment which controls the vecx-emulator and the game.
         * @param frames: Number of frames for which an input is emulated
         * @param enable_sound: Control sound
        */
        environment(uint64_t frames = 1, bool enable_sound = false);
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

        // get_pixel_array

    private:
        // configuration flags
        bool sound;
        uint64_t emu_frames;
        // current game
        std::unique_ptr<ROM> rom;
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
} // namespace vecx_rl
