#pragma once

#include "ROM.hpp"

namespace vecx_rl
{
    class frog_jump : public ROM
    {
    public:
        frog_jump();
        ~frog_jump();

        void reset() override;

        bool is_terminal() const override;

        reward_t get_reward() const override;

        virtual const char* get_name() const override;

        reward_t process_state() override;

        uint8_t get_start_game_action() override;

    private:
        uint16_t score;
        uint16_t last_score;
    };
} // namespace vecx_rl
