//GET SERIAL DATA

static byte ndx = 0;
char endMarker = '\n';
char rc;

// sets the LED display on
void initSerial() {
  digitalWrite(LED_BUILTIN, HIGH);
}

void setupSerial() {
  mySerial1.begin(9600); // Lelit Gicar
  mySerial2.begin(9600); // Water Sensor
  pinMode(LED_BUILTIN, OUTPUT);
  memset(receivedChars, 0, numChars);
  mySerial1.write(0x11);
}

//get the serial string
//example: C123b,117,112,079,0780,1
// C = coffee of V = vapour (1/0 switch on right of the machine)
// 123b = softwareVersion
// 117  = steamTemperature
// 112  = targetSteamTemperatur
// 079  = hxTemperature
// 0780 = heatingBooster
// 1    = heatingElement


void updateSerial() {
  mySerial1.listen();
  while (mySerial1.available() ) {
    serialUpdateMillis = millis();
    rc = mySerial1.read();

    //the serial string is receievd charater by charater
    //if end marker (new line) is found the serial is complete
    if (rc != endMarker) {
      receivedChars[ndx] = rc;    
      ndx++;
      if (ndx >= numChars) {
        ndx = numChars - 1;
      }
    } else {
      receivedChars[ndx] = '\0';
      ndx = 0;
      
      //check if the received characters are 25 long to avoid strange charaters
      if(String(receivedChars).length() == 25 
       && receivedChars[5] == ',' //check if the comma separators are there
       && receivedChars[9] == ','
       && receivedChars[13] == ','
       && receivedChars[17] == ','
       && receivedChars[22] == ','
       && String(receivedChars).indexOf('⸮') == -1   ){
        machineState = getMachineState();
        machineHeating = getMachineHeating();
        machineHeatingBoost = getMachineHeatingBoost();
        heatingBoost = getHeatingBoost();
        hxTemperature = getTemperatureHx();
        steamTemperature = getTemperatureSteam();
        targetSteamTemperature = getTargetTemperatureSteam();
        Serial.println(receivedChars);
      }
    }
  }
  
  //millis = the number of milliseconds passed since the Arduino board began running the current program
  //This number will overflow (go back to zero), after approximately 50 days.
  if (millis() - serialUpdateMillis > 5000) {
    serialUpdateMillis = millis();
    memset(receivedChars, 0, numChars);
    Serial.println("Request serial update");
    mySerial1.write(0x11);
    machineState = "off"; // also used to indicate machine is off 
    machineHeating = NULL;
    machineHeatingBoost = NULL;
    hxTemperature = NULL;
    steamTemperature = NULL;
    targetSteamTemperature = NULL;
    if (timerStarted != true){
      setupDisplay();   
    }
  }
}


//get the level from sensor
void getWaterLevel() {
  waterLevelOld = waterLevel;
  mySerial2.listen();
  if ((mySerial2.available() > 0)) {
    DistanceSensor_A02YYUW_MEASSUREMENT_STATUS meassurementStatus;
    meassurementStatus = distanceSensor.meassure();
    if (meassurementStatus == DistanceSensor_A02YYUW_MEASSUREMENT_STATUS_OK) {
      waterLevel = distanceSensor.getDistance();
      waterLevel = (int)waterLevel/10;
      if (waterLevel == 0){
        waterLevel = waterLevelOld;
      }
    } else {
      waterLevel = waterLevelOld;
    }
  }
}


//get substrings from received characters
String getMachineState() {
    return String(receivedChars[0]);
}


int getTemperatureSteam() {
  if (receivedChars[6] && receivedChars[7] && receivedChars[8]) {
    //Steam is stable no drops to 0
    return String(receivedChars).substring(6, 9).toInt();
  }
  return NULL;
}


int getTargetTemperatureSteam() {
  if (receivedChars[6] && receivedChars[7] && receivedChars[8]) {
    return String(receivedChars).substring(10, 13).toInt();
  }
  return NULL;
}


int getTemperatureHx() {
  if (receivedChars[14] && receivedChars[15] && receivedChars[16]) {
    return String(receivedChars).substring(14, 17).toInt();
  }
  return NULL;
}


bool getMachineHeatingBoost() {
  return String(receivedChars).substring(18, 22) == "0000";
}


int getHeatingBoost() {
  return String(receivedChars).substring(18, 22).toInt();
}


bool getMachineHeating() {
  return String(receivedChars[23]) == "1";
}
