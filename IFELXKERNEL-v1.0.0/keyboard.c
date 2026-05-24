#include "font.h"
#include "keyboard.h"
#include "graphics.h"
#include "vga.h"
#include <stdint.h>
#include <stdbool.h>

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

// scancode to ascii (US QWERTY), index = scancode, shift=0 or 1
static const char scancode_table[2][128] = {
    // no shift
    {
        0,    27,   '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 8,  9,
        'q',  'w',  'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\\', 0, 'a', 's',
        'd',  'f',  'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,   '#', 'z', 'x', 'c', 'v',
        'b',  'n',  'm', ',', '.', '/', 0,   '*', 0,   ' ', 0,   0,   0,   0,   0,   0,
        0,    0,    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,    0,    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,    0,    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,    0,    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
    },
    // shift
    {
        0,    27,   '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', 8,  9,
        'Q',  'W',  'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '|',  0, 'A', 'S',
        'D',  'F',  'G', 'H', 'J', 'K', 'L', ':', '\"', '~', 0,   '~', 'Z', 'X', 'C', 'V',
        'B',  'N',  'M', '<', '>', '?', 0,   '*', 0,   ' ', 0,   0,   0,   0,   0,   0,
        0,    0,    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,    0,    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,    0,    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,    0,    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
    }
};

#define FONT_W 8
#define FONT_H_PX 16
#define LINE_SPACING 2

int cursor_x = 0;
int cursor_y = 0;
static const uint32_t fg_color = 0x00FFFFFF; /* white */
static const uint32_t bg_color = 0x00000000; /* black */
static bool cursor_visible = false;

/* forward declare erase_rect so callers above its definition compile cleanly */
static inline void erase_rect(int x, int y, int w, int h);

static inline void draw_cursor_block(void)
{
    if (cursor_visible) return;
    for (int yy = 0; yy < FONT_H_PX; yy++) {
        for (int xx = 0; xx < FONT_W; xx++) {
            putpixel(cursor_x + xx, cursor_y + yy, fg_color);
        }
    }
    cursor_visible = true;
}

static inline void hide_cursor_block(void)
{
    if (!cursor_visible) return;
    erase_rect(cursor_x, cursor_y, FONT_W, FONT_H_PX);
    cursor_visible = false;
}

static inline void erase_rect(int x, int y, int w, int h)
{
    for (int yy = 0; yy < h; yy++) {
        for (int xx = 0; xx < w; xx++) {
            putpixel(x + xx, y + yy, bg_color);
        }
    }
}

void keyboard_readline(char* buf, size_t maxlen) {
    if (!buf || maxlen == 0) return;
    size_t i = 0;
    static bool shift = false;

    /* Initialize buffer */
    buf[0] = '\0';

    while (i + 1 < maxlen) {
        /* Show cursor while waiting for input */
        draw_cursor_block();

        /* Wait for data to be available */
        while ((inb(0x64) & 0x01) == 0);

        /* Hide cursor while processing this scancode */
        hide_cursor_block();

        uint8_t sc = inb(0x60);
        if (sc == 0) {
            /* redraw cursor and continue */
            draw_cursor_block();
            continue;
        }

        bool key_pressed = !(sc & 0x80);
        sc &= 0x7F;

        switch (sc) {
            case 0x2A:  /* Left Shift */
            case 0x36:  /* Right Shift */
                shift = key_pressed;
                break;
            default:
                if (key_pressed) {
                    if (sc == 0x0E) { /* Backspace */
                        if (i > 0) {
                            i--;
                            if (cursor_x >= FONT_W) {
                                cursor_x -= FONT_W;
                            } else {
                                cursor_x = 0;
                            }
                            erase_rect(cursor_x, cursor_y, FONT_W, FONT_H_PX);
                            buf[i] = '\0';
                            /* redraw cursor at new position */
                            draw_cursor_block();
                        }
                        continue;
                    } else if (sc == 0x1C) { /* Enter */
                        /* Move to next line */
                        cursor_x = 0;
                        cursor_y += FONT_H_PX + LINE_SPACING;
                        buf[i] = '\0';
                        return;
                    } else if (sc < 128) {
                        if (i + 1 < maxlen) {
                            char c = scancode_table[shift ? 1 : 0][sc];
                            if (c >= 32 && c <= 126) {
                                buf[i++] = c;
                                buf[i] = '\0';
                                /* Draw character and advance cursor */
                                draw_char(cursor_x, cursor_y, c, fg_color);
                                cursor_x += FONT_W;
                                /* draw cursor at next position */
                                draw_cursor_block();
                            }
                        }
                    }
                }
                break;
        }
    }

    buf[i] = '\0';
    /* finalize with newline on overflow */
    cursor_x = 0;
    cursor_y += FONT_H_PX + LINE_SPACING;
    return;
}

bool keyboard_check_esc(void) {
    uint8_t status = inb(0x64);
    if (status & 0x01) { // إذا كان هناك بيانات متاحة
        uint8_t scancode = inb(0x60);
        return (scancode == 0x01); // 0x01 هو scancode مفتاح ESC
    }
    return false;
}
