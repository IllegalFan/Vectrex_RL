#pragma once

#include "../action.hpp"

#include <algorithm>
#include <vector>
#include <memory>

namespace vecx_rl
{
    // max 16 bit possible on MC6809
    typedef uint16_t reward_t;

    class ROM
    {
    public:
        ROM(std::vector<uint8_t> legal_actions);
        virtual ~ROM() {};

        // reset
        virtual void reset();
 
        virtual std::unique_ptr<ROM> get_instance() const = 0;

        // is end of game
        virtual bool is_terminal() const = 0;

        // get the most recently observed reward
        virtual reward_t get_reward() const = 0;

        // the rom-name
        virtual const char* get_name() const = 0;

        // calc reward and check if end is reached
        virtual reward_t process_state() = 0;

        inline bool is_action_legal(const action& input);

        inline std::vector<uint8_t> get_legal_actions() const;

    protected:
        const std::vector<uint8_t> legal_actions;
        reward_t reward;
        bool terminal;
    };

    /* Implementation */

    bool ROM::is_action_legal(const action& input)
    {
        auto is_legal = std::find(begin(legal_actions), end(legal_actions), input.get_action());
        return (is_legal != std::end(legal_actions));
    }

    std::vector<uint8_t> ROM::get_legal_actions() const
    {
        return legal_actions;
    }

} // namespace vecx_rl


