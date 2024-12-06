#include "StoreMutex.h"

SemaphoreHandle_t StoreMutex::mutex = xSemaphoreCreateMutex();

bool StoreMutex::take() {
    return xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE;
}

bool StoreMutex::give() {
    return xSemaphoreGive(mutex);
}
