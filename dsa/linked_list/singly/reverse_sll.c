// //this code is helpful for reverse entire singly linked list

// #include <stdio.h>
// #include <stdlib.h>

// struct node
// {
//     int data;
//     struct node *next;
// };

// struct node *reverse(struct node *head)
// {
//     struct node *ptr = NULL;
//     struct node *ptr2 = NULL;
//     while (head != NULL)
//     {
//         ptr2 = head->next;
//         head->next = ptr;
//         ptr = head;
//         head = ptr2;
//     }
//     head = ptr;
//     return head;
// }

// void print_ll(struct node *n)
// {
//     while (n != NULL)
//     {
//         printf("%d ->", n->data);
//         n = n->next;
//     }
//     printf("NULL\n");
// }
// int count(struct node *n)
// {
//     struct node *ptr = n;
//     int cnt = 0;
//     while (ptr != NULL)
//     {
//         ptr = ptr->next;
//         cnt++;
//     }
//     return cnt;
// }

// int main()
// {
//     struct node *head = NULL;
//     struct node *second = NULL;
//     struct node *third = NULL;

//     head = (struct node *)malloc(sizeof(struct node));
//     second = (struct node *)malloc(sizeof(struct node));
//     third = (struct node *)malloc(sizeof(struct node));

//     if (head == NULL || second == NULL || third == NULL)
//     {
//         printf("Memory allocation failed\n");
//         return 1;
//     }

//     head->data = 1;
//     head->next = second;

//     second->data = 2;
//     second->next = third;

//     third->data = 3;
//     third->next = NULL;

//     printf("Count of nodes before adding: %d\n", count(head));
//     print_ll(head);

//     head = reverse(head);

//     printf("Count of nodes after adding: %d\n", count(head));
//     print_ll(head);

//     // Free the allocated memory
//     // free(third->next);
//     free(third);
//     free(second);
//     free(head);

//     return 0;
// }
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "nvs.h"

#define STORAGE_NAMESPACE "storage"

// Structure definition for blob data
struct data
{
    char data[32]; // Fixed-size buffer for storing strings
};

// Function to save and increment the restart counter in NVS
esp_err_t save_restart_counter(void)
{
    nvs_handle_t my_handle;
    esp_err_t err;

    // Open NVS handle
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK)
        return err;

    // Read the restart counter (default to 0 if not found)
    int32_t restart_counter = 0;
    err = nvs_get_i32(my_handle, "restart_counter", &restart_counter);
    if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND)
        return err;

    // Increment and save the restart counter
    restart_counter++;
    err = nvs_set_i32(my_handle, "restart_counter", restart_counter);
    if (err != ESP_OK)
        return err;

    // Commit changes
    err = nvs_commit(my_handle);
    nvs_close(my_handle);
    return err;
}

// Function to read and print stored data in NVS
esp_err_t print_stored_data(void)
{
    nvs_handle_t my_handle;
    esp_err_t err;

    // Open NVS handle
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK)
        return err;

    printf("Stored Data:\n");

    // List and print data for slots
    char key[16];
    char value[32];
    size_t required_size = 0;

    for (int i = 1; i <= 3; i++)
    {
        snprintf(key, sizeof(key), "slot_%d", i);
        required_size = sizeof(value);
        err = nvs_get_blob(my_handle, key, value, &required_size);
        if (err == ESP_OK)
        {
            printf("%s: %s\n", key, value);
        }
        else if (err == ESP_ERR_NVS_NOT_FOUND)
        {
            printf("%s: Not Found\n", key);
        }
        else
        {
            printf("Error reading %s: %s\n", key, esp_err_to_name(err));
        }
    }

    nvs_close(my_handle);
    return ESP_OK;
}

void app_main(void)
{
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    // Create and initialize data structures
    struct data d1[3] = {0};
    strcpy(d1[0].data, "data_1");
    strcpy(d1[1].data, "data_2");
    strcpy(d1[2].data, "data_3");

    // Save and print data
    ESP_ERROR_CHECK(save_restart_counter());

    nvs_handle_t my_handle;
    ESP_ERROR_CHECK(nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle));
    ESP_ERROR_CHECK(nvs_set_blob(my_handle, "slot_1", d1[0].data, strlen(d1[0].data) + 1));
    ESP_ERROR_CHECK(nvs_set_blob(my_handle, "slot_2", d1[1].data, strlen(d1[1].data) + 1));
    ESP_ERROR_CHECK(nvs_set_blob(my_handle, "slot_3", d1[2].data, strlen(d1[2].data) + 1));
    ESP_ERROR_CHECK(nvs_commit(my_handle));
    nvs_close(my_handle);

    printf("////////////////////////First Time//////////////////////////\n");
    ESP_ERROR_CHECK(print_stored_data());

    // Erase "slot_2" and print the result
    ESP_ERROR_CHECK(nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle));
    ESP_ERROR_CHECK(nvs_erase_key(my_handle, "slot_2"));
    ESP_ERROR_CHECK(nvs_commit(my_handle));
    nvs_close(my_handle);

    printf("////////////////////////After Slot 2 Erase//////////////////////////\n");
    ESP_ERROR_CHECK(print_stored_data());

    // Update "slot_2" with new data and print the result
    ESP_ERROR_CHECK(nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle));
    strcpy(d1[1].data, "data_2_second_time");
    ESP_ERROR_CHECK(nvs_set_blob(my_handle, "slot_2", d1[1].data, strlen(d1[1].data) + 1));
    ESP_ERROR_CHECK(nvs_commit(my_handle));
    nvs_close(my_handle);

    printf("////////////////////////After Slot 2 Write//////////////////////////\n");
    ESP_ERROR_CHECK(print_stored_data());

    // Infinite loop
    while (1)
    {
        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
}

