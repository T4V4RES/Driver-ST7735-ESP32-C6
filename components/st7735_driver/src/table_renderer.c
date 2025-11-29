#include "table_renderer.h"
#include "st7735.h"
#include <string.h>

void table_init(table_t *tbl, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t r, uint8_t c) {
    tbl->x_origin = x;
    tbl->y_origin = y;
    tbl->width = w;
    tbl->height = h;
    tbl->rows = r;
    tbl->cols = c;
    tbl->grid_color = ST7735_WHITE;
    tbl->header_bg_color = ST7735_BLUE;
    tbl->header_text_color = ST7735_WHITE;
    tbl->cell_text_color = ST7735_GREEN;
    tbl->col_widths = NULL;
}

void table_draw_grid(table_t *tbl) {
    uint16_t row_h = tbl->height / (tbl->rows + 1); // +1 para cabeçalho
    uint16_t col_w = tbl->width / tbl->cols;       // Assumindo largura fixa por enquanto

    // Desenha bordas externas
    draw_rect(tbl->x_origin, tbl->y_origin, tbl->width, tbl->height, tbl->grid_color);

    // Linhas horizontais
    for (int i = 1; i <= tbl->rows; i++) {
        uint16_t y = tbl->y_origin + (i * row_h);
        draw_hline(tbl->x_origin, y, tbl->width, tbl->grid_color);
    }

    // Linhas verticais
    for (int i = 1; i < tbl->cols; i++) {
        uint16_t x = tbl->x_origin + (i * col_w);
        draw_vline(x, tbl->y_origin, tbl->height, tbl->grid_color);
    }
}

void table_set_cell(table_t *tbl, uint8_t row, uint8_t col, const char *text) {
    if (row >= tbl->rows || col >= tbl->cols) return;

    uint16_t row_h = tbl->height / (tbl->rows + 1);
    uint16_t col_w = tbl->width / tbl->cols;

    // Calcula coordenadas da célula (considerando +1 row para header)
    uint16_t cell_x = tbl->x_origin + (col * col_w) + 1;
    uint16_t cell_y = tbl->y_origin + ((row + 1) * row_h) + 1;
    uint16_t cell_w = col_w - 2;
    uint16_t cell_h = row_h - 2;

    // 1. Limpa a célula (preenche com cor de fundo, ex: Preto)
    st7735_fill_rect(cell_x, cell_y, cell_w, cell_h, ST7735_BLACK);

    // 2. Desenha o texto centralizado (cálculo simplificado)
    // Assumindo fonte 5x7 (largura 6 com espaçamento, altura 8)
    int text_len = strlen(text);
    int text_pixel_w = text_len * 6;
    int text_pixel_h = 8;

    int pos_x = cell_x + (cell_w - text_pixel_w) / 2;
    int pos_y = cell_y + (cell_h - text_pixel_h) / 2;

    if (pos_x < cell_x) pos_x = cell_x; // Clip simples

    draw_string(pos_x, pos_y, text, tbl->cell_text_color, ST7735_BLACK, 1);
}

void table_set_header(table_t *tbl, uint8_t col, const char *text) {
    if (col >= tbl->cols) return;

    uint16_t row_h = tbl->height / (tbl->rows + 1);
    uint16_t col_w = tbl->width / tbl->cols;

    // Cabeçalho está na primeira linha (row 0 visual)
    uint16_t cell_x = tbl->x_origin + (col * col_w) + 1;
    uint16_t cell_y = tbl->y_origin + 1;
    uint16_t cell_w = col_w - 2;
    uint16_t cell_h = row_h - 2;

    // Preenche com cor de fundo do cabeçalho
    st7735_fill_rect(cell_x, cell_y, cell_w, cell_h, tbl->header_bg_color);

    // Desenha o texto centralizado
    int text_len = strlen(text);
    int text_pixel_w = text_len * 6;
    int text_pixel_h = 8;

    int pos_x = cell_x + (cell_w - text_pixel_w) / 2;
    int pos_y = cell_y + (cell_h - text_pixel_h) / 2;

    if (pos_x < cell_x) pos_x = cell_x;

    draw_string(pos_x, pos_y, text, tbl->header_text_color, tbl->header_bg_color, 1);
}