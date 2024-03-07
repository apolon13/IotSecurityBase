#include "ProjectPreferencesMutex.h"

SemaphoreHandle_t ProjectPreferencesMutex::mutex = xSemaphoreCreateMutex();

bool ProjectPreferencesMutex::take() {
    return xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE;
}

bool ProjectPreferencesMutex::give() {
    return xSemaphoreGive(mutex);
}
