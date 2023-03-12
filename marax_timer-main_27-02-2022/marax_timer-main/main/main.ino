#define D3 (0)
#define D4 (2)
#define D5 (14)
#define D6 (12)
#define D7 (13)
#define ANALOG_IN_PIN A0//33K resistor to

#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Event.h>
#include <Wire.h>
#include <Timer.h>
#include <DistanceSensor_A02YYUW.h>
#include <SoftwareSerial.h>

Adafruit_SSD1306 display(128, 64, &Wire, -1);
SoftwareSerial mySerial1(D5, D6);
SoftwareSerial mySerial2(D3, D4);
DistanceSensor_A02YYUW distanceSensor(&mySerial2);
Timer t;
WiFiClient espClient;
PubSubClient client(espClient);

// Config
const char* SSID = "Linksys9091";
const char* PSK = "2010QuaTTro";
const char* MQTT_BROKER = "192.168.178.199";

// State
int timerCount = 0;
int prevTimerCount = 0;
bool timerStarted = false;
long timerStartMillis = 0;
long timerStopMillis = 0;
long serialUpdateMillis = 0;
String machineState;
String timerState;
bool machineHeating;
bool machineHeatingBoost;
int hxTemperature = NULL;
int hxTemperatureOld = NULL;
int targetSteamTemperature = NULL;
int steamTemperature = NULL;
int steamTemperatureOld = NULL;
int heatingBoost = NULL;
int heatingBoostOld = NULL;
const byte numChars = 32;
char receivedChars[numChars];

float adc_voltage = 0.0;
float in_voltage = 0.0;
float R1 = 30000.0;
float R2 = 7500.0;
float ref_voltage = 5.0;
int adc_value = 0;
int data = 100;
int waterLevel = 0;
int waterLevelOld = 0;
int meassurementStatus;

// start the initial program
void setup() {
  mySerial2.begin(9600);
  Serial.begin(9600);
  if (SSID != "" && PSK != "" && MQTT_BROKER != "") {
    Serial.println("setup wifi");
    setupWifi();
  } else {
    WiFi.mode(WIFI_OFF);
  }
  initSerial();
  setupSerial();  // starts receiving serial data
  setupDisplay(); // setup display
  t.every(100, render);
  broadcastShot();
}

// start looping
void loop() {
  t.update();

  updateTimer();
  updateSerial();
  
  if (SSID != "" && PSK != "" && MQTT_BROKER != "") {
    updateWifi();
  }
}
