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
int LOADING_DELAY = 100;


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
}

void printNum(int num) {
  for (int i = 1; i <= USED_DISPLAY; i++){
    printNumToScreen(num%10, i);
    num /= 10;
    delay(BLINK_DELAY);
  }
  clearCTL();
}

void setAll(){
  for(int i = 1; i<= 4 ; i++) {
    clearCTL(); 
    delay(BLINK_DELAY);
    activateCTL(i);
  }
  clearCTL();
}

void loadingStep(int s){
  off(PIN_A);
  off(PIN_B);
  off(PIN_C);
  off(PIN_D);
  off(PIN_E);
  off(PIN_F);
  off(PIN_G);
  switch(s) {
    case 0:
      on(PIN_A);
      break;
    case 1:
      on(PIN_B);
      break;
    case 2:
      on(PIN_C);
      break;
    case 3:
      on(PIN_D);
      break;
    case 4:
      on(PIN_E);
      break;
    case 5:
      on(PIN_F);
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
  long temp;
  if (Serial.available() > 0 ) {
    temp = Serial.parseFloat();
    if (mode == 0) {
      if (temp == -1) {
        mode = 1;
        Serial.println("Clock mode activated, awaiting synchro");
      } else {
        Serial.println("Going into display mode");
        mode = 2;
        received = temp;
        Serial.print("Received: ");
        Serial.println(received);
      }
    } else if (mode == 1){
     deltaTime = (temp*1000 - millis());
     timeSet = true;
     Serial.print("Clock set to ");
     Serial.print(temp / 60);
     Serial.print(" : ");
     Serial.println(temp % 60);
    } else {
      received = temp;
      Serial.print("Printing : ");
      Serial.println(received);
    }
  }
  
  if (received < 0 ){
    printTime();
  } else {
    printNum(received);
  }
}


