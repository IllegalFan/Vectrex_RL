#pragma once

#include "../action.hpp"

#include <algorithm>
#include <memory>
#include <vector>

namespace vecx_rl
{
    // max 16 bit possible on MC6809
    typedef uint16_t reward_t;

    /**
     * Helper function to swap endiannes for 16 byte integer
     */
    inline uint16_t byte_swap16(uint16_t integer)
    {
        uint16_t first_byte = integer & 0xff00;
        integer = integer << 8;
        integer |= first_byte >> 8;
        return integer;
    }

    /**
     * Virtual ROM Cartridge
     * Every rom must inherit from this class and implement the abstract methods to be supported by the environment
     */
    class ROM
    {
    public:
        /**
         * Create a ROM Cartride
         * @param legal_actions: Action set that is legal for a specific game
         */
        ROM(std::vector<uint8_t> legal_actions);
        virtual ~ROM(){};

        /**
         * Reset the game
         */
        virtual void reset();

        /**
         * True if end of game is reached
         */
        virtual bool is_terminal() const = 0;

        /**
         * Return the most recently observed reward
         */
        virtual reward_t get_reward() const = 0;

        /**
         * The rom name
         */
        virtual const char* get_name() const = 0;

        /**
         * Calculate the reward and check if end is reached
         */
        virtual reward_t process_state() = 0;

        /**
         * True if the action is in the legal action set
         * @param input: Action to be checked
         */
        inline bool is_action_legal(const action& input);

        /**
         * Return the set of legal actions
         */
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
