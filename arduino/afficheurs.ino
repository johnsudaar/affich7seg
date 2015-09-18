int PIN_A = 3;
int PIN_B = 4;
int PIN_C = 5;
int PIN_D = 6;
int PIN_E = 7;
int PIN_F = 8;
int PIN_G = 9;
int PIN_CTL_1 = 10;
int PIN_CTL_2 = 11;
int PIN_CTL_3 = 12;
int PIN_CTL_4 = 13;

int BLINK_DELAY = 4;
int USED_DISPLAY = 4;
int LOADING_DELAY = 65;


long received = -1;
long deltaTime = 0;
boolean timeSet = false;

void on(int pin) {
  digitalWrite(pin, HIGH);
}

void off(int pin) {
  digitalWrite(pin, LOW);
}

void printTo7Seg(int num) {
 switch(num) {
   case 0:
     on(PIN_A);
     on(PIN_B);
     on(PIN_C);
     on(PIN_D);
     on(PIN_E);
     on(PIN_F);
     off(PIN_G);
     break;
   case 1:
     off(PIN_A);
     on(PIN_B);
     on(PIN_C);
     off(PIN_D);
     off(PIN_E);
     off(PIN_F);
     off(PIN_G);
     break;
   case 2:
     on(PIN_A);
     on(PIN_B);
     off(PIN_C);
     on(PIN_D);
     on(PIN_E);
     off(PIN_F);
     on(PIN_G);
     break;
   case 3:
     on(PIN_A);
     on(PIN_B);
     on(PIN_C);
     on(PIN_D);
     off(PIN_E);
     off(PIN_F);
     on(PIN_G);
     break;
   case 4:
     off(PIN_A);
     on(PIN_B);
     on(PIN_C);
     off(PIN_D);
     off(PIN_E);
     on(PIN_F);
     on(PIN_G);
     break;
   case 5:
     on(PIN_A);
     off(PIN_B);
     on(PIN_C);
     on(PIN_D);
     off(PIN_E);
     on(PIN_F);
     on(PIN_G);
     break;
   case 6:
     on(PIN_A);
     off(PIN_B);
     on(PIN_C);
     on(PIN_D);
     on(PIN_E);
     on(PIN_F);
     on(PIN_G);
     break;
   case 7:
     on(PIN_A);
     on(PIN_B);
     on(PIN_C);
     off(PIN_D);
     off(PIN_E);
     off(PIN_F);
     off(PIN_G);
     break;
   case 8:
     on(PIN_A);
     on(PIN_B);
     on(PIN_C);
     on(PIN_D);
     on(PIN_E);
     on(PIN_F);
     on(PIN_G);
     break;
   case 9:
     on(PIN_A);
     on(PIN_B);
     on(PIN_C);
     on(PIN_D);
     off(PIN_E);
     on(PIN_F);
     on(PIN_G);
     break;
 }
}

void clearCTL(){
  on(PIN_CTL_1);
  on(PIN_CTL_2);
  on(PIN_CTL_3);
  on(PIN_CTL_4);
}

void clearAllSeg(){
  off(PIN_A);
  off(PIN_B);
  off(PIN_C);
  off(PIN_D);
  off(PIN_E);
  off(PIN_F);
  off(PIN_G);
}

void clearScreen(){
  clearCTL();
  clearAllSeg();
}

void activateCTL(int num){
  switch(num) {
    case 1:
      off(PIN_CTL_1);
      break;
    case 2:
      off(PIN_CTL_2);
      break;
    case 3:
      off(PIN_CTL_3);
      break;
    case 4:
      off(PIN_CTL_4);
      break;
  }
}

void printNumToScreen(int num, int ctl) {
  clearCTL();
  printTo7Seg(num);
  activateCTL(ctl);
  delay(BLINK_DELAY);
}

void printNum(int num) {
  for (int i = 1; i <= USED_DISPLAY; i++){
    printNumToScreen(num%10, i);
    num /= 10;
  }
  clearCTL();
}

void setAll(){
  for(int i = 1; i<= 4 ; i++) {
    clearCTL(); 
    activateCTL(i);
    delay(BLINK_DELAY);
  }
  clearCTL();
}

void setSeg(int seg, int screen){
  clearScreen();
  on(seg);
  activateCTL(screen);
  delay(BLINK_DELAY);
}

void loadingStep(int s){
  clearScreen();
  switch(s) {
    case 0:
      setSeg(PIN_A, 1);
      setSeg(PIN_D, 4);
      break;
    case 1:
      setSeg(PIN_B, 1);
      setSeg(PIN_E, 4);
      break;
    case 2:
      setSeg(PIN_C, 1);
      setSeg(PIN_F, 4);
      break;
    case 3:
      setSeg(PIN_D, 1);
      setSeg(PIN_A, 4);
      break;
    case 4:
      setSeg(PIN_D, 2);
      setSeg(PIN_A, 3);
      break;
    case 5:
      setSeg(PIN_D, 3);
      setSeg(PIN_A, 2);
      break;
   }
}

void printTime() {
  if(! timeSet) {
   loadingStep((millis() / LOADING_DELAY)%6);
  } else {
    int cur_minutes = ((millis() + deltaTime) / 60000)%3600;
    int minutes = cur_minutes % 60;
    int heures = cur_minutes / 60;
    printNum(heures * 100 + minutes);
  }
}

void setup() {
  pinMode(PIN_A, OUTPUT);
  pinMode(PIN_B, OUTPUT);
  pinMode(PIN_C, OUTPUT);
  pinMode(PIN_D, OUTPUT);
  pinMode(PIN_E, OUTPUT);
  pinMode(PIN_F, OUTPUT);
  pinMode(PIN_G, OUTPUT);
  pinMode(PIN_CTL_1, OUTPUT);
  pinMode(PIN_CTL_2, OUTPUT);
  pinMode(PIN_CTL_3, OUTPUT);
  pinMode(PIN_CTL_4, OUTPUT);
  
  Serial.begin(9600);
}

int mode = 0;

void loop() {
  if (Serial.available() > 0 ) {
    delay(10);
    long temp = 0;
    boolean isClock = false, neg = false, cont = true;
    if (Serial.peek() == 'H') {
      isClock = true;
      Serial.read();
    }
    if (Serial.peek() ==  'I') {
      while(Serial.available() > 0) {
        Serial.read();
      }
    } else {
      if (Serial.peek() == '-') {
        neg = true; 
        Serial.read();
      }
      while(Serial.available() > 0) {
        temp *= 10;
        temp += Serial.read() - '0';
      }
      if (neg) {
        temp *= -1; 
      }
      if(isClock) {
        temp *= 1000;
        received = -1;
        deltaTime = temp - millis();
        timeSet = true;
      } else {
        received = temp;
      }
    }
  }
  if (received < 0 ){
    printTime();
  } else {
    printNum(received);
  }
}


