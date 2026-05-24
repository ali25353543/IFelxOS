#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdint.h>

void graphics_init(uint32_t* fb, uint32_t width, uint32_t height, uint32_t pitch);
void putpixel(int x, int y, uint32_t color);
void clear_screen(uint32_t color);

#endif
