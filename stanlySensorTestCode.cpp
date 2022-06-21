# pragma region Setups

// ---------------------- Pin Definitions ----------------------
const int pinMagnetic = A0;
const int pinIR = A1;
const int pinRadio = 8;
const int pinUltra = 7;


//  --------------------- Magnetic Sensor ---------------------
int northCount = 0;
int southCount = 0;
int noneCount = 0;
long init_measure = 0;


// --------------------- IR Sensor ---------------------

int SAMPLE_CYCLE = 5;     // number of complete periods we sample and take median of
bool IR_DETAILED_DEBUG_MODE = false;

double FrequencyTolerance = 75.0;
double ThiotimolineFrequency = 353.0;
double NetheriteFrequency = 571.0;


// --------------------- DEBUG Options ---------------------
bool ContinuousDetection = true;
bool PrintExecutionTime = true;

bool DEBUG_Magnetic = false;
bool DEBUG_IR = false;
bool DEBUG_Radio = true;
bool DEBUG_Ultra = false;

bool USE_MAGNETIC = true;
bool USE_IR = true;
bool USE_RADIO = true;
bool USE_ULTRA = false;


// --------------------- Global Variables ---------------------
bool DetectionStarted = false;


void setup () {

  pinMode(pinMagnetic, INPUT);
  pinMode(pinIR, INPUT); 
  pinMode(pinRadio, INPUT);
  //pinMode(pinUltra, INPUT);
  Serial.begin(9600);

  if (DEBUG_Magnetic) {
      Serial.println("Background Magnetic measuring...");
    }

  for(int i = 0; i < 100; i++) {

      init_measure += analogRead(pinMagnetic);

  }

   init_measure /= 100; 

    if (DEBUG_Magnetic) {
        Serial.print("adc value = ");
        Serial.println(init_measure);
    }

}

# pragma endregion


# pragma region Magnetic Sensor

//Magnetic Sensor Code
String magneticSensor() {

    //long init_measure = backgroundMagnetic();

    String magDirect;
    long measure = 0;

    for(int i = 0; i < 10; i++){
      measure += analogRead(pinMagnetic);
    }

    measure /= 10;  
    measure -= init_measure;

    if (DEBUG_Magnetic){
        Serial.print("adc value = ");
        Serial.println(measure);
    }

    //delay(2000);

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
    magDirect = "South";
    southCount += 1;
    northCount = 0;
    noneCount = 0; 
    if (DEBUG_Magnetic)
      Serial.println("South");
    return magDirect;
  }
  else if(measure < -2.5){
    magDirect = "North";
    northCount += 1; 
    southCount = 0;
    noneCount = 0;
    if (DEBUG_Magnetic)
        Serial.println("North");
     return magDirect;
  }
  else{
    magDirect = "None";
    noneCount += 1;
    northCount = 0;
    southCount = 0;
    if (DEBUG_Magnetic)
        Serial.println("None");
       return magDirect;
  }

  if (northCount == 3) {
    if (DEBUG_Magnetic)
      Serial.println("Average of 3 is: North");
      northCount = 0; 
     // magDirect = "North";
      //return magDirect;
  }
  else if (southCount == 3) {
    if (DEBUG_Magnetic)
      Serial.println("Average of 3 is: South");
      southCount = 0; 
      //magDirect = "South";
      //return magDirect;
  }
  else if (noneCount == 3) {
    if (DEBUG_Magnetic)
      Serial.println("Average of 3 is: None");
      noneCount = 0; 
     // magDirect = "None";
      //return magDirect;
  }
  else {
    if (DEBUG_Magnetic)
    Serial.println("Still detecting");
    //magDirect = "Still detecting";
    //return magDirect;
  }

}

# pragma endregion


# pragma region IR Sensor

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

    if (IR_DETAILED_DEBUG_MODE) {
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

  if (DEBUG_IR)
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

# pragma endregion


# pragma region Radio Sensor

float radioSensor () {

  int Htime = 0;             //integer for storing high time
  int Ltime = 0;                //integer for storing low time
  float Ttime = 0;           // integer for storing total time of a cycle
  float frequency = 0;       //storing frequency

  Htime = pulseIn(pinRadio, HIGH);   //read high time
  Ltime = pulseIn(pinRadio, LOW);     //read low time
  Ttime = Htime + Ltime;

  frequency = 1000000/ (Ttime); //getting frequency with Ttime is in Micro seconds
  
  if (DEBUG_Radio) {
    Serial.print("Radio frequency: ");
    Serial.print(frequency);
  }
  

  if ((frequency>230) && (frequency<250)){ //if frequency is within these values it is correct
    frequency = 239;
    if (DEBUG_Radio) {
      Serial.print(" - correct reading detected. Setting frequency to ");
      Serial.println(frequency);
    }
    
  }

  else if ((frequency>140)&&(frequency<160)){ //if frequency is within these values it is correct
    frequency = 151;
    if (DEBUG_Radio) {
      Serial.print(" - correct reading detected. Setting frequency to ");
      Serial.println(frequency);
    }
  }
  else{
    if (DEBUG_Radio) {
      Serial.println();
    }
  }
  
  return frequency;
}

# pragma endregion


# pragma region UltraSonic Sensor

int ultrasonicSensor () {

  int Htime2 = 0;             //integer for storing high time
  int Ltime2 = 0;                //integer for storing low time
  float Ttime2 = 0;            // integer for storing total time of a cycle
  float frequency2 = 0;        //storing frequency

  Htime2 = pulseIn(pinUltra, HIGH);   //read high time

  Ltime2 = pulseIn(pinUltra, LOW);     //read low time

  Ttime2 = Htime2 + Ltime2;

  frequency2 = 1000000/ (Ttime2); //getting frequency with Ttime is in Micro seconds

    if (DEBUG_Ultra) {
        Serial.print("Ultra frequency: ");
        Serial.print(frequency2);
    }

  if ((frequency2 > 37500) && (frequency2 < 42500)){ //if frequency is within these values it is correct
    frequency2 = 40000;

    if (DEBUG_Ultra) {
      Serial.print(" - correct reading detected. Setting frequency to ");
      Serial.println(frequency2);
    }
  }

  return frequency2;

}

# pragma endregion


//void detect() {
void loop () 
{

    float current_time;

    if (!ContinuousDetection)
    {
        // press a to start detecting
        if (Serial.available() > 0) {
            char key = Serial.read();
            if (key == 'a') {
                Serial.println();
                Serial.println("* Detection started *");
                DetectionStarted = true;
            }
        }
    }
    else
        DetectionStarted = true;

    if (!DetectionStarted)
        return;
    
    if (PrintExecutionTime)
    {
      Serial.print("------- Detection Started -------");
      current_time = millis();
    }

    //server.send(200, F("text/html"), webpageDetect);

    String magDirect = "";
    String irType = "";
    int ultraVal = 40;
    float radioVal = 0;

    //Values from Sensors
    if (USE_MAGNETIC){
        magDirect = magneticSensor(); 
        if (PrintExecutionTime)
        {
          Serial.print("Magnetic: ");
          Serial.print(millis() - current_time);
          Serial.print(" ms");
          Serial.println();
          current_time = millis();
        }
    }

    if (USE_IR)
    {
        irType = GetMineralType();
        if (PrintExecutionTime)
        {
          Serial.print("IR: ");
          Serial.print(millis() - current_time);
          Serial.print(" ms");
          Serial.println();
          current_time = millis();
        }
    }

    if (USE_ULTRA)
    {
      ultraVal = (ultrasonicSensor()/1000);
      if (PrintExecutionTime)
      {
        Serial.print("Ultra: ");
        Serial.print(millis() - current_time);
        Serial.print(" ms");
        Serial.println();
        current_time = millis();
      }
    }
    
    if (USE_RADIO)
    {
        radioVal = radioSensor();
        if (PrintExecutionTime)
        {
          Serial.print("Radio: ");
          Serial.print(millis() - current_time);
          Serial.print(" ms");
          Serial.println();
          current_time = millis();
        }

    //Mineral type conditionals
    //Adamantine
    if (magDirect == "North" && radioVal == 151) {
        //server.send(200, F("text/html"), webpageDetectA); 
        Serial.println("Rock: Adamantine");
    }
    //Xirang
    else if (magDirect == "South" && radioVal == 239) {
        //server.send(200, F("text/html"), webpageDetectX);
        Serial.println("Rock: Xirang");
    }
    //Thiotimoline
    else if (irType == "Thiotimoline" && magDirect == "None") { // irVal == "Thiotimoline"
        //server.send(200, F("text/html"), webpageDetectT);
        Serial.println("Rock: Thiotimoline");
    }
    //Netherite
    else if (irType == "Netherite" && ultraVal == 40 && magDirect == "None") { // irVal == "Netherite"
        //server.send(200, F("text/html"), webpageDetectN);
        Serial.println("Rock: Netherite");
    }
    //gaborium
    else if (radioVal == 151 && ultraVal == 40 && magDirect == "None") {
        //server.send(200, F("text/html"), webpageDetectG);
        Serial.println("Rock: Gaborium");
    }
    //lathwaite
    else if (radioVal == 239 && magDirect == "None") { //maybe 
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