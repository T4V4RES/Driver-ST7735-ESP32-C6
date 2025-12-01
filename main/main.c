#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
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

    ESP_LOGI(TAG, "A testar cores...");
    // Teste: Vermelho

    st7735_fill_screen(ST7735_RED);
    vTaskDelay(pdMS_TO_TICKS(1000));
    // Teste: Verde

    st7735_fill_screen(ST7735_GREEN);
    vTaskDelay(pdMS_TO_TICKS(1000));
    // Teste: Azul

    st7735_fill_screen(ST7735_BLUE);
    vTaskDelay(pdMS_TO_TICKS(1000));
    // Teste: Branco

    st7735_fill_screen(ST7735_WHITE);
    vTaskDelay(pdMS_TO_TICKS(2000));


    // Loop infinito - manter o programa a correr
    while (1) {
    
        ESP_LOGI(TAG, "A desenhar linhas...");
        st7735_fill_screen(ST7735_BLACK);
        
        // Desenha uma linha horizontal a azul (do inicio ao fim)
        st7735_fill_rect(0, 20, 160, 1, ST7735_BLUE);
        vTaskDelay(pdMS_TO_TICKS(1000));

        // =============================================
        ESP_LOGI(TAG, "A escrever texto...");
        st7735_fill_screen(ST7735_BLACK);
        
        // Escreve uma palavra no ecrã
        st7735_draw_string(0, 30, "Dispositivos Conectados", ST7735_WHITE, ST7735_BLACK, 1);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}