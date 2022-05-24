#include "osint.h"
#include "e8910.h"
#include "vecx.h"

#define EMU_TIMER 20 /* the emulators heart beats at 20 milliseconds */

void init(const char* romfilename, const char* cartfilename)
{
	FILE *f;
	if(!(f = fopen(romfilename, "rb"))){
		perror(romfilename);
		exit(EXIT_FAILURE);
	}
	if(fread(rom, 1, sizeof (rom), f) != sizeof (rom)){
		printf("Invalid rom length\n");
		exit(EXIT_FAILURE);
	}
	fclose(f);

	memset(cart, 0, sizeof (cart));
	if(cartfilename){
		FILE *f;
		if(!(f = fopen(cartfilename, "rb"))){
			perror(cartfilename);
			exit(EXIT_FAILURE);
		}
		fread(cart, 1, sizeof (cart), f);
		fclose(f);
	}
}

void periphery_emu(const SDL_Event input)
{
	switch (input.type)
	{
		case SDL_KEYDOWN:
			switch(input.key.keysym.sym)
			{
				case SDLK_a:
					snd_regs[14] &= ~0x01;
					break;
				case SDLK_s:
					snd_regs[14] &= ~0x02;
					break;
				case SDLK_d:
					snd_regs[14] &= ~0x04;
					break;
				case SDLK_f:
					snd_regs[14] &= ~0x08;
					break;
				case SDLK_LEFT:
					alg_jch0 = 0x00;
					break;
				case SDLK_RIGHT:
					alg_jch0 = 0xff;
					break;
				case SDLK_UP:
					alg_jch1 = 0xff;
					break;
				case SDLK_DOWN:
					alg_jch1 = 0x00;
					break;
				default:
					break;
			}
			break;
		case SDL_KEYUP:
			switch(input.key.keysym.sym)
			{
				case SDLK_a:
					snd_regs[14] |= 0x01;
					break;
				case SDLK_s:
					snd_regs[14] |= 0x02;
					break;
				case SDLK_d:
					snd_regs[14] |= 0x04;
					break;
				case SDLK_f:
					snd_regs[14] |= 0x08;
					break;
				case SDLK_LEFT:
					alg_jch0 = 0x80;
					break;
				case SDLK_RIGHT:
					alg_jch0 = 0x80;
					break;
				case SDLK_UP:
					alg_jch1 = 0x80;
					break;
				case SDLK_DOWN:
					alg_jch1 = 0x80;
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}
}

void osint_emu(const unsigned int emu_frames) {
	Uint32 next_time = SDL_GetTicks() + EMU_TIMER;

	for(unsigned int frames=0; frames<=emu_frames; ++frames) {
		vecx_emu((VECTREX_MHZ / 1000) * EMU_TIMER);

		{
			Uint32 now = SDL_GetTicks();
			if(now < next_time)
				SDL_Delay(next_time - now);
			else
				next_time = now;
			next_time += EMU_TIMER;
		}
	}
}
