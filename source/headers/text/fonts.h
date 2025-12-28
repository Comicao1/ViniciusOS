#ifndef FONTS_H
#define FONTS_H

#include "../types.h"

extern int8 font8x8_basic[128][8];
extern uint8* vga;

void put_pixel(int16 x, int16 y, int32 color);
void draw_char(int32 x, int32 y, int8 c, int32 color);
void draw_text(int32 x, int32 y, const int8* text, int32 color);
void clear_buffer(uint8 color);

#endif