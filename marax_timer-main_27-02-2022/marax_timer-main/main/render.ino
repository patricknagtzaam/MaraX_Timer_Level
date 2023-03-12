//RENDER DISPLAY

void setupDisplay() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.drawBitmap(0, 0, lelitNameBitmap, 128, 64, WHITE);
  display.display();
  delay(5000);
}


// start rendering the display
void render() {
  display.clearDisplay();
  if (machineState !="off") {
    if (timerStarted) {
      renderTimer();
    } else {
      renderDashboard();
    }
  }
  display.display();
}


// draw timer
void renderTimer() {
  if (machineState != "off"){

    //Display comments
    if (timerCount < 12){
      display.setTextSize(1);
      display.setCursor(33, 0);
      display.print("pre-infusion");
    }
    if (timerCount >= 11 && timerCount < 24) {
      display.setTextSize(1);
      display.setCursor(49, 0);
      display.print("brewing");
    }
    if (timerCount >= 24 && timerCount < 36){
      display.setTextSize(1);
      display.setCursor(43, 0);
      display.print("get ready");
    }
     if (timerCount >= 36 && timerCount < 38){
      display.setTextSize(1);
      display.setCursor(39, 0);
      display.print("you missed");
    }
     if (timerCount >= 38){
      display.setTextSize(1);
      display.setCursor(15, 0);
      display.print("going for a lungo");
    }
    

    //Display counter
    display.setTextSize(5);
    display.setCursor(40, 15);
    display.print(getTimer());


    //Display coffee cup
    switch(timerCount){
      case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9: case 10: 
        display.drawBitmap(0, 20, myBitmap0, 28, 26, WHITE);
        break;
      case 11: case 15: case 19: case 23: case 27: case 31: case 35: case 39: case 43:
        display.drawBitmap(0, 20, myBitmap1, 28, 26, WHITE);
        break;
      case 12: case 16: case 20: case 24: case 28: case 32: case 36: case 40: case 44:
        display.drawBitmap(0, 20, myBitmap2, 28, 26, WHITE);
        break;
      case 13: case 17: case 21: case 25: case 29: case 33: case 37: case 41: case 45:
        display.drawBitmap(0, 20, myBitmap3, 28, 26, WHITE);
        break;
      case 14: case 18: case 22: case 26: case 30: case 34: case 38: case 42: case 46:
        display.drawBitmap(0, 20, myBitmap4, 28, 26, WHITE);
        break;
      default:
        display.drawBitmap(0, 20, myBitmap4, 28, 26, WHITE);
        break;
    }

    //Display Temperature at the bottom
    if (hxTemperature) {
      String water = String(hxTemperature);
      if (water.length()==2){
        display.drawCircle(75, 56, 2, SSD1306_WHITE);
      }else{
        display.drawCircle(79, 56, 2, SSD1306_WHITE);
      }
      display.setTextSize(1);
      display.setCursor(60, 57);
      display.print(water);
    }
  }
}

// draw dashboard
void renderDashboard() {
  
  //draw water level
  getWaterLevel(); 
  
  switch(waterLevel){
    case 0: case 1: case 2: case 3: case 4:
      display.drawLine(64,  1, 64, 63, SSD1306_WHITE);
      break;
    case 5: case 6:
      display.drawLine(64,  8, 64, 63, SSD1306_WHITE);
      break;
    case 7: case 8:
      display.drawLine(64, 16, 64, 63, SSD1306_WHITE);
      break;
    case 9: case 10:
      display.drawLine(64, 24, 64, 63, SSD1306_WHITE);
      break;
    case 11: case 12:
      display.drawLine(64, 32, 64, 63, SSD1306_WHITE);
      break;
    case 13: case 14:
      display.drawLine(64, 40, 64, 63, SSD1306_WHITE);
      break;
    case 15: case 16:
      display.drawLine(64, 48, 64, 63, SSD1306_WHITE);
      break;
    case 17: case 18:
      display.drawLine(64, 56, 64, 63, SSD1306_WHITE);
      break;
    case 19: case 20: case 21: case 22: case 23: case 24:
      display.drawLine(64, 63, 64, 63, SSD1306_WHITE);
      break;
    default:
      display.drawLine(64,  1, 64, 63, SSD1306_WHITE);
      break;            
  }  

  // draw dots line
  display.drawLine(64,  0, 64,  0, SSD1306_WHITE);
  display.drawLine(64,  8, 64,  8, SSD1306_WHITE);
  display.drawLine(64, 16, 64, 16, SSD1306_WHITE);
  display.drawLine(64, 24, 64, 24, SSD1306_WHITE);
  display.drawLine(64, 32, 64, 32, SSD1306_WHITE);
  display.drawLine(64, 40, 64, 40, SSD1306_WHITE);
  display.drawLine(64, 48, 64, 48, SSD1306_WHITE);
  display.drawLine(64, 56, 64, 56, SSD1306_WHITE);    


  // draw last shot
  display.setTextSize(1);
  display.setCursor(85, 5);
  display.print(String(prevTimerCount) + " sec");
 
  // draw heating indicator    
  if (!machineHeatingBoost) {
    if (machineHeating) {
       // draw fill circle if heating on 
       display.fillCircle(6, 7, 6, SSD1306_WHITE);
       if (heatingBoost > 0) {
        display.setTextSize(1);
        display.setTextColor(BLACK);
        display.setCursor(4, 4);
        display.print(machineState);
        display.setTextColor(WHITE);
        display.setCursor(18, 4);
        display.print(String(heatingBoost));
       }
     } else {
       // draw empty circle if heating off
       display.drawCircle(6, 7, 6, SSD1306_WHITE);       
       if (heatingBoost > 0) {
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.setCursor(4, 4);
        display.print(machineState);
        display.setCursor(18, 4);
        display.print(String(heatingBoost));
       }
     }
  } else {
    if (machineHeating) {
      // draw fill rectangle if heating on
      display.fillRect(1, 1, 12, 12, SSD1306_WHITE);
      display.setTextSize(1);
      display.setTextColor(BLACK);
      display.setCursor(4, 4);
      display.print(machineState);
      display.setTextColor(WHITE);
      display.setCursor(18, 4);
      display.print("Boost");
    } else {
      // draw empty rectangle if heating off
      display.drawRect(1, 1, 12, 12, SSD1306_WHITE);
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(4, 4);
      display.print(machineState);
      display.setCursor(18, 4);
      if (hxTemperature >= 90){
        display.print("Ready");
      }else{
        display.print("Boost");
      }
    }
  }
  
  // draw steam temperature
  if (steamTemperature) {
    String steam = String(steamTemperature);
    display.drawBitmap(17, 17, steamBitmap, 25, 25, WHITE);
    // how many digits is the steam temperature?
    if (steam.length()==2){
      display.drawCircle(46, 40, 3, SSD1306_WHITE);
      display.setTextSize(2);
      display.setCursor(20, 46);
      display.print(steam);
    }else{
      display.drawCircle(50, 40, 3, SSD1306_WHITE);
      display.setTextSize(2);
      display.setCursor(12, 46);
      display.print(steam);
    }
  }

  // draw water temperature
  if (hxTemperature) {
    String water = String(hxTemperature);
    display.drawBitmap(92, 19, dropBitmap, 21, 21, WHITE);
    // how many digits is the water temperature?
    if (water.length()==2){
      display.drawCircle(118, 40, 3, SSD1306_WHITE);
      display.setTextSize(2);
      display.setCursor(93, 46);
      display.print(water);
    }else{
      display.drawCircle(122, 40, 3, SSD1306_WHITE);
      display.setTextSize(2);
      display.setCursor(83, 46);
      display.print(water);
    }
  }
}
