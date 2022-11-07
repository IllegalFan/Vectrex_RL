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
    bool real_time,
    bool enable_window,
    bool enable_sound,
    const std::optional<std::pair<int, int>>& image_dims)
    : real_time(real_time), window_enabled(enable_window), sound_enabled(enable_sound),
      screenshot_enabled(image_dims.has_value()), emu_frames(frames_per_step), sc()
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

void environment::load_rom(const std::filesystem::path& cartfilename)
{
    auto supported_rom = std::find_if(
        std::begin(supported_roms), std::end(supported_roms),
        [&cartfilename](const std::shared_ptr<ROM>& potential_rom)
        {
            return (potential_rom->get_name() == cartfilename.filename());
        });

    if (supported_rom != std::end(supported_roms))
    {
        rom = *supported_rom;

        std::filesystem::path rom_path;

        // first search the file in the standard directory
        std::filesystem::path std_rom_dir = std::filesystem::path(ROM_BASEDIR);
        std_rom_dir += cartfilename.filename();

        if (std::filesystem::exists(std_rom_dir))
        {
            rom_path = std::move(std_rom_dir);
        }
        // second search the given path
        else
        {
            if (std::filesystem::exists(cartfilename))
            {
                rom_path = std::move(cartfilename);
            }
            else
            {
                throw std::filesystem::filesystem_error("ROM could not be found", rom_path, std::error_code(ENOENT, std::system_category()));
            }
        }

        // vectrex rom (contains firmware and minestorm game)
        const char* romfilename = VECX_ROM;
        init(romfilename, rom_path.c_str());

        reset();
    }
    else
    {
        throw unsupported_rom("ROM is not supported!");
    }
}

/**
 * Note that `vecx_reset` overwrites the ram with incrementing integers (0,1,2,3,...)
 * Also note that rom::process_state is called before the game actually starts
 * So if a variable is expected to be e.g. 0 at the begining, that should be done in `ROM::reset`
 */
void environment::reset()
{
    vecx_reset(); // must be called first!
    if (rom.has_value())
    {
        rom.value()->reset();
    }
}

/**
 * Vectrex games require e.g. a button to be pressed to start the game
 * Automatically starting the game saves time and reduces the action space of the machine learning agent
 */
void environment::start_new_game()
{
    reset();

    action start_action;
    start_action.set_action(rom.value()->get_start_game_action());

    periphery_emu(start_action.get_action());
    // Emulate until the screen appears where the player is requested to e.g. press a button to start the game
    osint_emu(130, (uint8_t)0);
}

reward_t environment::step(const action& input)
{
    if (rom.value()->is_terminal())
    {
        return 0;
    }

    if (!(rom.value()->is_action_legal(input)))
    {
        return -1;
    }

    periphery_emu(input.get_action()); // set registers
    osint_emu(emu_frames, (uint8_t)real_time);

    return rom.value()->process_state();
}

std::optional<std::vector<uint8_t>> environment::get_image()
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
