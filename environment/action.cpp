#include "action.hpp"

using namespace vecx_rl;

action::action() : event(0) 
{ }

action::~action() { }

void action::set_action(const uint8_t action)
{
    event = action % 9;
}
