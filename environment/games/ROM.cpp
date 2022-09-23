#include "ROM.hpp"

using namespace vecx_rl;

ROM::ROM(std::vector<uint8_t> legal_actions) : legal_actions(legal_actions), reward(0), terminal(false)
{
}

// Standard reset
void ROM::reset()
{
    terminal = false;
    reward = 0;
}
