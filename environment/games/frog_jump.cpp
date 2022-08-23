#include "frog_jump.hpp"

extern "C"
{
    #include "vecx.h"
}

#ifdef DEBUG 
#include <iostream>
#endif

using namespace vecx_rl;

frog_jump::frog_jump() : ROM({NOOP, BTN_2, BTN_4, JOY_LEFT, JOY_RIGHT})
{ }

frog_jump::~frog_jump()
{
    reset();
}

void frog_jump::reset()
{
    terminal = false;
    reward = 0;
    // reset score                vecx-view:
    ram[0x82] = (unsigned char)0x00; // MSB
    ram[0x83] = (unsigned char)0x00; // LSB
}

std::unique_ptr<ROM> frog_jump::get_instance() const
{
    return std::make_unique<frog_jump>();
}

bool frog_jump::is_terminal() const
{
    return terminal;
}

reward_t frog_jump::get_reward() const
{
    return reward; 
}

const char* frog_jump::get_name() const
{ 
    return "frog_jump.bin"; 
}

// NOTE: GCC builtin!
// NOTE: Vecx is Big Endian
reward_t frog_jump::process_state() 
{
    /*  
    struct game_t
    {
        long unsigned int score;     = 0x82-0x83
        unsigned int difficulty;     = 0x84
        unsigned int running;        = 0x85
        long unsigned int highscore; = 0x86-0x87
        unsigned int alive;          = 0x88
        unsigned int score_delay;    = 0x89
    };
    */
    score = __builtin_bswap16(*((uint16_t*)(ram+0x82)));
    // prevent underflow 
    reward = (score != 0) ? score - last_score : 0;
    last_score = score;
    
    #ifdef DEBUG 
        std::cout << "Score: " <<  __builtin_bswap16(*((uint16_t*)(ram+0x82))) << " , REWARD_" << reward << std::endl;
    #endif

    uint16_t alive = (uint16_t)ram[0x88];

    terminal = (alive == 2 || alive == 3);

    return reward;
}
