#pragma once

#include "frog_jump.hpp"

#include <array>
#include <memory>

namespace vecx_rl
{
    const std::array<std::shared_ptr<ROM>, 1> supported_roms = {
        std::make_shared<frog_jump>()};
}
