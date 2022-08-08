#include "ROM.hpp"

#include <algorithm>

extern "C"
{
  #include "SDL.h"
  #include "periphery.h"
}

using namespace vecx_rl;

constexpr auto IS_ACTIVE = [](const uint8_t bit_list,const int peripheral) 
{
    //return ((bit_list >> (peripheral-1)) & 1UL);
    return (bit_list & peripheral);
};

ROM::ROM(std::vector<uint8_t> legal_actions) : legal_actions(legal_actions)
{ }

bool ROM::is_action_legal(const action& input)
{
    auto is_legal = std::find(begin(legal_actions), end(legal_actions), input.get_action());
    return (is_legal != std::end(legal_actions));
}
