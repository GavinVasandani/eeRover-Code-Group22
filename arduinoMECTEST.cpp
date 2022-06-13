const int INTERVAL = 4;

const int pin_DIRR = 10;
const int pin_DIRL = 9;
const int pin_PWMR = 8;
const int pin_PWML = 6;

void setup() {
  Serial.begin(115200);
  pinMode(pin_DIRR, OUTPUT);
  pinMode(pin_DIRL, OUTPUT);
  pinMode(pin_PWMR, OUTPUT);
  pinMode(pin_PWML, OUTPUT); 
}

void loop() {
  int k = 2;
  if (k == 0){
    //should FR
    analogWrite(pin_PWMR, 200);  //REN
    analogWrite(pin_PWML, 200);  //LEN
    digitalWrite(pin_DIRR, LOW);  //RDIR
    digitalWrite(pin_DIRL, LOW);  //LDIR
  }
  else if(k == 1){
    //should RF
    analogWrite(pin_PWMR, 220);  //REN
    analogWrite(pin_PWML, 220);  //LEN
    digitalWrite(pin_DIRR, HIGH);  //RDIR
    digitalWrite(pin_DIRL, HIGH);  //LDIR
  }
  else if(k == 2){
    //should FF
    analogWrite(pin_PWMR, 220);  //REN
    analogWrite(pin_PWML, 200);  //LEN
    digitalWrite(pin_DIRR, HIGH);  //RDIR
    digitalWrite(pin_DIRL, LOW);  //LDIR
  }
  else if(k == 3){
    //should RR
    analogWrite(pin_PWMR, 200);  //REN
    analogWrite(pin_PWML, 220);  //LEN
    digitalWrite(pin_DIRR, LOW);  //RDIR
    digitalWrite(pin_DIRL, HIGH);  //LDIR
  }
  else if(k == 4){
    digitalWrite(pin_DIRR, HIGH);  //RDIR
    digitalWrite(pin_DIRL, LOW);  //LDIR
  }
  else{
    digitalWrite(LED_BUILTIN, HIGH);
  }
}