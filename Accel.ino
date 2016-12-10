const uint8_t accelerometer = 0x1D;
const size_t accelDataLength = 6;
uint32_t accelData[accelDataLength] = {0};

float previousX = 0;
float previousY = 0;
float accumulated = 0;
const long shakeDetectInterval = 500;
unsigned long previousRead = 0;

void AccelInit(){
  WireWriteRegister(accelerometer, 0x31, 1);
  WireWriteRegister(accelerometer, 0x2D, 1 << 3);
}

void accelUpdate(){
  WireWriteByte(accelerometer, 0x32);
  WireRequestArray(accelerometer, accelData, accelDataLength);
}

float readAccelX(){
  int x1 = (int) accelData[0];
  int x2 = (int) accelData[1];

  //Tilt left
  if(x2 == 0) return (x1 / -118.0 * 90 >= -90 ? x1 / -120.0 * 90 : -90.0);
  //Tilt right
  if(x2 == 255) return ((255 - x1) / 132.0 * 90 <= 90 ? (255 - x1) / 135.0 * 90 : 90.0);
}

float readAccelY(){
  int y1 = (int) accelData[2];
  int y2 = (int) accelData[3];

  //Pitch Forward
  if(y2 == 0) return (y1 / -120.0 * 90 >= -90 ? y1 / -120.0 * 90 : -90.0);
  //Pitch Backward
  if(y2 == 255) return ((255 - y1) / 130.0 * 90 <= 90 ? (255 - y1) / 135.0 * 90 : 90.0);
}

float readAccelZ(){
  int z1 = (int) accelData[4];
  int z2 = (int) accelData[5];

  //Yaw.. ??
  if(z2 == 0) return (z1 / -120.0 * 90 >= -90 ? z1 / -120.0 * 90 : -90.0);
  //Yaw.. ??
  if(z2 == 255) return ((255 - z1) / 135.0 * 90 <= 90 ? (255 - z1) / 135.0 * 90 : 90.0);
}

//"Shake" detection
bool isShaking(int sensitivity){
  unsigned long now = millis();
  if(now - previousRead > shakeDetectInterval){
    accumulated = 0;
    previousRead = now;
  } else {
    float diffX = fabs(previousX - readAccelX());
    float diffY = fabs(previousY - readAccelY());
    accumulated += sqrt(diffX * diffX + diffY * diffY);
    previousX = readAccelX();
    previousY = readAccelY();
  }
  return accumulated > sensitivity;
}
