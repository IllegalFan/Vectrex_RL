#include "environment.hpp"

#include <chrono>
#include <thread>

#include <iostream>

struct timer
{
    timer()
    {
        start = std::chrono::high_resolution_clock::now();
    }
    ~timer()
    {
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
        std::cout << duration.count() << '\n';
    }

  private:
    std::chrono::_V2::system_clock::time_point start;
};

int main(void)
{
    open_window();
    init();
    // osint_emuloop();
    osint_emu(1);
    /* while (true)
    {
        {
            timer t = timer();
            osint_emu(50);
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    } */

    close_window();

    return 0;
}