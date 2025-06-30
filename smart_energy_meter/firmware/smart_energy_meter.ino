
/**
 * Smart Energy Meter firmware
 * ESP32 + JSY‑MK‑194G energy module
 * Publishes telemetry over MQTT and displays live data + appliance label.
 *
 * Dependencies (install via Arduino Library Manager):
 *   - ModbusMaster         by Doc Walker
 *   - PubSubClient         by Nick O'Leary
 *   - ArduinoJson          by Benoit Blanchon
 *   - Adafruit SSD1306     by Adafruit (plus Adafruit GFX)
 */

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ModbusMaster.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <ArduinoJson.h>
#include "config.h"

// ----- Display -----
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ----- UART for Modbus -----
#define MODBUS_RX_PIN 16
#define MODBUS_TX_PIN 17
HardwareSerial modbusSerial(2);
ModbusMaster node;

// JSY‑MK‑194G registers (holding)
const uint16_t REG_VOLTAGE  = 0x0000; // 0.1 V
const uint16_t REG_CURRENT  = 0x0001; // 0.001 A
const uint16_t REG_POWER    = 0x0002; // 0.1 W
const uint16_t REG_PF       = 0x0003; // 0.001

// ----- MQTT -----
WiFiClient espClient;
PubSubClient mqtt(espClient);

// State
String appliance = "—";
unsigned long lastRead = 0;
const uint32_t READ_PERIOD_MS = 2000;

void connectWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  uint8_t attempts = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    if (++attempts > 50) ESP.restart();
  }
}

void mqttCallback(char* topic, byte* payload, unsigned int len) {
  String msg;
  for (unsigned int i = 0; i < len; i++) msg += (char)payload[i];
  if (String(topic) == TOPIC_CLASSIFICATION) appliance = msg;
}

void connectMQTT() {
  while (!mqtt.connected()) {
    if (mqtt.connect(DEVICE_ID, MQTT_USER, MQTT_PASSWORD)) {
      mqtt.subscribe(TOPIC_CLASSIFICATION);
    } else {
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  Wire.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Smart Energy Meter");
  display.display();

  // Modbus
  modbusSerial.begin(9600, SERIAL_8N1, MODBUS_RX_PIN, MODBUS_TX_PIN);
  node.begin(1, modbusSerial);

  connectWiFi();
  mqtt.setServer(MQTT_BROKER, MQTT_PORT);
  mqtt.setCallback(mqttCallback);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) connectWiFi();
  if (!mqtt.connected()) connectMQTT();
  mqtt.loop();

  if (millis() - lastRead > READ_PERIOD_MS) {
    // Read registers
    node.readHoldingRegisters(REG_VOLTAGE, 4);
    float voltage = node.getResponseBuffer(0) / 10.0;
    float current = node.getResponseBuffer(1) / 1000.0;
    float power   = node.getResponseBuffer(2) / 10.0;
    float pf      = node.getResponseBuffer(3) / 1000.0;

    // Publish JSON
    StaticJsonDocument<128> doc;
    doc["voltage"] = voltage;
    doc["current"] = current;
    doc["power"]   = power;
    doc["pf"]      = pf;
    char buff[128];
    size_t n = serializeJson(doc, buff);
    mqtt.publish(TOPIC_PUBLISH, buff, n);

    // OLED
    display.clearDisplay();
    display.setCursor(0,0);
    display.printf("V: %.1fV\n", voltage);
    display.printf("I: %.3fA\n", current);
    display.printf("P: %.1fW\n", power);
    display.printf("PF: %.2f\n", pf);
    display.println("----------------");
    display.println(appliance.c_str());
    display.display();

    lastRead = millis();
  }
}
