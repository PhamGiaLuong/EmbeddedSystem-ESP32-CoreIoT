#include "led_blinky.h"

void led_blinky(void *pvParameters){
	TickType_t lastWakeTime = xTaskGetTickCount();
  	TickType_t period = pdMS_TO_TICKS(NORMAL_PERIOD);
    pinMode(LED_GPIO, OUTPUT);
    bool ledState = false;
  
    while(1) {       
        if(xSemaphoreTake(mutexForTempData, portMAX_DELAY) == pdTRUE) {
            if(glob_temperature == -1) {
                period = pdMS_TO_TICKS(FAST_PERIOD);
            } else if(glob_temperature >= 30) {
                period = pdMS_TO_TICKS(SLOW_PERIOD);
            } else {
                period = pdMS_TO_TICKS(NORMAL_PERIOD);
            }
            xSemaphoreGive(mutexForTempData);
        } else {
            Serial.println(ERR_TEMP_TIMEOUT);
        }
        vTaskDelayUntil(&lastWakeTime, period);

        ledState = !ledState;
        digitalWrite(LED_GPIO, ledState);
    }
}