#ifndef __LED_BLINKY__
#define __LED_BLINKY__

#include <Arduino.h>
#include "global.h"

// #define LED_GPIO 48 // for yolo_uno board
#define LED_GPIO 32 // for my board
#define FAST_PERIOD 250  
#define NORMAL_PERIOD 500  
#define SLOW_PERIOD 1000 

void led_blinky(void *pvParameters);

#endif