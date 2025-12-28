// console.c
#include "console.h"
#include "../text/fonts.h"
#include "../io.h"
#include "../idt/idt.h" // só se precisar do scancode_map


int cursor_x = 8;
int cursor_y = 0;

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200
#define CHAR_WIDTH 8
#define CHAR_HEIGHT 8

// você já tem scancode_map
const char scancode_map[128] = {
    0, 27, '1','2','3','4','5','6','7','8','9','0','-','=','\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n', 0,
    'a','s','d','f','g','h','j','k','l',';','\'','`', 0,
    '\\','z','x','c','v','b','n','m',',','.','/', 0, '*', 0,
    ' ', /* e resto zeros */
};

// Scroll: move todo o conteúdo 1 linha pra cima
static void console_scroll() {
    // simples: copia linhas para cima
    for (int y = CHAR_HEIGHT; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            uint8* vga = (uint8*)0xA0000;
            vga[y * SCREEN_WIDTH + x] = vga[(y - CHAR_HEIGHT) * SCREEN_WIDTH + x];
        }
    }
    // limpa última linha
    for (int x = 0; x < SCREEN_WIDTH; x++) {
        uint8* vga = (uint8*)0xA0000;
        vga[(SCREEN_HEIGHT - CHAR_HEIGHT) * SCREEN_WIDTH + x] = 0;
    }
    cursor_y -= CHAR_HEIGHT;
}

void console_newline() {
    cursor_x = 8;
    cursor_y += CHAR_HEIGHT;
    if (cursor_y >= SCREEN_HEIGHT) {
        console_scroll();
    }
}

void console_put_char(int8 scancode) {
    char c = scancode_map[scancode];
    if (!c) return; // tecla não mapeada
    if (c == '\n') {
        console_newline();
    } else if (c == '\b') {
        // backspace
        if (cursor_x > 8) cursor_x -= CHAR_WIDTH;
        draw_text(cursor_x, cursor_y, " ", 1); // apaga char
    } else {
        char buf[2] = { c, 0 };
        draw_text(cursor_x, cursor_y, buf, 1);
        cursor_x += CHAR_WIDTH;
        if (cursor_x >= SCREEN_WIDTH) console_newline();
    }
}

void console_put_string(const char* str) {
    while (*str) {
        char c = *str++;
        if (c == '\n') console_newline();
        else {
            char buf[2] = { c, 0 };
            draw_text(cursor_x, cursor_y, buf, 1);
            cursor_x += CHAR_WIDTH;
            if (cursor_x >= SCREEN_WIDTH) console_newline();
        }
    }
}
