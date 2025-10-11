#include "global.h"

float glob_temperature = 0;
float glob_humidity = 0;

String ssid = "ESP32-LIAM";
String password = "12345678";
String wifi_ssid = "Bon Bon";
String wifi_password = "03102017";
boolean isWifiConnected = false;
SemaphoreHandle_t xBinarySemaphoreInternet = xSemaphoreCreateBinary();
SemaphoreHandle_t mutexForTempData = xSemaphoreCreateMutex();
SemaphoreHandle_t mutexForHumiData = xSemaphoreCreateMutex();
const char* ERR_TEMP_TIMEOUT  = "ERROR: Timeout waiting for Temperature resource!";
const char* ERR_HUMI_TIMEOUT  = "ERROR: Timeout waiting for Humidity resource!";
const char* ERR_LIGHT_TIMEOUT = "ERROR: Timeout waiting for Light resource!";

const char MAIN_PAGE[] PROGMEM = R"rawliteral(

)rawliteral";

const char SETTINGS_PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1.0" />
  <title>Wi-Fi Settings</title>
  <style>
    :root {
      --primary: #00bad1;
      --light: #d6f4f8;     
      --dark: #d6f4f8;
      --bg: #f6f8fa;
      --text: #333;
      --radius: 8px;
      --shadow: 0 2px 5px rgba(214, 244, 248, 1);
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
      max-width: 450px;
      background: #fff;
      border-radius: var(--radius);
      box-shadow: var(--shadow);
      margin-top: 50px;
      padding: 16px;
      box-sizing: border-box;
      animation: fadeIn 0.5s ease;
    }

    h2 {
      text-align: center;
      color: var(--primary);
      margin-bottom: 25px;
    }

    .form-group {
      width: 100%;
      margin-bottom: 20px;
    }

    label {
      display: block;
      font-size: 0.9rem;
      color: var(--text);
      margin-bottom: 5px;
      font-weight: 500;
    }

    input {
  	  box-sizing: border-box;
      width: 100%;
      padding: 10px 12px;
      border: 1px solid #ccc;
      border-radius: var(--radius);
      font-size: 1rem;
      transition: all 0.3s ease;
    }

    input:focus {
      border-color: var(--primary);
      box-shadow: 0 0 4px var(--primary);
      outline: none;
    }
    
    button {
      padding: 10px 18px;
      font-size: 16px;
      border: none;
      border-radius: 8px;
      cursor: pointer;
      transition: all 0.25s ease;
    }
    
    #connectBtn {
      background-color: var(--light);
      color: var(--primary);
      width: 100%;
    }

    #connectBtn:hover {
      box-shadow: var(--shadow);
      background: var(--dark);
    }

    #backBtn {
      background-color: #e0e0e0;
      color: #333;
      margin-top: 10px;
      width: 100%;
    }

    #backBtn:hover {
      background-color: #ccc;
    }

    .status {
      margin-top: 15px;
      text-align: center;
      font-size: 0.9rem;
      color: #777;
    }

    @keyframes fadeIn {
      from { opacity: 0; transform: translateY(10px); }
      to { opacity: 1; transform: translateY(0); }
    }

    @media (max-width: 480px) {
      .container { padding: 20px; margin-top: 30px; }
    }
  </style>
</head>
<body>
  <div class="container">
    <h2>Wi-Fi Settings</h2>
    <form id="wifiForm">
      <div class="form-group">
        <label for="ssid">Wi-Fi SSID</label>
        <input type="text" id="ssid" name="ssid" placeholder="Enter Wi-Fi name" required>
      </div>

      <div class="form-group">
        <label for="password">Wi-Fi Password</label>
        <input type="password" id="password" name="password" placeholder="Enter password" required>
      </div>

      <button id="connectBtn" type="submit">Connect</button>
      <button id="backBtn" type="button" onclick="window.location='/'">Back</button>
      <p class="status" id="statusText">Waiting for input...</p>
    </form>
  </div>

  <script>
    const form = document.getElementById("wifiForm");
    const statusText = document.getElementById("statusText");

    form.addEventListener("submit", async (e) => {
      e.preventDefault();
      const ssid = document.getElementById("ssid").value;
      const password = document.getElementById("password").value;
      statusText.textContent = "Connecting...";

      try {
        const res = await fetch(`/connect?ssid=${encodeURIComponent(ssid)}&password=${encodeURIComponent(password)}`);
        if (res.ok) {
          statusText.textContent = "Connected successfully!";
          statusText.style.color = "green";
        } else {
          statusText.textContent = "Connection failed.";
          statusText.style.color = "red";
        }
      } catch (err) {
        statusText.textContent = "Error connecting.";
        statusText.style.color = "red";
      }
    });
  </script>
</body>
</html>

)rawliteral";