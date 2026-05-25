#ifndef MULTIBOOT_H
#define MULTIBOOT_H

#include <stdint.h>

#define MULTIBOOT2_MAGIC 0x36d76289

#define MULTIBOOT2_TAG_TYPE_END         0
#define MULTIBOOT2_TAG_TYPE_FRAMEBUFFER 8

typedef struct multiboot_tag {
    uint32_t type;
    uint32_t size;
} __attribute__((packed)) multiboot_tag_t;

typedef struct multiboot_tag_framebuffer {
    uint32_t type;
    uint32_t size;

    uint64_t framebuffer_addr;
    uint32_t framebuffer_pitch;
    uint32_t framebuffer_width;
    uint32_t framebuffer_height;
    uint8_t  framebuffer_bpp;
    uint8_t  framebuffer_type;
    uint16_t reserved;
} __attribute__((packed)) multiboot_tag_framebuffer_t;

// هذه القيم نحتاجها في الرسم لاحقاً
extern uint32_t* fb;
extern uint32_t fb_width;
extern uint32_t fb_height;
extern uint32_t fb_pitch;
extern uint8_t  fb_bpp;

void parse_multiboot2(uint32_t magic, void* info);

#endif
