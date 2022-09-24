#include "display.h"
#include "vecx.h"

#include "SDL2_gfxPrimitives.h"
#include "SDL_image.h"

#define R_MASK 0x00ff0000
#define G_MASK 0x0000ff00
#define B_MASK 0x000000ff
#define A_MASK 0xff000000

// all color channels have the same value
#define GET_GRAYSCALE_VAL(pixel) (pixel & B_MASK)

Display display = {
    .height = 410 * 3 / 2, // 615
    .width = 330 * 3 / 2,  // 495
    .scl_factor = 0,
    .offx = 0,
    .offy = 0,
    .screen = NULL,
    .renderer = NULL,
    .overlay = NULL};

void open_window(Display* d, uint8_t show_window)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        fprintf(stderr, "Failed to initialize SDL: %s\n", SDL_GetError());
        exit(-1);
    }
    Uint32 window_flags = SDL_WINDOW_RESIZABLE;
    if (!show_window)
    {
        window_flags |= SDL_WINDOW_HIDDEN;
    }

    SDL_CreateWindowAndRenderer(d->width, d->height, window_flags, &(d->screen), &(d->renderer));
    if (d->screen == NULL || d->renderer == NULL)
    {
        fprintf(stderr, "Failed to initialize SDL window/renderer: %s\n", SDL_GetError());
        exit(-2);
    }

    resize(d, d->width, d->height);
}

void close_window()
{
    SDL_Quit();
}

void load_overlay(Display* d, const char* filename)
{
    SDL_Surface* image;
    image = SDL_LoadBMP(filename);
    if (image)
    {
        d->overlay = SDL_CreateTextureFromSurface(d->renderer, image);
        SDL_FreeSurface(image);
    }
    else
    {
        fprintf(stderr, "IMG_Load: %s\n", IMG_GetError());
    }
}

void render(const Display* d)
{
    SDL_SetRenderDrawColor(d->renderer, 0, 0, 0, 255);
    SDL_RenderClear(d->renderer);

    int v;
    for (v = 0; v < vector_draw_cnt; v++)
    {
        Uint8 c = vectors_draw[v].color * 256 / VECTREX_COLORS; // [0,254]
        aalineRGBA(d->renderer,
                   d->offx + vectors_draw[v].x0 / d->scl_factor,
                   d->offy + vectors_draw[v].y0 / d->scl_factor,
                   d->offx + vectors_draw[v].x1 / d->scl_factor,
                   d->offy + vectors_draw[v].y1 / d->scl_factor,
                   c, c, c, 0xff);
    }
    if (d->overlay)
    {
        SDL_Rect dest_rect = {d->offx, d->offy, ((double)ALG_MAX_X / (double)(d->scl_factor)), ((double)ALG_MAX_Y / (double)(d->scl_factor))};
        SDL_RenderCopy(d->renderer, d->overlay, NULL, &dest_rect);
    }

    SDL_RenderPresent(d->renderer);
}

void get_pixels(const Display* d, uint8_t* inout_pixels)
{
    SDL_Surface* frame = SDL_CreateRGBSurface(0, d->width, d->height, 32, R_MASK, G_MASK, B_MASK, A_MASK);

    if (frame == NULL)
    {
        fprintf(stderr, "Surface creation failed: %s\n", SDL_GetError());
        return;
    }

    if (SDL_RenderReadPixels(d->renderer, NULL, (Uint32)0, frame->pixels, frame->pitch) != 0)
    {
        fprintf(stderr, "Screenshot creation failed: %s\n", SDL_GetError());
        SDL_FreeSurface(frame);
        return;
    }

    for (size_t i = 0; i <= (d->height * d->width); ++i)
    {
        Uint32 pixel = ((Uint32*)(frame->pixels))[i];
        // convert 4-byte ARGB to 1-byte grayscale
        inout_pixels[i] = GET_GRAYSCALE_VAL(pixel);
    }

    SDL_FreeSurface(frame);
}

void resize(Display* d, int width, int height)
{
    uint16_t sclx, scly;

    uint16_t screenx = width;
    uint16_t screeny = height;

    sclx = ALG_MAX_X / width;
    scly = ALG_MAX_Y / height;

    d->scl_factor = sclx > scly ? sclx : scly;

    d->offx = (screenx - ALG_MAX_X / d->scl_factor) / 2;
    d->offy = (screeny - ALG_MAX_Y / d->scl_factor) / 2;
}
