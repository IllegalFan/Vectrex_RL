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

// A certain periphery is interacted with if the according bit is set
// A periphery gets activated (e.g. button pushed) at the first interaction, at the second it gets deactivated (e.g. button released)
void periphery_emu(const uint8_t input)
{
	if (input == BTN_1)
    {
		printf("BTN_1\n");
        snd_regs[14] ^= 0x01;
    }
	else if (input == BTN_2)
    {
		printf("BTN_2\n");
        snd_regs[14] ^= 0x02;
    }
    else if (input == BTN_3)
    {
		printf("BTN_3\n");
        snd_regs[14] ^= 0x04;
    }
    else if (input == BTN_4)
    {
		printf("BTN_4\n");
        snd_regs[14] ^= 0x08;
    }
    else if (input == JOY_UP)
    {
		printf("JOY_UP\n");
        alg_jch1 = (alg_jch0 == 0x80) ? 0xff : 0x80;
    }
    else if (input == JOY_DOWN)
    {
		printf("JOY_DOWN\n");
        alg_jch1 = (alg_jch0 == 0x80) ? 0x00 : 0x80;
    }
    else if (input == JOY_LEFT)
    {
		printf("JOY_LEFT\n");
        alg_jch0 = (alg_jch0 == 0x80) ? 0x00 : 0x80;
    }
    else if (input == JOY_RIGHT)
    {
		printf("JOY_RIGHT\n");
        alg_jch0 = (alg_jch0 == 0x80) ? 0xff : 0x80;
    }
	else printf("NOOP\n");
}

void osint_emu(const unsigned int emu_frames) {
	Uint32 next_time = SDL_GetTicks() + EMU_TIMER;
	SDL_Event e;
	for(unsigned int frames=0; frames<=emu_frames; ++frames) {
		vecx_emu((VECTREX_MHZ / 1000) * EMU_TIMER);

		#if 1
		if(SDL_PollEvent(&e))
		{
			if(e.type == SDL_QUIT)
			{
				exit(EXIT_SUCCESS);
			}
		}
		#endif

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
