extern "C"
{
#include "display.h"
#include "e8910.h"
#include "osint.h"
#include "vecx.h"
}

#include "environment.hpp"
#include "image.hpp"
#include "roms.hpp"

#include <algorithm>

#include <iostream>

using namespace vecx_rl;

environment::environment(
    uint64_t frames_per_step,
    bool enable_window,
    bool enable_sound,
    const std::optional<vector_2D<uint16_t>>& image_dims)
    : emu_frames(frames_per_step), window_enabled(enable_window), sound_enabled(enable_sound), screenshot_enabled(image_dims.has_value())
{
    // only render if window is shown or screenshots are required
    if (screenshot_enabled || window_enabled)
    {
        open_window(&display, (uint8_t)enable_window);
    }

    if (screenshot_enabled)
    {
        sc.set_target_dimensions(image_dims.value());
    }

    if (enable_sound)
    {
        e8910_init_sound();
    }
}

environment::~environment()
{
    if (screenshot_enabled || window_enabled)
    {
        close_window();
    }

    if (sound_enabled)
    {
        e8910_done_sound();
    }

    reset();
}

void environment::load_rom(const std::string& cartfilename)
{
    auto supported_rom = std::find_if(
        std::begin(supported_roms), std::end(supported_roms),
        [&cartfilename](const std::shared_ptr<ROM>& potential_rom)
        {
            return (potential_rom->get_name() == cartfilename);
        });

    if (supported_rom != std::end(supported_roms))
    {
        rom = *supported_rom;

        // vectrex rom (contains firmware and minestorm game)
        const char* romfilename = "../../vecx/rom_noIntro.dat";
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

std::optional<uint8_t*> environment::get_image()
{
    if (screenshot_enabled)
    {
        return sc.get_image();
    }
    else
    {
        std::cerr << "Screenshoting is not enabled!\n";
        return {};
    }
}
