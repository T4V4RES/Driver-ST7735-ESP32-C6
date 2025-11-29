#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "st7735.h"

// Mapeamento de Pinos (conforme o teu hardware)
#define PIN_MOSI 19
#define PIN_MISO 20  // Não usado
#define PIN_CLK  21
#define PIN_CS   22
#define PIN_DC   2
#define PIN_RST  3
#define PIN_BL   15

static const char *TAG = "MAIN_APP";

// Função auxiliar para desenhar linha horizontal
void draw_hline(uint16_t x, uint16_t y, uint16_t w, uint16_t color) {
    st7735_fill_rect(x, y, w, 1, color);
}

// Função auxiliar para desenhar linha vertical
void draw_vline(uint16_t x, uint16_t y, uint16_t h, uint16_t color) {
    st7735_fill_rect(x, y, 1, h, color);
}

void app_main(void)
{
    ESP_LOGI(TAG, "=================================");
    ESP_LOGI(TAG, "Iniciando Display ST7735S Demo");
    ESP_LOGI(TAG, "=================================");

    // 1. Configuração do Driver ST7735
    st7735_config_t cfg = {
       .mosi_io_num = PIN_MOSI,
       .sclk_io_num = PIN_CLK,
       .cs_io_num = PIN_CS,
       .dc_io_num = PIN_DC,
       .rst_io_num = PIN_RST,
       .bl_io_num = PIN_BL,
       .host_id = SPI2_HOST
    };

    // 2. Inicialização do Hardware
    if (st7735_init(&cfg) != ESP_OK) {
        ESP_LOGE(TAG, "Falha critica na inicializacao do display!");
        return;
    }

    ESP_LOGI(TAG, "Display inicializado com sucesso!");
    ESP_LOGI(TAG, "A testar com cores solidas...");

    // TESTE 1: Vermelho
    ESP_LOGI(TAG, "Teste: VERMELHO");
    st7735_fill_screen(ST7735_RED);
    vTaskDelay(pdMS_TO_TICKS(2000));

    // TESTE 2: Verde
    ESP_LOGI(TAG, "Teste: VERDE");
    st7735_fill_screen(ST7735_GREEN);
    vTaskDelay(pdMS_TO_TICKS(2000));

    // TESTE 3: Azul
    ESP_LOGI(TAG, "Teste: AZUL");
    st7735_fill_screen(ST7735_BLUE);
    vTaskDelay(pdMS_TO_TICKS(2000));

    // TESTE 4: Branco
    ESP_LOGI(TAG, "Teste: BRANCO");
    st7735_fill_screen(ST7735_WHITE);
    vTaskDelay(pdMS_TO_TICKS(2000));

    // Limpar com preto e mostrar texto
    ESP_LOGI(TAG, "A mostrar 'Ola!'");
    st7735_fill_screen(ST7735_BLACK);
    vTaskDelay(pdMS_TO_TICKS(100));

    // 4. Mostrar "Ola!" no centro
    st7735_draw_string(50, 35, "Ola!", ST7735_WHITE, ST7735_BLACK, 2);
    vTaskDelay(pdMS_TO_TICKS(2000));

    // 5. Limpar e mostrar tabela de sensores
    st7735_fill_screen(ST7735_BLACK);

    // Desenhar título
    st7735_draw_string(30, 2, "SENSORES", ST7735_CYAN, ST7735_BLACK, 1);
    
    // Desenhar linha separadora do título
    draw_hline(0, 12, 160, ST7735_GRAY);

    // Desenhar cabeçalhos da tabela
    st7735_draw_string(5, 16, "SENSOR", ST7735_WHITE, ST7735_BLACK, 1);
    st7735_draw_string(90, 16, "VALOR", ST7735_WHITE, ST7735_BLACK, 1);
    
    // Linha separadora do cabeçalho
    draw_hline(0, 26, 160, ST7735_GRAY);
    
    // Linha vertical separadora
    draw_vline(80, 12, 68, ST7735_GRAY);

    // Rótulos fixos
    st7735_draw_string(5, 30, "TEMP", ST7735_GREEN, ST7735_BLACK, 1);
    st7735_draw_string(5, 45, "UMID", ST7735_GREEN, ST7735_BLACK, 1);
    st7735_draw_string(5, 60, "STATUS", ST7735_GREEN, ST7735_BLACK, 1);

    ESP_LOGI(TAG, "Loop principal iniciado - atualizando valores...");

    // Variáveis simuladas de sensores
    float temperatura = 24.0f;
    int umidade = 55;
    char buf[16];
    int contador = 0;

    while (1) {
        // Simulação de variação de sensores
        temperatura = 20.0f + (float)(rand() % 150) / 10.0f; // 20.0 a 35.0
        umidade = 40 + (rand() % 40); // 40 a 80

        // Limpar área dos valores (coluna direita)
        st7735_fill_rect(85, 28, 70, 50, ST7735_BLACK);

        // Atualizar temperatura
        snprintf(buf, sizeof(buf), "%.1f C", temperatura);
        st7735_draw_string(90, 30, buf, ST7735_YELLOW, ST7735_BLACK, 1);

        // Atualizar humidade
        snprintf(buf, sizeof(buf), "%d %%", umidade);
        st7735_draw_string(90, 45, buf, ST7735_YELLOW, ST7735_BLACK, 1);

        // Atualizar status com cor condicional
        if (temperatura > 30.0f) {
            st7735_draw_string(90, 60, "ALERTA!", ST7735_RED, ST7735_BLACK, 1);
        } else if (temperatura < 22.0f) {
            st7735_draw_string(90, 60, "FRIO", ST7735_BLUE, ST7735_BLACK, 1);
        } else {
            st7735_draw_string(90, 60, "NORMAL", ST7735_GREEN, ST7735_BLACK, 1);
        }

        contador++;
        ESP_LOGI(TAG, "Atualização #%d: Temp=%.1fC, Umid=%d%%", 
                 contador, temperatura, umidade);

        // Aguarda 2 segundos
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}