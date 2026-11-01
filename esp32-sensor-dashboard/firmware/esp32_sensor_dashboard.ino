/*
 * ESP32 Sensor Dashboard - Open-source example
 * MIT License
 * No credentials needed: the ESP32 creates its own Wi-Fi network.
 * Connect to "ESP32-Sensors" and open http://192.168.4.1
 */
#include <WiFi.h>
#include <WebServer.h>
#include <DHT.h>

#define DHT_PIN 4
#define DHT_TYPE DHT11
#define LDR_PIN 34

const char* AP_SSID = "ESP32-Sensors";
const char* AP_PASSWORD = "demo12345"; // Local demonstration password; change before real use.
WebServer server(80);
DHT dht(DHT_PIN, DHT_TYPE);

// Serve the dashboard directly from flash; no internet or cloud needed.
const char DASHBOARD[] PROGMEM = R"HTML(<!doctype html>
<html lang="fr"><head><meta charset="utf-8"><meta name="viewport" content="width=device-width,initial-scale=1">
<title>ESP32 Sensor Dashboard</title><style>
body{font-family:system-ui,sans-serif;background:#0b1220;color:#edf2fa;margin:0;padding:24px}
main{max-width:780px;margin:auto}h1{font-size:clamp(24px,5vw,36px)}p{color:#adbdd5}
.grid{display:grid;grid-template-columns:repeat(auto-fit,minmax(170px,1fr));gap:16px;margin-top:24px}
.card{background:#17243b;border:1px solid #2d4565;border-radius:16px;padding:22px}
.label{font-size:14px;color:#adc2df}.value{font-size:35px;font-weight:750;margin-top:12px}
footer{margin-top:24px;color:#adc2df;font-size:13px}.error{color:#ffb1b1}
</style></head><body><main><h1>ESP32 Sensor Dashboard</h1>
<p>Mesures locales, mises à jour toutes les 3 secondes.</p>
<div class="grid"><section class="card"><div class="label">Température</div><div class="value" id="temp">—</div></section>
<section class="card"><div class="label">Humidité</div><div class="value" id="hum">—</div></section>
<section class="card"><div class="label">Luminosité (valeur brute)</div><div class="value" id="light">—</div></section></div>
<footer id="status" role="status">Connexion au capteur…</footer></main>
<script>
async function update(){try{const r=await fetch('/api/sensors',{cache:'no-store'});if(!r.ok)throw Error('HTTP '+r.status);
const d=await r.json();document.getElementById('temp').textContent=d.temperature===null?'—':d.temperature.toFixed(1)+' °C';
document.getElementById('hum').textContent=d.humidity===null?'—':d.humidity.toFixed(1)+' %';
document.getElementById('light').textContent=d.light_raw;
document.getElementById('status').textContent='Dernière mise à jour : '+new Date().toLocaleTimeString();
}catch(e){document.getElementById('status').textContent='Connexion impossible : '+e.message;
document.getElementById('status').className='error';}}
update();setInterval(update,3000);
</script></body></html>)HTML";

void sendSensorData() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  int lightRaw = analogRead(LDR_PIN); // 0..4095, NOT calibrated lux.
  String json = "{\"temperature\":";
  json += isnan(temperature) ? "null" : String(temperature, 1);
  json += ",\"humidity\":";
  json += isnan(humidity) ? "null" : String(humidity, 1);
  json += ",\"light_raw\":" + String(lightRaw) + "}";
  server.sendHeader("Cache-Control", "no-store");
  server.send(200, "application/json; charset=utf-8", json);
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  analogReadResolution(12);
  WiFi.mode(WIFI_AP);
  WiFi.softAP(AP_SSID, AP_PASSWORD);
  Serial.print("Dashboard: http://"); Serial.println(WiFi.softAPIP());
  server.on("/", HTTP_GET, [](){ server.send_P(200, "text/html; charset=utf-8", DASHBOARD); });
  server.on("/api/sensors", HTTP_GET, sendSensorData);
  server.onNotFound([](){ server.send(404, "text/plain", "Not found"); });
  server.begin();
}
void loop() { server.handleClient(); }
