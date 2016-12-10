//For ease of bitmap drawing
int binaryToDecimal(char a[], int n){
  int value = 0;
  for(int i = n-1; i >= 0; i--){
    if(a[i] == '1'){
      value += pow(2, n-i-1);
    }
  }
  return value;
}

//Takes *Binary variable, converts to bitmap and stores in *Bitmap variables
void binaryToBitmap(char *a[], char b[], int n){
  char c[n];
  for(int i = 0; i < n; i++){
    c[i] = binaryToDecimal(a[i], 8);
  }
  for(int i = 3; i >= 0; i--){
    for(int j = 0; j < n / 4; j++){
      b[j + (3 - i) * n / 4] = c[j * 4 + i];
    }
  }
}

//Print numbers more easily on display
void printNum(float number){
  char buffer[10];
  OrbitOledDrawString(itoa(number, buffer, 10));
  OrbitOledUpdate();
}

//Updates OrbitOled
void updateOled(void (*f)()){
  currentMillis = millis();
  if(currentMillis - previousMillis > interval){
    previousMillis = currentMillis;
    (*f)();
  }
}

//Updates OrbitOled with specified interval
void updateOled(void (*f)(), long i){
  currentMillis = millis();
  if(currentMillis - previousMillis > i){
    previousMillis = currentMillis;
    (*f)();
  }
}

