#ifndef __TEMP_HUMI_MONITOR__
#define __TEMP_HUMI_MONITOR__

#include <Arduino.h>
#include "LiquidCrystal_I2C.h"
#include "DHT20.h"
#include "global.h"

// #define DHTPIN 22   //for my board
#define DHTPIN 8 // for yolouno
#define DHTTYPE DHT20  

void temp_humi_monitor(void *pvParameters);

#endif