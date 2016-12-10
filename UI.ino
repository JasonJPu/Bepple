void inputInit() {
  input = {0};
  times = {0};
}

void updateInput() {
  checkFunctions();
  input.potentiometer = analogRead(Potentiometer);

  for(int i = 0; i < 4; i++) {
    bool lastButtonState = input.buttons[i];
    input.buttons[i] = digitalRead(Buttons[i]);
    if(input.buttons[i] != lastButtonState) {
      if(i <= 1) {
        if(input.buttons[i] == false) {
          input.pressed[i] = true;
        }
      }
      else {
        if(input.buttons[i] == true) {
          input.pressed[i] = true;
        }
      }
    }
  }

  for(int i = 0; i < 2; i++) {
    input.switches[i] = digitalRead(Switches[i]);
  }

}

bool isPressed(int i) {
  if(input.pressed[i] == true) {
    return 1;
  }
  return 0;
}

void resetButton(int i) {
  input.pressed[i] = false;
}

bool isAnyPressed() {
  bool press = false;
  for(int i = 0; i < 4; i++) {
    if(input.pressed[i]) {
      press = true;
    }
  }
  return press;
}
