#include "mainserver.h"

bool led1_state = false;
bool led2_state = false;
bool isAPMode = true;

WebServer server(80);

unsigned long connect_start_ms = 0;
bool connecting = false;

String mainPage() {
  float temperature = -1;
  if(xSemaphoreTake(mutexForTempData, portMAX_DELAY) == pdTRUE) {
    temperature = glob_temperature;
    xSemaphoreGive(mutexForTempData);
  } else {
    Serial.println(ERR_TEMP_TIMEOUT);
  }
  float humidity = -1;
  if(xSemaphoreTake(mutexForHumiData, portMAX_DELAY) == pdTRUE) {
    humidity = glob_humidity;
    xSemaphoreGive(mutexForHumiData);
  } else {
    Serial.println(ERR_HUMI_TIMEOUT);
  }
  String led1 = led1_state ? "ON" : "OFF";
  String led2 = led2_state ? "ON" : "OFF";

  return R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1.0" />
  <title>ESP32 Dashboard</title>
  <style>
    :root {
      --primary: #00bad1;
      --light: #d6f4f8;
      --bg: #f6f8fa;
      --text: #333;
      --radius: 10px;
      --shadow: 0 2px 8px rgba(214, 244, 248, 1);
    }

    body {
      font-family: "Segoe UI", Roboto, sans-serif;
      background: var(--bg);
      margin: 0;
      padding: 0;
      display: flex;
      justify-content: center;
      align-items: flex-start;
      min-height: 100vh;
    }

    .container {
      width: 95%;
      max-width: 420px;
      background: #fff;
      border-radius: var(--radius);
      box-shadow: var(--shadow);
      margin-top: 50px;
      padding: 24px;
      box-sizing: border-box;
      animation: fadeIn 0.5s ease;
      text-align: center;
    }

    h2 {
      color: var(--primary);
      margin-bottom: 25px;
      font-weight: 600;
    }

    .sensor {
      margin-bottom: 25px;
      font-size: 1.1rem;
      color: var(--text);
    }

    .sensor span {
      display: inline-block;
      min-width: 50px;
      font-weight: bold;
      color: var(--primary);
    }

    .buttons {
      display: flex;
      justify-content: center;
      flex-wrap: wrap;
      gap: 12px;
    }

    button {
      padding: 10px 16px;
      font-size: 1rem;
      border: none;
      border-radius: var(--radius);
      cursor: pointer;
      transition: all 0.3s ease;
      box-shadow: var(--shadow);
      min-width: 120px;
    }

    button:hover {
      transform: translateY(-2px);
    }

    .led-on {
      background: var(--primary);
      color: #fff;
    }

    .led-off {
      background: #eaeaea;
      color: var(--text);
    }

    #settings {
      display: inline-block;
      margin-top: 25px;
      padding: 10px 16px;
      font-size: 0.95rem;
      border-radius: var(--radius);
      background: var(--light);
      color: var(--primary);
      border: none;
      box-shadow: var(--shadow);
      cursor: pointer;
      transition: all 0.3s ease;
    }

    #settings:hover {
      background: var(--primary);
      color: white;
    }

    @keyframes fadeIn {
      from { opacity: 0; transform: translateY(10px); }
      to { opacity: 1; transform: translateY(0); }
    }
  </style>
</head>
<body>
  <div class="container">
    <h2>ESP32 Dashboard</h2>

    <div class="sensor">
      <b>Temperature:</b> <span id="temp">)rawliteral" + String(temperature) + R"rawliteral(</span> &deg;C<br>
      <b>Humidity:</b> <span id="hum">)rawliteral" + String(humidity) + R"rawliteral(</span> %
    </div>

    <div class="buttons">
      <button id="led1" class=")rawliteral" + String(led1_state ? "led-on" : "led-off") + R"rawliteral(" onclick="toggleLED(1)">
        LED1: <span id="l1">)rawliteral" + led1 + R"rawliteral(</span>
      </button>
      <button id="led2" class=")rawliteral" + String(led2_state ? "led-on" : "led-off") + R"rawliteral(" onclick="toggleLED(2)">
        LED2: <span id="l2">)rawliteral" + led2 + R"rawliteral(</span>
      </button>
    </div>

    <button id="settings" onclick="window.location='/settings'">&#9881; Settings</button>
  </div>

  <script>
    function toggleLED(id) {
      fetch('/toggle?led=' + id)
        .then(response => response.json())
        .then(json => {
          document.getElementById('l1').innerText = json.led1;
          document.getElementById('l2').innerText = json.led2;
          updateLEDStyle('led1', json.led1);
          updateLEDStyle('led2', json.led2);
        });
    }

    function updateLEDStyle(btnId, state) {
      const btn = document.getElementById(btnId);
      if (state === 'ON') {
        btn.classList.add('led-on');
        btn.classList.remove('led-off');
      } else {
        btn.classList.add('led-off');
        btn.classList.remove('led-on');
      }
    }

    setInterval(() => {
      fetch('/sensors')
        .then(res => res.json())
        .then(d => {
          document.getElementById('temp').innerText = d.temp;
          document.getElementById('hum').innerText = d.hum;
        });
    }, 3000);
  </script>
</body>
</html>
  )rawliteral";
}


// String settingsPage() {
//   return SETTINGS_PAGE;
// }

// ========== Handlers ==========
void handleRoot() { server.send(200, "text/html", mainPage()); }

void handleToggle() {
  int led = server.arg("led").toInt();
  if (led == 1) {
    led1_state = !led1_state;
    Serial.println("YOUR CODE TO CONTROL LED1");
  }
  else if (led == 2){
    led2_state = !led2_state;
    Serial.println("YOUR CODE TO CONTROL LED2");
  }
  server.send(200, "application/json",
    "{\"led1\":\"" + String(led1_state ? "ON":"OFF") +
    "\",\"led2\":\"" + String(led2_state ? "ON":"OFF") + "\"}");
}

void handleSensors() {
  float t = -1;
  if(xSemaphoreTake(mutexForTempData, portMAX_DELAY) == pdTRUE) {
    t = glob_temperature;
    xSemaphoreGive(mutexForTempData);
  } else {
    Serial.println(ERR_TEMP_TIMEOUT);
  }

  float h = -1;
  if(xSemaphoreTake(mutexForHumiData, portMAX_DELAY) == pdTRUE) {
    h = glob_humidity;
    xSemaphoreGive(mutexForHumiData);
  } else {
    Serial.println(ERR_HUMI_TIMEOUT);
  }

  String json = "{\"temp\":"+String(t)+",\"hum\":"+String(h)+"}";
  server.send(200, "application/json", json);
}

void handleSettings() { server.send(200, "text/html", SETTINGS_PAGE); }

void handleConnect() {
  wifi_ssid = server.arg("ssid");
  wifi_password = server.arg("pass");
  server.send(200, "text/plain", "Connecting....");
  isAPMode = false;
  connecting = true;
  connect_start_ms = millis();
  connectToWiFi();
}

// ========== WiFi ==========
void startAP() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid.c_str(), password.c_str());
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());
  isAPMode = true;
  connecting = false;
}

void setupServer() {
  server.on("/", HTTP_GET, handleRoot);
  server.on("/toggle", HTTP_GET, handleToggle);
  server.on("/sensors", HTTP_GET, handleSensors);
  server.on("/settings", HTTP_GET, handleSettings);
  server.on("/connect", HTTP_POST, handleConnect); 
  server.begin();
}

void connectToWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(wifi_ssid.c_str(), wifi_password.c_str());
  Serial.print("Connecting to: '");
  Serial.print(wifi_ssid.c_str());

  Serial.print("' Password: '");
  Serial.print(wifi_password.c_str());
  Serial.println("'");
}

// ========== Main task ==========
void main_server_task(void *pvParameters){
  pinMode(BOOT_PIN, INPUT_PULLUP);

  startAP();
  setupServer();

  while(1){
    server.handleClient();

    // BOOT Button to switch to AP Mode
    if (digitalRead(BOOT_PIN) == LOW) {
      vTaskDelay(100);
      if (digitalRead(BOOT_PIN) == LOW) {
        if (!isAPMode) {
          startAP();
          setupServer();
        }
      }
    }

    // STA Mode
    if (connecting) {
      if (WiFi.status() == WL_CONNECTED) {
        Serial.print("STA IP address: ");
        Serial.println(WiFi.localIP());
        isWifiConnected = true; //Internet access

        xSemaphoreGive(xBinarySemaphoreInternet);

        isAPMode = false;
        connecting = false;
      } else if (millis() - connect_start_ms > 10000) { // timeout 10s
        Serial.println("WiFi connect failed! Back to AP.");
        startAP();
        setupServer();
        connecting = false;
        isWifiConnected = false;
      }
    }

    vTaskDelay(20); // avoid watchdog reset
  }
}