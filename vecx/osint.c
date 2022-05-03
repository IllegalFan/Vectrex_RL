
#include "SDL.h"
#include "SDL_image.h"
#include "SDL2_gfxPrimitives.h"

#include "osint.h"
#include "e8910.h"
#include "vecx.h"
#include "sys/types.h"
#include "sys/ipc.h"
#include "sys/msg.h"

#include "type_definitions.h"

#define EMU_TIMER 20 /* the emulators heart beats at 20 milliseconds */

#define BUFF_SIZE 16

static SDL_Window *screen = NULL;
static SDL_Renderer *renderer= NULL;
static SDL_Texture *overlay = NULL;

static long scl_factor;
static long offx;
static long offy;

typedef struct {
	long  data_type;
	//int   data_num;
	unsigned char  data_buff[BUFF_SIZE];
} t_data;

void osint_render(void){
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	int v;
	for(v = 0; v < vector_draw_cnt; v++){
		Uint8 c = vectors_draw[v].color * 256 / VECTREX_COLORS;
		aalineRGBA(renderer,
				offx + vectors_draw[v].x0 / scl_factor,
				offy + vectors_draw[v].y0 / scl_factor,
				offx + vectors_draw[v].x1 / scl_factor,
				offy + vectors_draw[v].y1 / scl_factor,
				c, c, c, 0xff);
	}
	if(overlay){
		SDL_Rect dest_rect = {offx, offy, ((double)ALG_MAX_X / (double)scl_factor), ((double)ALG_MAX_Y / (double)scl_factor)};
		SDL_RenderCopy(renderer, overlay, NULL, &dest_rect);
	}
	SDL_RenderPresent(renderer);
}

static char *romfilename = "rom.dat";
static char *cartfilename = NULL;

static void init(){
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

void resize(int width, int height){
	long sclx, scly;

	long screenx = width;
	long screeny = height;

	sclx = ALG_MAX_X / width;
	scly = ALG_MAX_Y / height;

	scl_factor = sclx > scly ? sclx : scly;

	offx = (screenx - ALG_MAX_X / scl_factor) / 2;
	offy = (screeny - ALG_MAX_Y / scl_factor) / 2;
}

static void readevents(int ctrlInput){
	//reset all inputs
	snd_regs[14] |= 0x01; //a
	snd_regs[14] |= 0x02; //s
	snd_regs[14] |= 0x04; //d
	snd_regs[14] |= 0x08; //f
	alg_jch0 = 0x80;	  //left
	alg_jch0 = 0x80;	  //right
	alg_jch1 = 0x80;	  //up
	alg_jch1 = 0x80;	  //down
	switch(ctrlInput){
		case 1: //a
			snd_regs[14] &= ~0x01;
			break;
		case 2: //s
			snd_regs[14] &= ~0x02;
			break;
		case 3: //d
			snd_regs[14] &= ~0x04;
			break;
		case 4: //f
			snd_regs[14] &= ~0x08;
			break;
		case 5: //left
			alg_jch0 = 0x00;
			break;
		case 6: //right
			alg_jch0 = 0xff;
			break;
		case 7: //up
			alg_jch1 = 0xff;
			break;
		case 8: //down
			alg_jch1 = 0x00;
			break;
		default:
			break;
	}
}

void osint_emuloop(){
	vecx_reset();
	int frame_count = 0;
	int msqid_rc;
	int msqid_sd;
	t_data data_rc;
	t_data data_sd;
	int ctrlInput;
	if ( -1 == ( msqid_rc = msgget( (key_t)1234, IPC_CREAT | 0666)))
	{
		perror( "msgget() failed");
		exit( 1);
	}
	if ( -1 == ( msqid_sd = msgget( (key_t)1235, IPC_CREAT | 0666)))
	{
		perror( "msgget() failed");
		exit( 1);
	}
	for(;;){
		if ( -1 == msgrcv( msqid_rc, &data_rc, sizeof( t_data) - sizeof( long), 0, 0))
		{
			perror( "msgrcv() failed");
			exit( 1);
		}
		if(data_rc.data_type == TYPE_INTEGER)
		{
			memcpy(&ctrlInput, data_rc.data_buff, sizeof(int));
			printf("\n");
			printf("Interpreted as Integer: %d", ctrlInput);
		}
		else if(data_rc.data_type == TYPE_STRING)
		{
			ctrlInput = 5;
			printf("Interpreted as string: %15s\n", data_rc.data_buff);
		}
		readevents(ctrlInput);
		data_sd.data_type = TYPE_INTEGER;
		memcpy(data_sd.data_buff, &frame_count, sizeof(int));
		if ( -1 == msgsnd( msqid_sd, &data_sd, sizeof( t_data) - sizeof( long), 0))
		{
			perror( "msgsnd() failed");
			exit(1);
		}
		frame_count++;
		vecx_emu((VECTREX_MHZ / 1000) * EMU_TIMER);
	}
}

void load_overlay(const char *filename){
	SDL_Surface *image;
	image = SDL_LoadBMP(filename);
	if(image){
		overlay = SDL_CreateTextureFromSurface(renderer, image);
		SDL_FreeSurface(image);
	}else{
		fprintf(stderr, "IMG_Load: %s\n", IMG_GetError());
	}
}

int main(int argc, char *argv[]){
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0){
		fprintf(stderr, "Failed to initialize SDL: %s\n", SDL_GetError());
		exit(-1);
	}
	SDL_CreateWindowAndRenderer(330*3/2, 410*3/2, SDL_WINDOW_RESIZABLE, &screen, &renderer);
	if(screen == NULL || renderer == NULL){
		fprintf(stderr, "Failed to initialize SDL window/renderer: %s\n", SDL_GetError());
		exit(-2);
	}

	resize(330*3/2, 410*3/2);

	if(argc > 1)
		cartfilename = argv[1];
	if(argc > 2)
		load_overlay(argv[2]);

	init();

	e8910_init_sound();
	osint_emuloop();
	e8910_done_sound();
	SDL_Quit();

	return 0;
}

