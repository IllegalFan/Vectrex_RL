#include "ROM.hpp"

#include <iostream>

extern "C"
{
    #include "vecx.h"
}


namespace vecx_rl
{
    uint8_t read_ram(const uint16_t address);

    uint8_t read_ram(const uint16_t address)
    {
        return ram[address % 1024];
    }

    class frog_jump : public ROM
    {
    public:
        frog_jump(/* args */);
        ~frog_jump();

        void reset() override;

        bool is_terminal() override;

        // get the most recently observed reward
        reward get_reward() override;

        // the rom-name
        const char* rom() override;

        // is an action part of the minimal set?
        //virtual bool is_minimal(const SDL_KeyCode& a) = 0;

        // calc reward and check if end is reached
        void process_state() override;
    };

    frog_jump::frog_jump() : ROM({NOOP, BTN_2, BTN_4, JOY_LEFT, JOY_RIGHT})
    {
    }

    frog_jump::~frog_jump()
    {
    }

    void frog_jump::reset()
    {

    }

    bool frog_jump::is_terminal()
    {
        return false;
    }

    reward frog_jump::get_reward() { return 0; }

    const char* frog_jump::rom() { return "frog_jump"; }
    // NOTE: GCC builtin!
    void frog_jump::process_state() { std::cout << __builtin_bswap16(*((int16_t*)(ram+0x82))) << "\n"; }
} // namespace vecx_rl

