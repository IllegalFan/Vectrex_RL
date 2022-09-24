#include "environment.hpp"

#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <thread>

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
        std::cout << duration.count() << std::endl;
    }

private:
    std::chrono::_V2::system_clock::time_point start;
};

int main(int argc, char** argv)
{
    std::string cartfilename = std::string("frog_jump.bin");

    vecx_rl::vector_2D<uint16_t> dims = {.width = 40, .height = 60};
    uint8_t* pixels = nullptr;

    vecx_rl::environment env = vecx_rl::environment(25, true, false, dims);

    vecx_rl::reward_t r = 0;

    try
    {
        env.load_rom(cartfilename);
    }
    catch (vecx_rl::unsupported_rom& e)
    {
        std::cerr << e.what() << '\n';
        std::exit(-1);
    }

    vecx_rl::action a;

    std::srand(std::time(nullptr)); // use current time as seed for random generator
    uint16_t random_variable = 0;

    std::vector<uint8_t> legal_actions = env.get_legal_actions();

    for (size_t episode = 0; episode < 10; ++episode)
    {
        while (!env.is_game_finished())
        {
            random_variable = std::rand();
            a.set_action(
                legal_actions.at(
                    random_variable % legal_actions.size()));
            r += env.step(a);

            pixels = env.get_image().value_or(nullptr);
            //  std::cout << (uint16_t)(pixels[0]) << std::endl;
        }

        std::cout << "Episode: " << episode << ", Score: " << r << "\n";
        env.reset();
        r = 0;
    }

    return 0;
}
