#pragma once

#include "../action.hpp"

#include <array>
#include <vector>

namespace vecx_rl
{
    typedef int reward;


    class ROM
    {
    public:
        ROM(std::vector<uint8_t> legal_actions);
        virtual ~ROM() {};

    // reset
    virtual void reset() = 0;

    // is end of game
    virtual bool is_terminal() = 0;

    // get the most recently observed reward
    virtual reward get_reward() = 0;

    // the rom-name
    virtual const char* rom() = 0;

    // is an action part of the minimal set?
    //virtual bool is_minimal(const SDL_KeyCode& a) = 0;

    // calc reward and check if end is reached
    virtual void process_state() = 0;

    virtual bool is_action_legal(const action& input);

    inline std::vector<uint8_t> get_legal_actions() const
    {
        return legal_actions;
    }

    protected:
    const std::vector<uint8_t> legal_actions;
    };
} // namespace vecx_rl


