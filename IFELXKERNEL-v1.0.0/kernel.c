#include "keyboard.h"
#include "multiboot2.h"
#include "graphics.h"
#include "font.h"
#include "commands.h"
#include <string.h>

#define PROMPT "> "
#define PROMPT_COLOR 0x00FFFF00
#define CHAR_W 8
#define LINE_SPACING 2

void kernel_main(uint32_t magic, void* mbi)
{
    parse_multiboot2(magic, mbi);

    if (!fb) {
        while (1) __asm__("hlt");
    }

    graphics_init(fb, fb_width, fb_height, fb_pitch);
    clear_screen(0x000000);

    /* Initialize cursor position */
    cursor_x = 0;
    cursor_y = 0;

    while (1) {
        /* Draw prompt */
        draw_string(cursor_x, cursor_y, PROMPT, PROMPT_COLOR);

        /* set input start after prompt */
        size_t prompt_len = strlen(PROMPT);
        cursor_x += (int)(prompt_len * CHAR_W);

        /* read a line into buffer */
        char buf[1024];
        keyboard_readline(buf, sizeof(buf));

        /* handle the command; commands_handle is expected to draw output
           at current `cursor_x`/`cursor_y` positions or update them */
        if (!commands_handle(buf)) {
            draw_string(cursor_x, cursor_y, "Unknown command: ", 0x00FFFFFF);
            draw_string(cursor_x, cursor_y, buf, 0x00FFFFFF);
            draw_string(cursor_x, cursor_y, "\n", 0x00FFFFFF);
        }
    }
}
