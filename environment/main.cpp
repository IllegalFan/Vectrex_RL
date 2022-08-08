#include "environment.hpp"

#include <chrono>
#include <thread>

#include <string>
#include <iostream>

#include <cstdlib>
#include <ctime>

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

/**
 * Fragen:
 * - Welches Spiel als Proof of Work? -> Wahlfach
 * - Eigenschaften Input-Bild (Dimension, Farbtiefe)?
 * - Live-training oder Hintergrund (SDL/Sound)?
 *      -> Introsequenz wegschneiden
*/

int main(int argc, char** argv)
{
    std::string cartfilename = std::string("frog_jump.bin");

    vecx_rl::environment env(cartfilename);
    
    vecx_rl::action a;

    std::srand(std::time(nullptr)); // use current time as seed for random generator
    uint16_t random_variable = 0;

    for (size_t i = 0; i < 7; i++)
    {
        a.set_action(0x02);
        env.step(a);
        a.set_action(0x02);
        env.step(a);
    }
    
    std::vector<uint8_t> legal_actions = env.get_legal_actions();

    while (true)
    {
        random_variable = std::rand();
        a.set_action(
            legal_actions.at(
                random_variable % legal_actions.size()
            )
        );
        env.step(a);
        #if 0
        random_variable = (std::rand() % 4) + 1;
        a.press_button(random_variable);
        //std::cout << random_variable << " ";
        env.step(a);
        
        random_variable = (std::rand() % 4) + 1;
        a.release_button(random_variable);
        //std::cout << random_variable << "\n";
        env.step(a);
        #endif
    }


    return 0;
}