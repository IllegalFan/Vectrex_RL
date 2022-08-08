extern "C"
{
#include "display.h"
#include "e8910.h"
#include "osint.h"
#include "vecx.h"
}

#include "games.hpp"

#include "environment.hpp"
#include <algorithm>
#include <string_view>

using namespace vecx_rl;

environment::environment(const std::string& cartfilename, bool enable_sound)
{
    open_window(&display);
    // vectrex rom
    const char* romfilename = "../../vecx/rom.dat";
    init(romfilename, cartfilename.c_str());

    if(enable_sound)
    {
        sound = enable_sound;
        e8910_init_sound();
    }

    reset();

    // TODO
    rom = std::make_unique<frog_jump>();
}

environment::~environment()
{
    close_window();

    vecx_reset();

    if(sound)
    {
        e8910_done_sound();
    }
}

void environment::reset()
{
    vecx_reset();
}

reward environment::step(const action& input)
{
    reward reward = 0;

    if (rom->is_terminal())
    {
        return 0;
    }

    if(!rom->is_action_legal(input))
    {
        return -1;
    }

    periphery_emu(input.get_action()); // set registers
    osint_emu(25); // TODO

    //rom->process_state();
    #if 0

    // reward = rom.process_state();
    #endif

    return reward;
}
