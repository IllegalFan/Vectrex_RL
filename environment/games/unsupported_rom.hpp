#pragma once

#include <exception>
#include <string>

namespace vecx_rl
{
    /**
     * Indicate that a given rom is not supported
     */
    class unsupported_rom : public std::exception
    {
    public:
        unsupported_rom(const char* msg) : message{msg} {}

        const char* what() const noexcept override
        {
            return message.c_str();
        }

    private:
        std::string message = "";
    };
} // namespace vecx_rl
