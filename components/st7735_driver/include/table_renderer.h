#pragma once
#include <stdint.h>
#include "graphics.h"

typedef struct {
    uint16_t x_origin;
    uint16_t y_origin;
    uint16_t width;
    uint16_t height;
    uint8_t rows;
    uint8_t cols;
    uint16_t grid_color;
    uint16_t header_bg_color;
    uint16_t header_text_color;
    uint16_t cell_text_color;
    uint16_t *col_widths; // Array dinâmico ou fixo definindo largura % ou px
} table_t;

void table_init(table_t *tbl, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t r, uint8_t c);
void table_draw_grid(table_t *tbl);
void table_set_header(table_t *tbl, uint8_t col, const char *text);
void table_set_cell(table_t *tbl, uint8_t row, uint8_t col, const char *text);