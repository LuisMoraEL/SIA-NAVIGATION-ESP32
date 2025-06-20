#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_mac.h"
#include "esp_log.h"
#include "esp_private/esp_clk.h"

static const char *TAG = "MAIN_TAG";

void show_esp_info(void);

void app_main(void)
{
    printf("\n ----------> SIA NAVIGATION ESP32 <---------- \n");

    show_esp_info();

    while (1) {
        printf(". \n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    
}

void show_esp_info() {
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);

    uint32_t flash_size;
    esp_flash_get_size(NULL, &flash_size);

    uint8_t mac[6];
    esp_read_mac(mac, ESP_MAC_WIFI_STA);

    ESP_LOGI(TAG, "===== INFO DEL CHIP ESP32 =====");
    ESP_LOGI(TAG, "Modelo: ESP32");
    ESP_LOGI(TAG, "Núcleos: %d", chip_info.cores);
    ESP_LOGI(TAG, "WiFi%s%s",
             (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
             (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");
    ESP_LOGI(TAG, "Silicio revisión: %d", chip_info.revision);
    ESP_LOGI(TAG, "Flash: %luMB %s", flash_size / (1024 * 1024),
             (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "interna" : "externa");

    ESP_LOGI(TAG, "IDF Version: %s", esp_get_idf_version());

    ESP_LOGI(TAG, "Dirección MAC (WIFI STA): %02X:%02X:%02X:%02X:%02X:%02X",
             mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    ESP_LOGI(TAG, "Ciclo de CPU a %d MHz", (int)(esp_clk_cpu_freq() / 1000000));

#if CONFIG_IDF_TARGET_ESP32
    ESP_LOGI(TAG, "Este es un ESP32 clásico (no ESP32-S, ESP32-C, etc.)");
#endif

    ESP_LOGI(TAG, "================================");
}