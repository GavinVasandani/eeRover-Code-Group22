const int INTERVAL = 4;

void setup() {
  Serial.begin(9600);
}

void loop() {
  for (double x = -PI; x < PI; x+= 0.025){
    int outPWM = (sin(x) + 1.0) * 127.5;
    int outPWM2 = (sin(x + PI * 0.5) + 1.0) * 127.5;
    analogWrite(9, outPWM);
    analogWrite(10, outPWM2);
    delay(INTERVAL);
  }
}