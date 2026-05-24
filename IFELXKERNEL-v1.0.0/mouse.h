#pragma once

#include <stdint.h>

void mouse_init();
void mouse_interrupt();

extern int mouse_x;
extern int mouse_y;