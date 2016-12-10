long timeInterval = 500;

void hours() {
  timeWrite('h', timeInterval);
  if(isPressed(0)) {
    resetButton(0);
    hours();
  }
  else if(isPressed(1)) {
    resetButton(1);
    minutes();
  }
}

void minutes() {
  timeWrite('m', timeInterval);
  if(isPressed(0)) {
    resetButton(0);
    hours();
  }
  else if(isPressed(1)) {
    resetButton(1);
    seconds();
  }
}

void seconds() {
  timeWrite('s', timeInterval);
  if(isPressed(0)) {
    resetButton(0);
    minutes();
  }
  else if(isPressed(1)) {
    resetButton(1);
    ampm();
  }
}

void ampm() {
  timeWrite('a', timeInterval);
  if(isPressed(0)) {
    resetButton(0);
    seconds();
  }
  else if(isPressed(1)) {
    resetButton(1);
    clockTime = millis();
    timeClock();
  }
}

void timePrompt() {
  OrbitOledClear();
  OrbitOledMoveTo(0, 0);
  OrbitOledDrawString("Enter the time:");
  OrbitOledMoveTo(0, LINEHEIGHT);
  OrbitOledDrawString("HH:MM:SS");
  OrbitOledUpdate();

  hours();
}

void timeWrite(char type, long timeInterval) {
  long current = millis();
  long previous = millis();
  bool noPress = true;
  while(noPress) {
    while ((current - previous < timeInterval) && noPress) {
      OrbitOledMoveTo(0, 2 * LINEHEIGHT);
      updateInput();
      if(isPressed(0) || isPressed(1)) {
        noPress = false;
      }
      int divisor;
      switch(type) {
        case 'h': divisor = 4096 / 12; break;
        case 'm': divisor = 4096 / 60; break;
        case 's': divisor = 4096 / 60; break;
        case 'a': divisor = 4096 / 3; break;
      }
      int c = (int)((analogRead(Potentiometer) - divisor / 2) / divisor);
      if(type == 'h') {
        c++;
      }
      switch(type) {
        case 'h': times.hours = c; break;
        case 'm': times.minutes = c; break;
        case 's': times.seconds = c; break;
        case 'a': times.ampm = c; break;
      }

      if(times.hours < 10) {
        OrbitOledDrawChar('0');
      }
      printNum(times.hours);
      OrbitOledDrawChar(':');
      if(times.minutes < 10) {
        OrbitOledDrawChar('0');
      }
      printNum(times.minutes);
      OrbitOledDrawChar(':');
      if(times.seconds < 10) {
        OrbitOledDrawChar('0');
      }
      printNum(times.seconds);
      OrbitOledDrawChar(' ');
      if(times.ampm) {
        OrbitOledDrawString("PM");
      }
      else {
        OrbitOledDrawString("AM");
      }

      OrbitOledUpdate();
      current = millis();
    }
    previous = millis();

    while ((current - previous < timeInterval) && noPress) {
      updateInput();
      if(isPressed(0) || isPressed(1)) {
        noPress = false;
      }
      OrbitOledMoveTo(0, 2 * LINEHEIGHT);
      OrbitOledDrawString("  :  :     ");
      OrbitOledUpdate();
      current = millis();
    }
    previous = millis();
  }
}

void timeClock() {
  OrbitOledClear();
  OrbitOledMoveTo(0, 0);
  OrbitOledDrawString("The current");
  OrbitOledMoveTo(0, LINEHEIGHT);
  OrbitOledDrawString("time is:");
  long current = millis();
  long previous = millis();
  long timeInterval = 500;
  bool noPress = true;

  timeDisplay currentTime = {times.hours, times.minutes, times.seconds, times.ampm};
  while(noPress) {
    
    currentTime.hours = ((times.hours + (millis() - clockTime + 1000 * times.seconds + 1000 * 60 * times.minutes) / 3600000) - 1) % 12 + 1;
    currentTime.minutes = (times.minutes + (millis() - clockTime + 1000 * times.seconds) / 60000) % 60;
    currentTime.seconds = (times.seconds + (millis() - clockTime) / 1000) % 60;
    currentTime.ampm = ((millis() - clockTime + 1000 * times.seconds + 1000 * 60 * times.minutes + 1000 * 60 * 60 * (times.hours) + 1000 * 60 * 60 * 12 * times.ampm) / (1000 * 60 * 60 * 12) ) % 2;
    
    while ((current - previous < timeInterval) && noPress) {
      OrbitOledMoveTo(0, 0);
      OrbitOledDrawString("The current");
      OrbitOledMoveTo(0, LINEHEIGHT);
      OrbitOledDrawString("time is:");
      OrbitOledMoveTo(0, LINEHEIGHT * 2);
      updateInput();
      if(isPressed(0)) {
        noPress = false;
      }
      if(currentTime.hours < 10) {
        OrbitOledDrawChar('0');
      }
      printNum(currentTime.hours);
      OrbitOledDrawChar(':');
      if(currentTime.minutes < 10) {
        OrbitOledDrawChar('0');
      }
      printNum(currentTime.minutes);
      OrbitOledDrawChar(':');
      if(currentTime.seconds < 10) {
        OrbitOledDrawChar('0');
      }
      printNum(currentTime.seconds);
      OrbitOledDrawChar(' ');
      if(currentTime.ampm) {
        OrbitOledDrawString("PM");
      }
      else {
        OrbitOledDrawString("AM");
      }
      OrbitOledUpdate();
      current = millis();
    }
    previous = millis();

    while ((current - previous < timeInterval) && noPress) {
      OrbitOledMoveTo(0, LINEHEIGHT * 2);
      updateInput();
      if(isPressed(0)) {
        noPress = false;
      }
      if(currentTime.hours < 10) {
        OrbitOledDrawChar('0');
      }
      printNum(currentTime.hours);
      OrbitOledDrawChar(' ');
      if(currentTime.minutes < 10) {
        OrbitOledDrawChar('0');
      }
      printNum(currentTime.minutes);
      OrbitOledDrawChar(' ');
      if(currentTime.seconds < 10) {
        OrbitOledDrawChar('0');
      }
      printNum(currentTime.seconds);
      OrbitOledDrawChar(' ');
      if(currentTime.ampm) {
        OrbitOledDrawString("PM");
      }
      else {
        OrbitOledDrawString("AM");
      }
      OrbitOledUpdate();
      current = millis();
    }
    previous = millis();
  }
}
