extern "C"{
#include <delay.h>
#include <FillPat.h>
#include <LaunchPad.h>
#include <OrbitBoosterPackDefs.h>
#include <OrbitOled.h>
#include <OrbitOledChar.h>
#include <OrbitOledGrph.h>
#include <math.h>
}

#include <math.h>

#define WIDTH 128
#define HEIGHT 32
#define LINEHEIGHT 8

const uint32_t Potentiometer = PE_3;
const uint32_t Switches[2] = {PA_7, PA_6};
const uint32_t Buttons[4] = {PF_4, PF_0, PD_2, PE_0};

struct inputState {
  float potentiometer;
  bool switches[2];
  bool buttons[4];
  bool pressed[4];
} input;

struct timeDisplay {
  int hours;
  int minutes;
  int seconds;
  int ampm;
} times;

const long interval = 50;
unsigned long previousMillis = 0;
unsigned long currentMillis;
unsigned long clockTime;

struct lockCombo {
  int num[4];
  bool master;
};

struct lockCombo master = { {0}, true };
struct lockCombo newCombo = { {0}, false };

float accelX;
float accelY;

bool lock = false;

int app = -1;
//-1: homepage
//0: clock
//1: level
//2: magic 8 ball
//3: turret game

//Icons declarations
char clockIconBitmap[32 * 4];
char levelIconBitmap[32 * 4];
char magicIconBitmap[32 * 4];
char turretIconBitmap[32 * 4];

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(50);
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  for(int i = 0; i < 4; i++) {
    if(i <= 1) {
      pinMode(Buttons[i], INPUT_PULLUP);
    }
    else {
      pinMode(Buttons[i], INPUT);
    }
  }
  for(int i = 0; i < 2; i++) {
    pinMode(Switches[i], INPUT);
  }
  inputInit();
  OrbitOledInit();
  OrbitOledSetCharUpdate(0);

  WireInit();
  IconInit();
  LevelInit();
  Magic8BallInit();
  TurretInit();

  //Setup lock and clock
  lockPrompt(&master);
  timePrompt();
}

void loop() {
  updateInput();

  switch(app){
    case -1:
      updateOled(homepage);
      break;
    case 0:
      timeClock();
      break;
    case 1:
      levelUpdate();
      updateOled(levelUiUpdate);
      break;
    case 2:
      Magic8BallUpdate();
      updateOled(Magic8BallUiUpdate);
      break;
    case 3:
      TurretUpdate();
      updateOled(TurretUiUpdate);
      break;
  }
  
  if(isPressed(0)) {
    app = -1;
    for(int i = 0; i < 4; i++) {
      resetButton(i);
    }
    resetButton(0);
  }
}
