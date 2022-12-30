# Vectrex-RL

## Requirements
* cmake >= 3.18
* python >= 3.6
* c++17 compiler
* make or ninja

## Usage
```bash
git clone --recursive <url>
cd Vectrex_RL && mkdir build
cmake --build build --config Release --target pyvecx_rl -j 8
```

## Implementation of new Vectrex Game

First the binary of the game itself needs to be added in the `res/vectrex_roms` folder.

Then the files `game_name.cpp` and `game_name.hpp` in the `games` folder need to be created. They also must be added to the list of SOURCES in the CMake file.
In the c++ files the class `game_name` must be defined, which must inherit from the abstract class `ROM` from `games/ROM.hpp`.
The methods of `ROM` must be overriden to fit the needs of the new game. Also do not forget to declare the set of legal actions by calling the `ROM` constructor:
```cpp
std::vector<uint8_t> legal_actions = {NOOP, BTN_4, JOY_LEFT, JOY_RIGHT};
frog_jump::frog_jump() : ROM(legal_actions)
{ }
```

In order to be able to implement the necessary methods properly, especially `ROM::process_state`, certain memory locations need to be known.
Examples are the score or the amount of lives the player has left.
These memory locations can either be located on the linker map, which can be generated with a compile option, or can be looked up on the internet for known vectrex games.
Be aware that the value returned by `ROM::get_name` must be equal to the binary name.

After the new class is finished, it needs to be added to the array of supported roms in `games/roms.hpp`.

Then the training environment needs to be implemented for the new game.
Therefore a new python file, similar to `FrogEnv.py`, needs be created.
This file needs to contain the `init`-, `step`-, and `render`-function. 
Furthermore the individual action- and observation space needs to be implemented. 