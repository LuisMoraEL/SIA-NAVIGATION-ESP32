#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_chip_info.h"
#include "esp_mac.h"
#include "esp_log.h"
#include "esp_private/esp_clk.h"

#include "driver/i2c.h"

// sensores
#include "bmp180.h"
#include "mpu6050.h"

static const char *TAG = "MAIN_TAG";

// Definiciones y constantes
#define I2C_SPEED_FAST 400000

// Declaración de métodos
void show_esp_info(void);
esp_err_t init_i2c(void);

void app_main(void)
{
    printf("\n ----------> SIA NAVIGATION ESP32 <---------- \n");

    show_esp_info();

    ESP_ERROR_CHECK(init_i2c());

    while (1) {
        printf(". \n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    
}

// Inicialización y configuración de I2C
esp_err_t init_i2c() {
    i2c_config_t i2c_config = {};

    // Definición de la configuración i2c
    i2c_config.mode = I2C_MODE_MASTER;
    i2c_config.sda_io_num = 21;
    i2c_config.scl_io_num = 22;
    i2c_config.sda_pullup_en = true;
    i2c_config.scl_pullup_en = true;
    i2c_config.master.clk_speed = I2C_SPEED_FAST;
    i2c_config.clk_flags = 0;

    // implementación de la configuración
    ESP_ERROR_CHECK(i2c_param_config(I2C_NUM_0, &i2c_config));
    ESP_ERROR_CHECK(i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, ESP_INTR_FLAG_LEVEL1));

    return ESP_OK;
}

void show_esp_info() {
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);

    uint8_t mac[6];
    esp_read_mac(mac, ESP_MAC_WIFI_STA);

    ESP_LOGI(TAG, "===== INFO DEL CHIP ESP32 =====");
    ESP_LOGI(TAG, "Modelo: ESP32");
    ESP_LOGI(TAG, "Núcleos: %d", chip_info.cores);
    ESP_LOGI(TAG, "WiFi%s%s",
             (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
             (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");
    ESP_LOGI(TAG, "Silicio revisión: %d", chip_info.revision);
    ESP_LOGI(TAG, "IDF Version: %s", esp_get_idf_version());

    ESP_LOGI(TAG, "Dirección MAC (WIFI STA): %02X:%02X:%02X:%02X:%02X:%02X",
             mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    ESP_LOGI(TAG, "Ciclo de CPU a %d MHz", (int)(esp_clk_cpu_freq() / 1000000));

#if CONFIG_IDF_TARGET_ESP32
    ESP_LOGI(TAG, "Este es un ESP32 clásico (no ESP32-S, ESP32-C, etc.)");
#endif

    ESP_LOGI(TAG, "================================");
}