#ifndef __LED_BLINKY__
#define __LED_BLINKY__

#include <Arduino.h>
#include "global.h"

// #define LED_GPIO 48 // for yolo_uno board
#define LED_GPIO 32 // for my board

void led_blinky(void *pvParameters);

#endif