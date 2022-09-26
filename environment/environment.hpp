#pragma once

/**
 * Restrictions:
 * - only solo player games
 * - do NOT touch vecx directly, could end in segfaults
 *
 * NOTE:
 * - Only one instance of environment can exist since vecx operates on global values!
 */

#include "ROM.hpp"
#include "action.hpp"
#include "image.hpp"
#include "unsupported_rom.hpp"

#include <memory>
#include <optional>
#include <string>

namespace vecx_rl
{
    /**
     * Provides methods for interaction with the vecx emulator for reinforcement learning agents
     */
    class environment
    {
    public:
        /**
         * Construct an environment which controls the vecx-emulator and the game.
         * @param frames_per_step: Number of frames for which vecx will emulate per step
         * @param real_time: If true the emulation will happen in real time (50 frames per second). Else it will go as fast as the cpu can
         * @param enable_sound: En-/Disable sound output of vecx
         * @param enable_window: En-/Disable rendering of vecx
         * @param image_dims: If the option has a value, the environment will take a screenshot per step.
         * If both dimensions are > 0, the screenshot will be downsampled to the given dimensions. Format is: {width, height}.
         * Else the screenshots will keep their original size: {495, 615}
         */
        environment(
            // vecx configuration
            uint64_t frames_per_step = 1,
            bool real_time = true,
            bool enable_window = true,
            bool enable_sound = false,
            // screenshot configuration
            const std::optional<vector_2D<uint16_t>>& image_dims = {});
        ~environment();

        /**
         * Check if rom with given name is supported
         * @param carfilename: Name of game that should be emulated
         * @throws unsupported_rom: Throws if rom is not supported. See vecx_rl::ROM for details
         */
        void load_rom(const std::string& carfilename);

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
         * Get current frame as input for the reinforcement learning agent
         */
        std::optional<uint8_t*> get_image();

    private:
        // configuration flags
        bool real_time;
        bool window_enabled;
        bool sound_enabled;
        bool screenshot_enabled;

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

} // namespace vecx_rl
