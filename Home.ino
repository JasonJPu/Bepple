const int scroll = 8.0;

void homepage() {
  OrbitOledClearBuffer();
  updateInput();

  int inputHome = ((int)input.potentiometer - 1024 / 2) / 1024;

  OrbitOledMoveTo(0, 0);
  OrbitOledPutBmp(32, 32, clockIconBitmap);
  OrbitOledMoveTo(32, 0);
  OrbitOledPutBmp(32, 32, levelIconBitmap);
  OrbitOledMoveTo(64, 0);
  OrbitOledPutBmp(32, 32, magicIconBitmap);
  OrbitOledMoveTo(96, 0);
  OrbitOledPutBmp(32, 32, turretIconBitmap);

  drawCursor(inputHome);

  if(isPressed(1)){
    resetButton(1);
    app = inputHome;
  }

  OrbitOledUpdate();
}

