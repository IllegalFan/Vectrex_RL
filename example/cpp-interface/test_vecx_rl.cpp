/**********************************************************************************************************
 * Simple example of the usage of the cpp interface with random inputs instead of a machine learning agent
 **********************************************************************************************************/

#include "environment.hpp"

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>

int main(int argc, char** argv)
{
    std::pair<int, int> image_dims = {40 /* width */, 60 /* height */};
    std::vector<uint8_t> pixels;

    vecx_rl::environment env = vecx_rl::environment(1, true, true, true, image_dims);

    std::string cartfilename = std::string("frog_jump.bin");

    try
    {
        env.load_rom(cartfilename);
    }
    catch (vecx_rl::unsupported_rom& e)
    {
        std::cerr << e.what() << '\n';
        std::exit(-1);
    }
    catch (std::filesystem::filesystem_error& e)
    {
        std::cerr << e.what() << '\n';
        std::exit(-1);
    }

    vecx_rl::action a;
    vecx_rl::reward_t r = 0;

    std::srand(std::time(nullptr)); // use current time as seed for random generator
    uint16_t rand_action = 0;

    std::vector<uint8_t> legal_actions = env.get_legal_actions();

    for (size_t episode = 0; episode < 10; ++episode)
    {
        while (!env.is_game_finished())
        {
            rand_action = std::rand();
            a.set_action(
                legal_actions.at(
                    rand_action % legal_actions.size()));
            r += env.step(a);

            pixels = env.get_image().value_or(std::vector<uint8_t>{});

            // feed reward and image to a reinforcement learning agent ...
        }

        std::cout << "Episode: " << episode << ", Score: " << r << "\n";
        env.reset();
        r = 0;
    }

    return 0;
}
