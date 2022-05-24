#ifndef __OSINT_H
#define __OSINT_H

#include "SDL.h"

void init(const char* romfilename, const char* cartfilename);

void osint_emu(const unsigned int emu_frames);

void periphery_emu(const SDL_Event input);

#endif
