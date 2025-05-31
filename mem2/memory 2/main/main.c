#include <stdio.h>
#include <string.h>
#include "esp_system.h"
#include "esp_flash.h"
#include "esp_partition.h"
#include "spi_flash_mmap.h"

#define PARTITION_NAME "storage"  // Partition name for flash storage

void flash_info() {
    printf("Getting flash information...\n");

    // Get default flash chip
    esp_flash_t *flash = esp_flash_default_chip;
    
    // Read Flash ID (Chip information)
    uint32_t flash_id = 0;
    esp_err_t err = esp_flash_read_id(flash, &flash_id);
    if (err == ESP_OK) {
        printf("Flash ID: 0x%lX\n", flash_id);
    } else {
        printf("Failed to read flash ID: %s\n", esp_err_to_name(err));
    }

    // Get flash size
    uint32_t size = 0;
    err = esp_flash_get_size(flash, &size);
    if (err == ESP_OK) {
        printf("Flash size: %lu bytes (%lu MB)\n", size, size / 1024 / 1024);
    } else {
        printf("Failed to get flash size: %s\n", esp_err_to_name(err));
    }
}

void flash_test() {
    const char *test_data = "Hello ESP32-S3!";
    size_t data_len = strlen(test_data) + 1;
    esp_err_t err;

    // List available partitions
    printf("Available partitions:\n");
    esp_partition_iterator_t it = esp_partition_find(ESP_PARTITION_TYPE_ANY, ESP_PARTITION_SUBTYPE_ANY, NULL);
    while (it != NULL) {
        const esp_partition_t *part = esp_partition_get(it);
        printf("  - %s (type: %d, subtype: %d, address: 0x%lx, size: %ld bytes)\n", 
               part->label, part->type, part->subtype, part->address, part->size);
        it = esp_partition_next(it);
    }
    esp_partition_iterator_release(it);

    // Try to find any data partition we can use (first try "storage", then any data partition)
    const esp_partition_t *part = esp_partition_find_first(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_ANY, PARTITION_NAME);
    if (!part) {
        // If "storage" is not found, try to find any data partition
        part = esp_partition_find_first(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_ANY, NULL);
        if (!part) {
            printf("No suitable data partition found!\n");
            return;
        }
    }

    printf("Using partition '%s' at address 0x%lx\n", part->label, part->address);

    // Erase a small sector of the partition
    size_t erase_size = 4096; // Typically flash erase size is 4KB
    if (erase_size > part->size) {
        erase_size = part->size;
    }
    
    printf("Erasing %zu bytes...\n", erase_size);
    err = esp_partition_erase_range(part, 0, erase_size);
    if (err != ESP_OK) {
        printf("Failed to erase partition: %s\n", esp_err_to_name(err));
        return;
    }

    // Write data to flash
    printf("Writing data...\n");
    err = esp_partition_write(part, 0, test_data, data_len);
    if (err != ESP_OK) {
        printf("Failed to write to flash: %s\n", esp_err_to_name(err));
        return;
    }
    printf("Data written: %s\n", test_data);

    // Read data from flash
    char read_data[32] = {0};
    printf("Reading data...\n");
    err = esp_partition_read(part, 0, read_data, data_len);
    if (err != ESP_OK) {
        printf("Failed to read from flash: %s\n", esp_err_to_name(err));
        return;
    }
    printf("Data read: %s\n", read_data);
}

void app_main() {
    flash_info();
    flash_test();
}
