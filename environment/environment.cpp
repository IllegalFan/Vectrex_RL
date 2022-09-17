extern "C"
{
#include "display.h"
#include "e8910.h"
#include "osint.h"
#include "vecx.h"
}

#include "environment.hpp"
#include "games.hpp"
#include "image.hpp"

#include <algorithm>

#include <iostream>

using namespace vecx_rl;

environment::environment(uint64_t frames, const vector_2D<uint16_t>& image_dims, bool enable_sound)
    : emu_frames(frames), sc(image_dims)
{
    open_window(&display); // TODO: disable rendering !!

    if (enable_sound)
    {
        sound = enable_sound;
        e8910_init_sound();
    }
}

environment::~environment()
{
    close_window();

    if (sound)
    {
        e8910_done_sound();
    }

    reset();
}

void environment::load_rom(const std::string& cartfilename)
{
    auto supported_rom = std::find_if(
        std::begin(supported_games), std::end(supported_games),
        [&cartfilename](const std::unique_ptr<ROM>& potential_rom)
        {
            return (potential_rom->get_name() == cartfilename);
        });

    if (supported_rom != std::end(supported_games))
    {
        rom = (*supported_rom)->get_instance();

        // vectrex rom (contains firmware and minestorm game)
        const char* romfilename = "../../vecx/rom.dat";
        init(romfilename, cartfilename.c_str());

        reset();
    }
    else
    {
        throw unsupported_rom("ROM is not supported!");
    }
}

void environment::reset()
{
    vecx_reset();
    rom->reset();
}

reward_t environment::step(const action& input)
{
    if (rom->is_terminal())
    {
        return 0;
    }

    if (!rom->is_action_legal(input))
    {
        return -1;
    }

    periphery_emu(input.get_action()); // set registers
    osint_emu(emu_frames);

    return rom->process_state();
}
