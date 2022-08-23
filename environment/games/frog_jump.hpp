#pragma once

#include "ROM.hpp"

namespace vecx_rl
{
    class frog_jump : public ROM
    {
    public:
        frog_jump(/* args */);
        ~frog_jump();

        void reset() override;

        std::unique_ptr<ROM> get_instance() const override;

        bool is_terminal() const override;

        // get the most recently observed reward
        reward_t get_reward() const override;

        // the rom-name
        virtual const char* get_name() const override;

        // calc reward and check if end is reached
        reward_t process_state() override;
    private:
        uint16_t score;
        uint16_t last_score;
    };
} // namespace vecx_rl

