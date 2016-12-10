//"Enemies"
struct square {
  bool alive;
  float pos[2];
  float direction;
  float rotation;
};

//Bullets
struct bullet {
  bool shot;
  float pos[2];
  float direction;
};

const int dimension = 8;
const int numOfSquares = 10;
const int numOfBullets = 6;
const unsigned long blinkInterval = 100;
const unsigned long spawnInterval = 1000;
unsigned long lastSpawned = 0;
unsigned long lastBlinkGreen = 0;
unsigned long lastBlinkRed = 0;
unsigned long startTime;
struct square squares[numOfSquares];
struct bullet bullets[numOfBullets] = { { .shot = false, .pos = {WIDTH / 2, HEIGHT / 2} } };
float turretSpin;
float speed;
int hp;
int kill;

int turretBitmap[] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0,
  0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0,
  0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1,
  0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1,
  0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0,
  0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

int squareBitmap[] = {
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1
};

int heartBitmap[] = {
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 1, 1, 0, 0, 1, 1, 0,
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1,
  0, 1, 1, 1, 1, 1, 1, 0,
  0, 0, 1, 1, 1, 1, 0, 0,
  0, 0, 0, 1, 1, 0, 0, 0
};

//Draws bitmap of specified dimension (as a square), rotated rad radians cw with center at x, y
void drawRotated(int *bitmap, int dimension, float rad, int x, int y){
  int wholeSize = round(dimension * (fabs(cos(rad)) + fabs(sin(rad))));
  bool toDraw[wholeSize][wholeSize];
  memset(toDraw, false, wholeSize * wholeSize * sizeof(bool));
  
  int rotatedX;
  int rotatedY;
  int workingX;
  int workingY;
  
  for(int i = -1 * dimension / 2; i < dimension / 2; i++){
    for(int j =  -1 * dimension / 2; j < dimension / 2; j++){
      if(bitmap[(dimension/2 + i) * dimension + (dimension/2 + j)] == 1){
        rotatedX = round(i * cos(rad) - j * sin(rad));
        rotatedY = round(i * sin(rad) + j * cos(rad));
        if(rotatedX >= 0){
          workingX = wholeSize / 2 + rotatedX < wholeSize ? wholeSize / 2 + rotatedX : wholeSize - 1;
        } else {
          workingX = wholeSize / 2 + rotatedX >= 0 ? wholeSize / 2 + rotatedX : 0;
        }
        if(rotatedY >= 0){
          workingY = wholeSize / 2 + rotatedY < wholeSize ? wholeSize / 2 + rotatedY : wholeSize - 1;
        } else {
          workingY = wholeSize / 2 + rotatedY >= 0 ? wholeSize / 2 + rotatedY : 0;
        }
        toDraw[workingX][workingY] = true;
      }
    }
  }
  for(int i = -1 * wholeSize / 2; i < wholeSize / 2; i++){
    for(int j =  -1 * wholeSize / 2; j < wholeSize / 2; j++){
      OrbitOledMoveTo(x + i, y + j);
      if(toDraw[wholeSize / 2 + i][wholeSize / 2 + j] == true){
        OrbitOledDrawPixel();
      }
    }
  }
}

void spawnEnemy(){
  if(millis() - lastSpawned > spawnInterval){
    for(int i = 0; i < numOfSquares; i++){
      if(squares[i].alive == false){
        int randomX, randomY, lr;
        while(!((randomX > -10 && randomX < WIDTH + 10) && (randomY > -10 && randomY < HEIGHT + 10))){
          lr = random(0, 2);
          if(lr == 0){
            randomX = random(-1 * WIDTH / 2, WIDTH / 2 - WIDTH / 4);
          } else {
            randomX = random(WIDTH / 2 + WIDTH / 4, WIDTH + WIDTH / 2);
          }
          randomY = random(-1 * HEIGHT / 2, HEIGHT + HEIGHT / 2);
        }
        squares[i].pos[0] = randomX;
        squares[i].pos[1] = randomY;

        squares[i].direction = atan2(HEIGHT / 2 - squares[i].pos[1], WIDTH / 2 - squares[i].pos[0]);

        squares[i].alive = true;
        
        break;
      }
    }
    lastSpawned = millis();
  }
}

//Shoot a bullet when button is pressed
void shoot(){
  if(isPressed(3)){
    for(int i = 0; i < numOfBullets; i++){
      if(bullets[i].shot == false){
        bullets[i].direction = turretSpin + PI / 2;
        bullets[i].shot = true;
        break;
      }
    }
    resetButton(3);
  }
}

void deadScreen(){
  OrbitOledMoveTo(32, 0);
  OrbitOledDrawString("You shot");
  OrbitOledMoveTo(WIDTH / 2 - 8 / 2 * (kill == 0 ? 1 : ceil(log10(kill+1))), 8);
  printNum(kill);
  OrbitOledMoveTo(32, 16);
  OrbitOledDrawString("enemies.");
  OrbitOledMoveTo(0, 24);
  OrbitOledDrawString("Play again? BTN1");
}

void TurretInit(){
  AccelInit();
  OrbitOledClearBuffer();
  OrbitOledMoveTo(0, 0);
  OrbitOledUpdate();

  startTime = millis();

  for(int i = 0; i < numOfSquares; i++){
    squares[i] = { .alive = false };
  }

  hp = 3;
  kill = 0;
}

void TurretUpdate(){
  accelUpdate();
  updateInput();
  accelX = readAccelX();
  accelY = readAccelY();

  speed = 0.01 * (millis() - startTime) / 5000;

  if(hp != 0){
    turretSpin = -1 * accelX / fabs(accelX) * PI / 2 - atan(accelY / accelX);

    shoot();
  
    for(int i = 0; i < numOfBullets; i++){
      if(bullets[i].shot == true){
        bullets[i].pos[0] += 0.1 * cos(bullets[i].direction);
        bullets[i].pos[1] += 0.1 * sin(bullets[i].direction);
  
        for(int j = 0; j < numOfSquares; j++){
          if(squares[j].alive == true && sqrt(pow(bullets[i].pos[0]-squares[j].pos[0], 2) + pow(bullets[i].pos[1]-squares[j].pos[1], 2)) < dimension / 2 + 1){
            digitalWrite(GREEN_LED, HIGH);
            lastBlinkGreen = millis();
            squares[j].alive = false;
            squares[j].pos[0] = 1000;
            squares[j].pos[1] = 1000;
            bullets[i].shot = false;
            bullets[i].pos[0] = WIDTH / 2;
            bullets[i].pos[1] = HEIGHT / 2;
  
            kill++;
          }
        }
        
        if(bullets[i].pos[0] > WIDTH || bullets[i].pos[0] < 0 || bullets[i].pos[1] > HEIGHT || bullets[i].pos[1] < 0){
          bullets[i].shot = false;
          bullets[i].pos[0] = WIDTH / 2;
          bullets[i].pos[1] = HEIGHT / 2;
        }
      }
    }
  
    spawnEnemy();
  
    for(int i = 0; i < numOfSquares; i++){
      if(squares[i].alive == true){
        squares[i].pos[0] += speed * cos(squares[i].direction);
        squares[i].pos[1] += speed * sin(squares[i].direction);
        if((cos(squares[i].direction) < 0 && squares[i].pos[0] < 0) || 
        (cos(squares[i].direction) > 0 && squares[i].pos[0] > WIDTH) || 
        (sin(squares[i].direction) < 0 && squares[i].pos[1] < 0) || 
        (sin(squares[i].direction) > 0 && squares[i].pos[1] > HEIGHT)){
          squares[i].alive = false;
        }
        if(sqrt(pow(squares[i].pos[0] - WIDTH/2, 2) + pow(squares[i].pos[1] - HEIGHT/2, 2)) < dimension / 2){
          digitalWrite(RED_LED, HIGH);
          lastBlinkRed = millis();
          hp--;
          squares[i].alive = false;
        }
      }
    }
  } else {
    if(isPressed(2)){
      TurretInit();
      resetButton(2);
    }
  }

  if(millis() - lastBlinkGreen > blinkInterval){
    digitalWrite(GREEN_LED, LOW);
  }
  if(millis() - lastBlinkRed > blinkInterval * 2){
    digitalWrite(RED_LED, LOW);
  }
}

void TurretUiUpdate(){
  OrbitOledClearBuffer();
  if(hp != 0){
    drawRotated(turretBitmap, 16, turretSpin, WIDTH/2, HEIGHT/2);

    for(int i = 0; i < numOfBullets; i++){
      if(bullets[i].shot == true){
        OrbitOledMoveTo(bullets[i].pos[0], bullets[i].pos[1]);
        OrbitOledDrawPixel();
        OrbitOledMoveTo(bullets[i].pos[0] + 1, bullets[i].pos[1]);
        OrbitOledDrawPixel();
        OrbitOledMoveTo(bullets[i].pos[0], bullets[i].pos[1] + 1);
        OrbitOledDrawPixel();
        OrbitOledMoveTo(bullets[i].pos[0] + 1, bullets[i].pos[1] + 1);
        OrbitOledDrawPixel();
      }
    }
  
    for(int i = 0; i < numOfSquares; i++){
      if(squares[i].alive == true){
        drawRotated(squareBitmap, dimension, squares[i].direction, squares[i].pos[0], squares[i].pos[1]);
      }
    }
  
    for(int i = 0; i < hp; i++){
      for(int j = 0; j < 8; j++){
        for(int k = 0; k < 8; k++){
          if(heartBitmap[j * 8 + k] == 1){
            OrbitOledMoveTo(WIDTH - 8 - 1 + k, 8 * i + j - 1);
            OrbitOledDrawPixel();
          }
        }
      }
    }
  
    OrbitOledMoveTo(WIDTH - 1 - 8 * (kill == 0 ? 1 : ceil(log10(kill+1))), 8 * 3);
    printNum(kill); 
    
  } else {
    deadScreen();
  }
  
  OrbitOledUpdate();
}

