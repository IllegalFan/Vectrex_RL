#pragma once

#include <exception>

namespace vecx_rl
{
    /**
     * Indicate that a given rom is not supported
    */
    class unsupported_rom : public std::exception 
    {
    public:
        unsupported_rom(const char* msg) : message(msg) {}
        
        const char* what() 
        {
            return message;
        }
    private:
        const char* message;
    };
} // namespace vecx_rl

