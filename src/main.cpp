#include <Arduino.h>
#include <Wire.h>
#include "DHT20.h"
#include "DHT.h"

#define DHTPIN GPIO_NUM_22     
#define DHTTYPE DHT11  

DHT dht11(DHTPIN, DHTTYPE);

void TaskLEDControl(void *pvParameters) {
  pinMode(GPIO_NUM_32, OUTPUT); // Initialize LED pin
  int ledState = 0;
  while(1) {
    
    if (ledState == 0) {
      digitalWrite(GPIO_NUM_32, HIGH); // Turn ON LED
    } else {
      digitalWrite(GPIO_NUM_32, LOW); // Turn OFF LED
    }
    ledState = 1 - ledState;
    vTaskDelay(2000);
  }
}

// void TaskTemperature_Humidity(void *pvParameters) {
//   DHT20 dht20;
//   Wire.begin(GPIO_NUM_21, GPIO_NUM_22);
//   dht20.begin();
  
//   while(1) {
//     dht20.read();
//     double temperature = dht20.getTemperature();
//     double humidity = dht20.getHumidity();

//     Serial.printf("Temp: %.1f *C", temperature);
//     Serial.printf(" Humidity: %.1f %%\n", humidity);
//     Serial.println();

//     vTaskDelay(5000);
//   }
// }

void TaskTemperature_Humidity(void *pvParameters) {
  dht11.begin();

  while (1) {
    float temperature = dht11.readTemperature();  
    float humidity = dht11.readHumidity();        

    // Kiểm tra lỗi đọc
    if (isnan(temperature) || isnan(humidity)) {
      Serial.println("Can not read data from DHT11!");
    } else {
      Serial.printf("Temp: %.1f *C - Humidity: %.1f %%\n", temperature, humidity);
    }

    vTaskDelay(pdMS_TO_TICKS(5000)); 
  }
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  xTaskCreate(TaskLEDControl, "LED Control", 2048, NULL, 2, NULL);
  xTaskCreate(TaskTemperature_Humidity, "Temperature & Humidity", 2048, NULL, 2, NULL);
}

void loop() {}