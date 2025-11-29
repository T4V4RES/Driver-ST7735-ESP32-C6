idf_component_register(
    SRCS "main.c"
    INCLUDE_DIRS "."
    REQUIRES st7735_driver driver esp_driver_gpio
)
