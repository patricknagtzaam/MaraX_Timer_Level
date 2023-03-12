//MQTT STUFF

String lastMachinePower;
String lastCoffeeVapour;
String lastSoftwareVersion;
int lastHxTemperature = NULL;
int lastSteamTemperature = NULL;
int lastTargetSteamTemperature = NULL;
int lastHeatingBoost = NULL;
long lastTimerStartMillis = 0;
bool lastTimerStarted = false;
int delayBroadcast = 0;

//wait until wifi is connected
void setupWifi() { 
  WiFi.begin(SSID, PSK);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
  }
  Serial.println("wifi connected");
  client.setServer(MQTT_BROKER, 1883);
}

//start broadcasting to mqtt
void updateWifi() {
  if (!client.connected()) {
    while (!client.connected()) {
      client.connect("marax");      
      delay(100);
    }
  }
  
  client.loop();


  if (lastHxTemperature != hxTemperature) {
    lastHxTemperature = hxTemperature;
    broadcastHxTemperature();
  }

  if (lastSteamTemperature != steamTemperature) {
    lastSteamTemperature = steamTemperature;
    broadcastSteamTemperature();
  }

  if (lastHeatingBoost != heatingBoost) {
    lastHeatingBoost = heatingBoost;
    broadcastHeatingBoost();
  }

  if (lastTargetSteamTemperature != targetSteamTemperature) {
    lastTargetSteamTemperature = targetSteamTemperature;
    broadcastTargetSteamTemperature();
  }

  if (lastTimerStarted != timerStarted) {
    lastTimerStarted = timerStarted;
    broadcastPump();
  }
}


void broadcastHxTemperature() {
  client.publish("lelit/marax/water", String(hxTemperature).c_str());
}


void broadcastTargetSteamTemperature() {
  client.publish("lelit/marax/target", String(targetSteamTemperature).c_str());
}


void broadcastSteamTemperature() {
  client.publish("lelit/marax/steam", String(steamTemperature).c_str());
}


void broadcastHeatingBoost() {
  client.publish("lelit/marax/boost", String(heatingBoost).c_str());
}


void broadcastShot() {
  client.publish("lelit/marax/shot",String(timerStarted).c_str());
}


void broadcastPump() {
  //timer starts when pump is on
  if (timerStarted) {
    client.publish("lelit/marax/pump", "on");
  } else {
    client.publish("lelit/marax/pump", "off");
  }
}
