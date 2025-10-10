#ifndef __TEMP_HUMI_MONITOR__
#define __TEMP_HUMI_MONITOR__

#include <Arduino.h>
#include "LiquidCrystal_I2C.h"
#include "DHT.h"
#include "global.h"

#define DHTPIN 22   
#define DHTTYPE DHT11  

void temp_humi_monitor(void *pvParameters);

#endif