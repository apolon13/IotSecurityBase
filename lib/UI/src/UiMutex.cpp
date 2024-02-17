#include "UiMutex.h"

auto mutex = xSemaphoreCreateMutex();

bool UiMutex::take() {
    return xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE;
}

bool UiMutex::give() {
    return xSemaphoreGive(mutex);
}
