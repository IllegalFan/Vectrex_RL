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

First the binary of the game itself needs to be added in the res folder.

Then the files game_name.cpp and game_name.hpp in the games folder need to be created. 

In order to be able to implement the necessary functions properly certain memory locations need to be known. 
Examples are the score or the amount of lives the player has left.
These memory locations can either be located on the linker map, which can be generated with a compile option, or can be looked up on the internet for known vectrex games. 

