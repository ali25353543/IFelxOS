#include "multiboot2.h"

#include <stdint.h>

// globals (framebuffer state)
uint32_t* fb = 0;
uint32_t fb_width = 0;
uint32_t fb_height = 0;
uint32_t fb_pitch = 0;
uint8_t  fb_bpp = 0;

void parse_multiboot2(uint32_t magic, void* info)
{
    if (magic != MULTIBOOT2_MAGIC) {
        while (1) __asm__("hlt");
    }

    uint8_t* ptr = (uint8_t*)info;

    // skip total_size + reserved
    ptr += 8;

    while (1) {
        multiboot_tag_t* tag = (multiboot_tag_t*)ptr;

        // end tag
        if (tag->type == MULTIBOOT2_TAG_TYPE_END)
            break;

        // framebuffer tag
        if (tag->type == MULTIBOOT2_TAG_TYPE_FRAMEBUFFER) {

            multiboot_tag_framebuffer_t* fbtag =
                (multiboot_tag_framebuffer_t*)tag;

            fb = (uint32_t*)(uintptr_t)fbtag->framebuffer_addr;
            fb_width  = fbtag->framebuffer_width;
            fb_height = fbtag->framebuffer_height;
            fb_pitch  = fbtag->framebuffer_pitch;
            fb_bpp    = fbtag->framebuffer_bpp;
        }

        // align to 8 bytes (important in multiboot2 spec)
        ptr += (tag->size + 7) & ~7;
    }
}
