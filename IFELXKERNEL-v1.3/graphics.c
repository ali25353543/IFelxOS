#include "graphics.h"

static uint32_t* framebuffer;
static uint32_t screen_width;
static uint32_t screen_height;
static uint32_t pitch;

void graphics_init(uint32_t* fb,
                   uint32_t width,
                   uint32_t height,
                   uint32_t fb_pitch)
{
    framebuffer = fb;
    screen_width = width;
    screen_height = height;
    pitch = fb_pitch;
}

void putpixel(int x, int y, uint32_t color)
{
    if (x < 0 || y < 0)
        return;

    if ((uint32_t)x >= screen_width ||
        (uint32_t)y >= screen_height)
        return;

    // حساب صحيح يعتمد على البايت والـ pitch
    uint8_t* pixel = (uint8_t*)framebuffer
                   + y * pitch
                   + x * 4;

    *(uint32_t*)pixel = color;
}

void clear_screen(uint32_t color)
{
    for (uint32_t y = 0; y < screen_height; y++) {
        uint32_t* row_ptr = (uint32_t*)((uint8_t*)framebuffer + y * pitch);
        
        for (uint32_t x = 0; x < screen_width; x++) {
            row_ptr[x] = color;
        }
    }
}