#include "led_blinky.h"

void led_blinky(void *pvParameters){
    pinMode(LED_GPIO, OUTPUT);
  
    while(1) {                        
        digitalWrite(LED_GPIO, HIGH);
        vTaskDelay(1000);
        digitalWrite(LED_GPIO, LOW);
        vTaskDelay(1000);
    }
}