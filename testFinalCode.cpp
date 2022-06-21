#include <LiquidCrystal.h>

const int pinMagnetic = A0;
const int pinIR = A1;
const int pinRadio = 1;
//const int pinUltra = 7;

int northCount = 0;
int southCount = 0;
int noneCount = 0;

int SAMPLE_CYCLE = 5;     // number of complete periods we sample and take median of
bool DEBUG_MODE = false;

double FrequencyTolerance = 75.0;
double ThiotimolineFrequency = 353.0;
double NetheriteFrequency = 571.0;

void setup () {

  pinMode(pinMagnetic, INPUT);
  pinMode(pinIR, INPUT); 
  pinMode(pinRadio, INPUT);
  //pinMode(pinUltra, INPUT);
  Serial.begin(9600);

}

long backgroundMagnetic () {

    long init_measure = 0;
    
    for(int i = 0; i < 100; i++) {

      init_measure += analogRead(pinMagnetic);

  }

  init_measure /= 100; 

  Serial.print("adc value = ");

  Serial.println(init_measure);

    return init_measure;

  delay(2000);

}

//Magnetic Sensor Code
String magneticSensor() {

    long init_measure = backgroundMagnetic();

    String magDirect;
    long measure = 0;

    for(int i = 0; i < 10; i++){

      measure += analogRead(pinMagnetic);

    }

     measure /= 10;  
     measure -= init_measure;

    Serial.print("adc Voltage = ");

    Serial.print(measure);

    delay(2000);

  //voltage in mV
  /*float outputV = measure * 5000.0 / 1023;
  Serial.print("Output Voltage = ");
  Serial.print(outputV);
  Serial.println(" mV   ");
  
  //flux density
  float magneticFlux =  (outputV * 53.33 - 133.3)-79160.39;
  Serial.print("Magnetic Flux Density = ");
  Serial.print(magneticFlux);
  Serial.println(" mT");  
  delay(2000);*/

  if (measure > 2.5){
    //magDirect = "South";
    southCount += 1;
    northCount = 0;
    noneCount = 0; 
    //Serial.println("South");
  }
  else if(measure < -2.5){
    //magDirect = "North";
    northCount += 1; 
    southCount = 0;
    noneCount = 0;
    //Serial.println("North");
  }
  else{
    //magDirect = "None";
    noneCount += 1;
    northCount = 0;
    southCount = 0;
    //Serial.println("None");
  }

  if (northCount == 3) {
      Serial.println("Average of 3 is: North");
      northCount = 0; 
      magDirect = "North";
      return magDirect;
  }
  else if (southCount == 3) {
      Serial.println("Average of 3 is: South");
      southCount = 0; 
      magDirect = "South";
      return magDirect;
  }
  else if (noneCount == 3) {
      Serial.println("Average of 3 is: None");
      noneCount = 0; 
      magDirect = "None";
      return magDirect;
  }
  else {
    magDirect = "Still detecting";
    return magDirect;
  }

}

//IR Sensor New
double GetIrFrequency() 
{
  long total_duration = 0;
  double frequency = 0;
  long total_duration_averaged = 0;  

  // starts sampling the values into samples
  for (int i = 0; i < SAMPLE_CYCLE; i++) {
    long low_duration = pulseIn(pinIR, LOW);
    long high_duration = pulseIn(pinIR, HIGH);
    total_duration += high_duration + low_duration;
    if (DEBUG_MODE) {
      Serial.print("low_duration: ");
      Serial.print(low_duration);
      Serial.print(", high_duration: ");
      Serial.print(high_duration);
      Serial.print(", total_duration: ");
      Serial.println(total_duration);
    }
  }

  // takes mean of the samples
  total_duration_averaged = total_duration / SAMPLE_CYCLE;
  frequency = 1000000.0 / total_duration_averaged;

  if (DEBUG_MODE)
  {
    Serial.print("frequency: ");
    Serial.println(frequency);
    Serial.println();
  }

  return frequency;
}

String GetMineralType()
{
  double frequency = GetIrFrequency();
  String mineral_type = "";

  if (frequency > ThiotimolineFrequency - FrequencyTolerance && frequency < ThiotimolineFrequency + FrequencyTolerance)
  {
    mineral_type = "Thiotimoline";
  }
  else if (frequency > NetheriteFrequency - FrequencyTolerance && frequency < NetheriteFrequency + FrequencyTolerance)
  {
    mineral_type = "Netherite";
  }
  else
  {
    mineral_type = "Unknown";
  }

  return mineral_type;
}

float radioSensor () {

  int Htime = 0;             //integer for storing high time
  int Ltime = 0;                //integer for storing low time
  float Ttime = 0;           // integer for storing total time of a cycle
  float frequency = 0;       //storing frequency

  Htime = pulseIn(pinRadio, HIGH);   //read high time
  Ltime = pulseIn(pinRadio, LOW);     //read low time
  Ttime = Htime + Ltime;

  frequency = 1000000/ (Ttime); //getting frequency with Ttime is in Micro seconds

  if ((frequency>230) && (frequency<250)){ //if frequency is within these values it is correct
    frequency = 239;
  }

  else if ((frequency>140)&&(frequency<160)){ //if frequency is within these values it is correct
    frequency = 151;
  }
  
  return frequency;

}

/*int ultrasonicSensor () {

  int Htime2 = 0;             //integer for storing high time
  int Ltime2 = 0;                //integer for storing low time
  float Ttime2 = 0;            // integer for storing total time of a cycle
  float frequency2 = 0;        //storing frequency

  Htime2 = pulseIn(pinUltra, HIGH);   //read high time

  Ltime2 = pulseIn(pinUltra, LOW);     //read low time

  Ttime2 = Htime2 + Ltime2;

  frequency2 = 1000000/ (Ttime2); //getting frequency with Ttime is in Micro seconds

 

  if ((frequency2 > 37500) && (frequency2 < 42500)){ //if frequency is within these values it is correct

    frequency2 = 40000;

  }

  return frequency2;

}*/


//void detect() {
void loop () {

    //server.send(200, F("text/html"), webpageDetect);

    //Values from Sensors
    String magDirect = magneticSensor(); 
    String irType = GetMineralType();
    //int ultraVal = (ultrasonicSensor()/1000);
    float radioVal = radioSensor();
    int ultraVal = 40;

    //Mineral type conditionals
    //Adamantine
    if (magDirect == "North") {
        //server.send(200, F("text/html"), webpageDetectA); 
        Serial.println("Rock: Adamantine");
    }
    //Xirang
    else if (magDirect == "South") {
        //server.send(200, F("text/html"), webpageDetectX);
        Serial.println("Rock: Xirang");
    }
    //Thiotimoline
    else if (irType == "Thiotimoline") { // irVal == "Thiotimoline"
        //server.send(200, F("text/html"), webpageDetectT);
        Serial.println("Rock: Thiotimoline");
    }
    //Netherite
    else if (irType == "Netherite" && ultraVal == 40) { // irVal == "Netherite"
        //server.send(200, F("text/html"), webpageDetectN);
        Serial.println("Rock: Netherite");
    }
    //gaborium
    else if (radioVal == 151) {
        //server.send(200, F("text/html"), webpageDetectG);
        Serial.println("Rock: Gaborium");
    }
    //lathwaite
    else if (radioVal == 239) { //maybe 
        //server.send(200, F("text/html"), webpageDetectL);
        Serial.println("Rock: Lathwaite");
    }
    //No rock
    else {
        //server.send(200, F("text/html"), webpageDetectError);
        Serial.println("Rock: Error");
    }

    //server.send(200, F("text/plain"), F("DETECT"));

}