#pragma once

#include "frog_jump.hpp"

#include <array>
#include <memory>

namespace vecx_rl
{
    const std::array<std::unique_ptr<ROM>, 1> supported_games = {
        std::make_unique<frog_jump>()
    };
}
