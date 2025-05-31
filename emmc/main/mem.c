#include <stdio.h>
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "esp_timer.h"
#include "esp_partition.h"

#define TEST_SIZE 4096  // Test size in bytes (4KB)
#define TEST_ITERATIONS 100

static uint8_t test_data[TEST_SIZE];
static uint8_t read_buffer[TEST_SIZE];

void benchmark_flash_speed() {
    const esp_partition_t *partition = esp_partition_find_first(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_ANY, NULL);
    if (!partition) {
        printf("No valid partition found!\n");
        return;
    }
    
    printf("Testing Flash Read/Write Speed\n");
    
    // Fill test data
    for (int i = 0; i < TEST_SIZE; i++) {
        test_data[i] = i % 256;
    }
    
    int64_t start_time, end_time;
    
    // Flash Erase
    printf("Erasing flash...\n");
    start_time = esp_timer_get_time();
    esp_err_t err = esp_partition_erase_range(partition, 0, TEST_SIZE);
    end_time = esp_timer_get_time();
    printf("Erase Time: %lld us\n", end_time - start_time);
    if (err != ESP_OK) {
        printf("Erase failed: %d\n", err);
        return;
    }
    
    // Flash Write (Sequential)
    printf("Writing to flash sequentially...\n");
    start_time = esp_timer_get_time();
    err = esp_partition_write(partition, 0, test_data, TEST_SIZE);
    end_time = esp_timer_get_time();
    printf("Write Time: %lld us\n", end_time - start_time);
    if (err != ESP_OK) {
        printf("Write failed: %d\n", err);
        return;
    }
    
    // Flash Read (Sequential)
    printf("Reading from flash sequentially...\n");
    start_time = esp_timer_get_time();
    err = esp_partition_read(partition, 0, read_buffer, TEST_SIZE);
    end_time = esp_timer_get_time();
    printf("Read Time: %lld us\n", end_time - start_time);
    if (err != ESP_OK) {
        printf("Read failed: %d\n", err);
        return;
    }
    
    // Random Read/Write Test
    printf("Performing random read/write test...\n");
    for (int i = 0; i < TEST_ITERATIONS; i++) {
        int offset = (rand() % (TEST_SIZE / 256)) * 256;
        start_time = esp_timer_get_time();
        err = esp_partition_write(partition, offset, test_data + offset, 256);
        end_time = esp_timer_get_time();
        printf("Random Write (offset %d) Time: %lld us\n", offset, end_time - start_time);
        
        start_time = esp_timer_get_time();
        err = esp_partition_read(partition, offset, read_buffer + offset, 256);
        end_time = esp_timer_get_time();
        printf("Random Read (offset %d) Time: %lld us\n", offset, end_time - start_time);
    }
    
    printf("Benchmark Completed!\n");
}

void app_main() {
    printf("Starting ESP32-S3 Flash Benchmark Test\n");
    benchmark_flash_speed();
}
