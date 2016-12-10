char *displayCircleBinary[] = {
  "00000000", "00001111", "11110000", "00000000",
  "00000000", "01110000", "00001110", "00000000",
  "00000001", "10000000", "00000001", "10000000",
  "00000010", "00000000", "00000000", "01000000",
  "00000100", "00000000", "00000000", "00100000",
  "00001000", "00000000", "00000000", "00010000",
  "00010000", "00000000", "00000000", "00001000",
  "00100000", "00000000", "00000000", "00000100",
  "00100000", "00000000", "00000000", "00000100",
  "01000000", "00000000", "00000000", "00000010",
  "01000000", "00000000", "00000000", "00000010",
  "01000000", "00000000", "00000000", "00000010",
  "10000000", "00000000", "00000000", "00000001",
  "10000000", "00000001", "10000000", "00000001",
  "10000000", "00000001", "10000000", "00000001",
  "10000000", "00000111", "11100000", "00000001",
  "10000000", "00000111", "11100000", "00000001",
  "10000000", "00000001", "10000000", "00000001",
  "10000000", "00000001", "10000000", "00000001",
  "10000000", "00000000", "00000000", "00000001",
  "01000000", "00000000", "00000000", "00000010",
  "01000000", "00000000", "00000000", "00000010",
  "01000000", "00000000", "00000000", "00000010",
  "00100000", "00000000", "00000000", "00000100",
  "00100000", "00000000", "00000000", "00000100",
  "00010000", "00000000", "00000000", "00001000",
  "00001000", "00000000", "00000000", "00010000",
  "00000100", "00000000", "00000000", "00100000",
  "00000010", "00000000", "00000000", "01000000",
  "00000001", "10000000", "00000001", "10000000",
  "00000000", "01110000", "00001110", "00000000",
  "00000000", "00001111", "11110000", "00000000"
};

char displayCircleBitmap[32 * 4];

char beadPos[2];

void LevelInit(){
  AccelInit();
  binaryToBitmap(displayCircleBinary, displayCircleBitmap, 32 * 4);
  OrbitOledClearBuffer();
  OrbitOledMoveTo(0, 0);
  OrbitOledUpdate();
}

void drawBead(int x, int y){
  OrbitOledMoveTo(x, y);
  OrbitOledDrawPixel();
  OrbitOledMoveTo(x+1, y);
  OrbitOledDrawPixel();
  OrbitOledMoveTo(x, y+1);
  OrbitOledDrawPixel();
  OrbitOledMoveTo(x+1, y+1);
  OrbitOledDrawPixel();
}

void beadPosUpdate(){
  float x = readAccelX();
  float y = readAccelY();
  beadPos[0] = 64 + (int) (x / 90 * pow((x * x + y * y), 0.5) / 90 * 16) - 1;
  beadPos[1] = 16 - (int) (y / 90 * pow((x * x + y * y), 0.5) / 89 * 16) - 1;
}

void levelUpdate(){
  accelUpdate();
  accelX = readAccelX();
  accelY = readAccelY();
  beadPosUpdate();
}

void levelUiUpdate(){
  OrbitOledClearBuffer();

  //Draw outer circle
  OrbitOledMoveTo(48, 0);
  OrbitOledPutBmp(32, 32, displayCircleBitmap);
  //Final line is invisible for some reason...
  for(int i = 0; i < 8; i++){
    OrbitOledMoveTo(60 + i, 31);
    OrbitOledDrawPixel();
  }

  //Draw level bead
  OrbitOledMoveTo(beadPos[0], beadPos[1]);
  drawBead(beadPos[0], beadPos[1]);

  //Print text on screen
  OrbitOledMoveTo(0, 6);
  OrbitOledDrawString("X:");
  printNum(accelX);
  OrbitOledMoveTo(0, 18);
  OrbitOledDrawString("Y:");
  printNum(accelY);
  OrbitOledMoveTo(90, 6);
  OrbitOledDrawString("Dot");
  OrbitOledMoveTo(90, 18);
  OrbitOledDrawString("Level");
  
  OrbitOledUpdate();
}


