#ifndef CONSOLE_H
#define CONSOLE_H
#include "../types.h"

extern const char scancode_map[128];
// ISR de teclado
extern int cursor_x;
extern int cursor_y;

void console_put_char(int8 scancode);
void console_put_string(const char* str);
void console_newline();

#endif