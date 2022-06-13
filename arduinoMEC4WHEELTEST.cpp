const int INTERVAL = 4;

const int pin_DIRFR = 8;
const int pin_DIRFL = 7;
const int pin_DIRBR = 9;
const int pin_DIRBL = 10;

const int pin_PWMFR = 6;
const int pin_PWMFL = 3;
const int pin_PWMBR = 11;
const int pin_PWMBL = 12;

void setup() {
  Serial.begin(115200);
  pinMode(pin_DIRFR, OUTPUT);
  pinMode(pin_DIRFL, OUTPUT);
  pinMode(pin_DIRBR, OUTPUT);
  pinMode(pin_DIRBL, OUTPUT);

  pinMode(pin_PWMFR, OUTPUT);
  pinMode(pin_PWMFL, OUTPUT);
  pinMode(pin_PWMBR, OUTPUT);
  pinMode(pin_PWMBL, OUTPUT);  
}

void loop() {
  int k = 0;
  if (k == 0){
    //ROVER MOVE FRONT
    analogWrite(pin_PWMFR, 200);  //REN
    analogWrite(pin_PWMFL, 200);  //LEN
    analogWrite(pin_PWMBR, 200);  //REN
    analogWrite(pin_PWMBL, 200);  //LEN

    digitalWrite(pin_DIRFR, LOW);  //RDIR
    digitalWrite(pin_DIRFL, LOW);  //LDIR
    digitalWrite(pin_DIRBR, LOW);  //RDIR
    digitalWrite(pin_DIRBL, LOW);  //LDIR
  }
  else if(k == 1){
    //should RF
    analogWrite(pin_PWMFR, 220);  //REN
    analogWrite(pin_PWMFL, 220);  //LEN
    digitalWrite(pin_DIRFR, HIGH);  //RDIR
    digitalWrite(pin_DIRFL, HIGH);  //LDIR
  }
  else if(k == 2){
    //should FF
    analogWrite(pin_PWMFR, 220);  //REN
    analogWrite(pin_PWMFL, 200);  //LEN
    digitalWrite(pin_DIRFR, HIGH);  //RDIR
    digitalWrite(pin_DIRFL, LOW);  //LDIR
  }
  else if(k == 3){
    //should RR
    analogWrite(pin_PWMFR, 200);  //REN
    analogWrite(pin_PWMFL, 220);  //LEN
    digitalWrite(pin_DIRFR, LOW);  //RDIR
    digitalWrite(pin_DIRFL, HIGH);  //LDIR
  }
  else if(k == 4){
    digitalWrite(pin_DIRFR, HIGH);  //RDIR
    digitalWrite(pin_DIRFL, LOW);  //LDIR
  }
  else{
    digitalWrite(LED_BUILTIN, HIGH);
  }
}