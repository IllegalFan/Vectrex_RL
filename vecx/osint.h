#ifndef __OSINT_H
#define __OSINT_H

/*extern char gbuffer[1024];*/

void init();

void osint_render (void);

void osint_emuloop();
void osint_emu(unsigned int emu_frames);

void resize(int width, int height);

void load_overlay(const char *filename);

void open_window();
void close_window();

#endif
