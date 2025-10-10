#include "temp_humi_monitor.h"

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(33,16,2);

void temp_humi_monitor(void *pvParameters){
	TickType_t lastWakeTime = xTaskGetTickCount();
  	const TickType_t period = pdMS_TO_TICKS(1000 * 10);

    // Wire.begin(11, 12);
    // Serial.begin(115200);
    dht.begin();

    while (1){
        dht.read();
        // Reading data
        float temperature = dht.readTemperature();
        float humidity = dht.readHumidity();

        // Check if any reads failed and exit early
        if(isnan(temperature) || isnan(humidity)) {
            Serial.println("Failed to read from DHT sensor!");
            temperature = humidity =  -1;
            //return;
        }

        //Update global variables for temperature and humidity
		if(xSemaphoreTake(mutexForTempData, portMAX_DELAY) == pdTRUE) {
			glob_temperature = temperature;
			xSemaphoreGive(mutexForTempData);
		} else {
			Serial.println(ERR_TEMP_TIMEOUT);
		}
        if(xSemaphoreTake(mutexForHumiData, portMAX_DELAY) == pdTRUE) {
            glob_humidity = humidity;
            xSemaphoreGive(mutexForHumiData);
        } else {
			Serial.println(ERR_HUMI_TIMEOUT);
		}

        // Print the results
        Serial.print("Humidity: ");
        Serial.print(humidity);
        Serial.print("% - Temperature: ");
        Serial.print(temperature);
        Serial.println("°C");
        
		vTaskDelayUntil(&lastWakeTime, period);
    }
}