#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

const uint32_t print_task_stack = 1024;
const uint32_t print_task_priority = configMAX_PRIORITIES;

void print_task(void) {
    while(1) {
        printf("hallo welt\n");
        fflush(stdout);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void app_main(void)
{
    xTaskCreatePinnedToCore(&print_task, "Print task", print_task_stack, NULL, print_task_priority, NULL, 0);
    return;
}

