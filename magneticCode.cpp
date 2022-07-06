const int pinHall = A0; 
void setup() {
  pinMode(pinHall, INPUT);
  Serial.begin(9600);
} 
void loop() {
 /*
   int sensorValue = analogRead(A0);
   Serial.println(sensorValue);
   delay(1);
  
 */
  //we measure 10 times adn make the mean
  long measure = 0;
  for(int i = 0; i < 10; i++){
      int measure += analogRead(pinHall);
  }
  measure /= 10; 
  measure -= init_measure; 
  
  if (measure > 2.5){
    Serial.println(" South");
  }

  else if(measure < -2.5){
    Serial.println(" North");
  }

  else{
    Serial.println(" Idk");
  }

const int pinHall = A0;

long init_measure = 0;

 

void setup() {

  pinMode(pinHall, INPUT);

  Serial.begin(9600);

  delay(100);

  for(int i = 0; i < 100; i++){

      init_measure += analogRead(pinHall);

  }

  init_measure /= 100; 

  Serial.print("adc value = ");

  Serial.println(init_measure);

  delay(2000);

}

void loop() {

  long measure = 0;

  for(int i = 0; i < 10; i++){
      measure += analogRead(pinHall);
  }

  measure /= 10; 

  measure -= init_measure;

  Serial.print("adc Voltage = ");

  Serial.print(measure);

  delay(2000);

  if (measure > 2.5){
    Serial.println(" South");
  }

  else if(measure < -2.5){
    Serial.println(" North");
  }

  else{
    Serial.println(" Idk");
  }

  delay(1000);

}

