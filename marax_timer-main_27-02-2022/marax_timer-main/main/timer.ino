//SHOT TIMER

//start timer
void updateTimer() {
  //get voltage read out
  adc_value = analogRead(ANALOG_IN_PIN);
  adc_voltage  = (adc_value * ref_voltage) / 1024.0;
  in_voltage = adc_voltage / (R2/(R1+R2)) ;

  //Serial.println(in_voltage);
  
  if (!timerStarted && in_voltage < 25 && in_voltage > 20) {
    timerStartMillis = millis();
    timerStarted = true;
    broadcastShot();
    Serial.println("Start pump");
    broadcastPump();
  }
  if (timerStarted  && in_voltage == 25) {
    if (timerStopMillis == 0) {
      timerStopMillis = millis();
    }
    if (millis() - timerStopMillis > 500) {
      timerStarted = false;
      timerStopMillis = 0;
      broadcastShot();
      Serial.println("Stop pump");
      broadcastPump();
    }
  } else {
    timerStopMillis = 0;
  }
}

//get timer value
  String getTimer() {
  char outMin[2];
  if (timerStarted) {
    timerCount = (millis() - timerStartMillis ) / 1000;
    //after pre-infushion start remembering the count
    if (timerCount > 10) {
      prevTimerCount = timerCount;
    }
  } else {
    timerCount = prevTimerCount;
  }
  if (timerCount > 99) {
    return "99";
  }
  sprintf( outMin, "%02u", timerCount);
  return outMin;
}
