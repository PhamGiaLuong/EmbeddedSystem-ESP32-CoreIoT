#ifndef __NEO_BLINKY__
#define __NEO_BLINKY__

#include "global.h"
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define NEO_PIN 45 // for yolo_uno board
// #define NEO_PIN 2 // for my board
#define LED_COUNT 1 

void neo_blinky(void *pvParameters);

#endif