void changeNum0(struct lockCombo *lock) {
  lockSetup(0, timeInterval, lock);
  if(isPressed(0)) {
    resetButton(0);
    changeNum0(lock);
  }
  else if(isPressed(1)) {
    resetButton(1);
    changeNum1(lock);
  }
}

void changeNum1(struct lockCombo *lock) {
  lockSetup(1, timeInterval, lock);
  if(isPressed(0)) {
    resetButton(0);
    changeNum0(lock);
  }
  else if(isPressed(1)) {
    resetButton(1);
    changeNum2(lock);
  }
}

void changeNum2(struct lockCombo *lock) {
  lockSetup(2, timeInterval, lock);
  if(isPressed(0)) {
    resetButton(0);
    changeNum1(lock);
  }
  else if(isPressed(1)) {
    resetButton(1);
    changeNum3(lock);
  }
}

void changeNum3(struct lockCombo *lock) {
  lockSetup(3, timeInterval, lock);
  if(isPressed(0)) {
    resetButton(0);
    changeNum2(lock);
  }
  else if(isPressed(1)) {
    resetButton(1);
    if(lock->master == true) {
      confirmLock();
    }
    else {
      checkCombo();
    }
  }
}

void confirmLock() {
  OrbitOledClear();
  OrbitOledMoveTo(0, 0);

  OrbitOledDrawString("Combination:");
  OrbitOledMoveTo(0, LINEHEIGHT);
  if(master.num[0] < 10) {
    OrbitOledDrawChar('0');
  }
  printNum(master.num[0]);
  OrbitOledDrawChar(' ');
  if(master.num[1] < 10) {
    OrbitOledDrawChar('0');
  }
  printNum(master.num[1]);
  OrbitOledDrawChar(' ');
  if(master.num[2] < 10) {
    OrbitOledDrawChar('0');
  }
  printNum(master.num[2]);
  OrbitOledDrawChar(' ');
  if(master.num[3] < 10) {
    OrbitOledDrawChar('0');
  }
  printNum(master.num[3]);

  OrbitOledMoveTo(0, 3 * LINEHEIGHT);
  OrbitOledDrawString("Press to confirm.");
  OrbitOledUpdate();
  bool noPress = true;
  while(noPress) {
    updateInput();
    if(isPressed(0) || isPressed(1)) {
      noPress = false;
    }
  }
  if(isPressed(0)) {
    resetButton(0);
    lockPrompt(&master);
  }
  else {
    resetButton(1);
  }
}

void lockPrompt(struct lockCombo *lock) {
  OrbitOledClear();
  OrbitOledMoveTo(0, 0);
  if(lock->master == true) {
    OrbitOledDrawString("Enter a combo:");
  }
  else {
    OrbitOledDrawString("Enter the combo:");
  }
  OrbitOledUpdate();
  changeNum0(lock);
}

void lockSetup(int position, long timeInterval, struct lockCombo *lock) {
  long current = millis();
  long previous = millis();
  bool noPress = true;
  while(noPress) {
    while ((current - previous < timeInterval) && noPress) {
      OrbitOledMoveTo(0, LINEHEIGHT);
      updateInput();
      if(isPressed(0) || isPressed(1)) {
        noPress = false;
      }

      int c = (int)((analogRead(Potentiometer) - (4096 / 60) / 2) / (4096 / 60));
      switch(position) {
        case 0: lock->num[0] = c; break;
        case 1: lock->num[1] = c; break;
        case 2: lock->num[2] = c; break;
        case 3: lock->num[3] = c; break;
      }

      if(lock->num[0] < 10) {
        OrbitOledDrawChar('0');
      }
      printNum(lock->num[0]);
      OrbitOledDrawChar(' ');
      if(lock->num[1] < 10) {
        OrbitOledDrawChar('0');
      }
      printNum(lock->num[1]);
      OrbitOledDrawChar(' ');
      if(lock->num[2] < 10) {
        OrbitOledDrawChar('0');
      }
      printNum(lock->num[2]);
      OrbitOledDrawChar(' ');
      if(lock->num[3] < 10) {
        OrbitOledDrawChar('0');
      }
      printNum(lock->num[3]);

      OrbitOledUpdate();
      current = millis();
    }

    previous = millis();

    while ((current - previous < timeInterval) && noPress) {
      updateInput();
      if(isPressed(0) || isPressed(1)) {
        noPress = false;
      }

      OrbitOledMoveTo(position * 3 * LINEHEIGHT, LINEHEIGHT);

      OrbitOledDrawString("  ");
      OrbitOledUpdate();
      current = millis();
    }
    previous = millis();
  }
}

void checkCombo() {
  bool same = true;
  for(int i = 0; i < 4; i++) {
    if (newCombo.num[i] != master.num[i])
      same = false;
  }
  if(same) {
    lock = false;
    return;
  }
  else {
    OrbitOledClear();
    OrbitOledMoveTo(0, 0);
    OrbitOledDrawString("Incorrect combo");
    OrbitOledMoveTo(0, LINEHEIGHT);
    OrbitOledDrawString("Please try again");
    OrbitOledUpdate();
    bool noPress = true;
    while(noPress) {
      updateInput();
      if(isAnyPressed()) {
        noPress = false;
        for(int i = 0; i < 4; i++) {
          resetButton(i);
        }
      }
    }
    lockPrompt(&newCombo);
  }
}

//lock
void checkFunctions() {
  if(lock == false) {
    if(input.switches[0] == true && input.switches[1] == true) {
      lock = true;
      lockPrompt(&newCombo);
      OrbitOledClearBuffer();
      OrbitOledMoveTo(0, 0);
    }
  }
}
