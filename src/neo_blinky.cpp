#include "neo_blinky.h"

Adafruit_NeoPixel strip(LED_COUNT, NEO_PIN, NEO_GRB + NEO_KHZ800);

void neo_blinky(void *pvParameters){
    strip.begin();
    // Set all pixels to off to start
    strip.setBrightness(50);
    strip.clear();
    strip.show();

    while(1) {                   
        uint8_t r, g, b;       
        Serial.printf("WiFi Connected: %s\n", isWifiConnected ? "Yes" : "No");
        // Check connection status
        if (!isWifiConnected) {
            // Disconnected: Red color
            r = 255; 
            g = 0; 
            b = 0;
        } else {
            // Connected: Check signal strength (RSSI)
            long rssi = WiFi.RSSI();
            Serial.printf("RSSI: %ld dBm\n", rssi);

            if (rssi >= -55) {
                // Strong signal (> -55dBm): Green color
                r = 0; 
                g = 255; 
                b = 0;
            } 
            else if (rssi >= -75) {
                // Medium signal (-55dBm to -75dBm): Yellow color
                r = 255; 
                g = 255; 
                b = 0;
            } 
            else {
                // Weak signal (< -75dBm): Orange color
                r = 255; 
                g = 140; 
                b = 0;
            }
        }

        // Set pixel to the determined color
        strip.setPixelColor(0, strip.Color(r, g, b)); 
        strip.show(); // Update the strip

        // Wait for 1 second
        vTaskDelay(1000);

        // Set the pixel to off
        strip.setPixelColor(0, strip.Color(0, 0, 0)); // Turn pixel 0 off
        strip.show(); // Update the strip

        // Wait for another 1 second
        vTaskDelay(1000);
    }
}