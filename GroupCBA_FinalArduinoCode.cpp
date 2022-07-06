//Compiling and uploading correctly to adafruit
//Needs to be tested fully.

#define USE_WIFI_NINA         false
#define USE_WIFI101           true
#include <WiFiWebServer.h>

const char ssid[] = "EEERover";
const char pass[] = "exhibition";
const int groupNumber = 3;

WiFiWebServer server(80);
//he
//Establishing the PIN numbers

//Front Right, Back Left only goes reverse

// --------------------- Motor Variables ---------------------

const int pin_DIRFR = 2; //faulty
const int pin_DIRFL = 8;
const int pin_DIRBR = 9;
const int pin_DIRBL = 13;

const int pin_PWMFR = 3;
const int pin_PWMFL = 6;
const int pin_PWMBR = 11;
const int pin_PWMBL = 12;

int motorSpeed = 200;
int motorSpeedFaulty = 243;
double motorTime = 1000;

// ---------------------- Pin Definitions ----------------------
const int pinMagnetic = A0;
const int pinIR = A1;
const int pinRadio = A2; //connect to pin 4 not 8
const int pinUltra = 5;


//  --------------------- Magnetic Sensor ---------------------
int northCount = 0;
int southCount = 0;
int noneCount = 0;
long init_measure = 0;


// --------------------- IR Sensor ---------------------

int SAMPLE_CYCLE = 5;     // number of complete periods we sample and take median of
bool IR_DETAILED_DEBUG_MODE = true;

double FrequencyTolerance = 75.0;
double ThiotimolineFrequency = 353.0;
double NetheriteFrequency = 571.0;


// --------------------- DEBUG Options ---------------------
bool StandaloneEnvironment = false;
bool ContinuousDetection = true;
float ContinuousDetectionInterval = 500;
bool PrintExecutionTime = false;

bool DEBUG_Magnetic = true;
bool DEBUG_IR = true;
bool DEBUG_Radio = true;
bool DEBUG_Ultra = false;

bool USE_MAGNETIC = true;
bool USE_IR = true;
bool USE_RADIO = true;
bool USE_ULTRA = false;

// --------------------- Global Variables ---------------------
bool DetectionStarted = false;

// --------------------- Declaring HTML Webpages ---------------------

//modifying webpage to include R, L, F, STOP
const char webpageHome[] = "<html><body><p style=\"font-size:10vw; padding: 10px; border: 5px solid red;\">Current Instruction is: Welcome to Rover!</p> <style>    h1 {text-align: center;} p {text-align: center;} div {text-align: center;} </style> </body> </html>";

const char webpage[] = "<html><head><style>\
.btn {background-color: inherit;padding: 100px 100px;font-size: 50px;}\
.btn:hover {background: #eee;}\
</style></head>\
<body>\
<button class=\"btn\" onclick=\"moveRight()\">Move Right</button>\
<button class=\"btn\" onclick=\"moveLeft()\">Move Left</button>\
<button class=\"btn\" onclick=\"moveFront()\">Move Front</button>\
<button class=\"btn\" onclick=\"moveStop()\">Stop</button>\
<button class=\"btn\" onclick=\"moveBack()\">Move Front</button>\
<button class=\"btn\" onclick=\"moveTurn()\">Stop</button>\
<br>MOVE STATE: <span id=\"state\">OFF</span>\
</body>\
<script>\
var xhttp = new XMLHttpRequest();\
xhttp.onreadystatechange = function() {if (this.readyState == 4 && this.status == 200) {document.getElementById(\"state\").innerHTML = this.responseText;}};\
function moveRight() {xhttp.open(\"GET\", \"/r\"); xhttp.send();}\
function moveLeft() {xhttp.open(\"GET\", \"/l\"); xhttp.send();}\
function moveFront() {xhttp.open(\"GET\", \"/f\"); xhttp.send();}\
function moveStop() {xhttp.open(\"GET\", \"/stop\"); xhttp.send();}\
</script></html>"; //NOT CALIBRATED FOR THIS
//MAYBE NOT NECESSARY

//this is a custom webpage just to show current instruction is right
const char webpageRight[] = "<html><body><p style=\"font-size:10vw; padding: 10px; border: 5px solid red;\">Current Instruction is: Right</p> <style>    h1 {text-align: center;} p {text-align: center;} div {text-align: center;} </style> </body> </html>";

//this is a custom webpage just to show current instruction is left
const char webpageLeft[] = "<html><body><p style=\"font-size:10vw; padding: 10px; border: 5px solid red;\">Current Instruction is: Left</p> <style>    h1 {text-align: center;} p {text-align: center;} div {text-align: center;} </style> </body> </html>";

//this is a custom webpage just to show current instruction is front
const char webpageFront[] = "<html><body><p style=\"font-size:10vw; padding: 10px; border: 5px solid red;\">Current Instruction is: Front</p> <style>    h1 {text-align: center;} p {text-align: center;} div {text-align: center;} </style> </body> </html>";

//this is a custom webpage just to show current instruction is stop
const char webpageStop[] = "<html><body><p style=\"font-size:10vw; padding: 10px; border: 5px solid red;\">Current Instruction is: Stop</p> <style>    h1 {text-align: center;} p {text-align: center;} div {text-align: center;} </style> </body> </html>";

//this is a custom webpage just to show current instruction is Back
const char webpageBack[] = "<html><body><p style=\"font-size:10vw; padding: 10px; border: 5px solid red;\">Current Instruction is: Back</p> <style>    h1 {text-align: center;} p {text-align: center;} div {text-align: center;} </style> </body> </html>";

//this is a custom webpage just to show current instruction is Turn
const char webpageTurnR[] = "<html><body><p style=\"font-size:10vw; padding: 10px; border: 5px solid red;\">Current Instruction is: Turn Right</p> <style>    h1 {text-align: center;} p {text-align: center;} div {text-align: center;} </style> </body> </html>";

const char webpageTurnL[] = "<html><body><p style=\"font-size:10vw; padding: 10px; border: 5px solid red;\">Current Instruction is: Turn Left</p> <style>    h1 {text-align: center;} p {text-align: center;} div {text-align: center;} </style> </body> </html>";

//this is a custom webpage just to show current instruction is front
const char webpageFrontRight[] = "<html><body><p style=\"font-size:10vw; padding: 10px; border: 5px solid red;\">Current Instruction is: Front-Right</p> <style>    h1 {text-align: center;} p {text-align: center;} div {text-align: center;} </style> </body> </html>";

//this is a custom webpage just to show current instruction is stop
const char webpageFrontLeft[] = "<html><body><p style=\"font-size:10vw; padding: 10px; border: 5px solid red;\">Current Instruction is: Front-Left</p> <style>    h1 {text-align: center;} p {text-align: center;} div {text-align: center;} </style> </body> </html>";

//this is a custom webpage just to show current instruction is Back
const char webpageBackLeft[] = "<html><body><p style=\"font-size:10vw; padding: 10px; border: 5px solid red;\">Current Instruction is: Back-Left</p> <style>    h1 {text-align: center;} p {text-align: center;} div {text-align: center;} </style> </body> </html>";

//this is a custom webpage just to show current instruction is Turn
const char webpageBackRight[] = "<html><body><p style=\"font-size:10vw; padding: 10px; border: 5px solid red;\">Current Instruction is: Back-Right</p> <style>    h1 {text-align: center;} p {text-align: center;} div {text-align: center;} </style> </body> </html>";

const char webpageDetect[] = "<html><body><p style=\"font-size:10vw; padding: 10px; border: 5px solid red;\">Current Instruction is: Detecting...</p> <style>    h1 {text-align: center;} p {text-align: center;} div {text-align: center;} </style> </body> </html>";

const char webpageDetectG[] = "<html><body><p style=\"font-size:10vw; padding: 10px; border: 5px solid red;\">Rock is Gaborium</p> <style>    h1 {text-align: center;} p {text-align: center;} div {text-align: center;} </style> </body> </html>";

const char webpageDetectL[] = "<html><body><p style=\"font-size:10vw; padding: 10px; border: 5px solid red;\">Rock is Lathwaite</p> <style>    h1 {text-align: center;} p {text-align: center;} div {text-align: center;} </style> </body> </html>";

const char webpageDetectA[] = "<html><body><p style=\"font-size:10vw; padding: 10px; border: 5px solid red;\">Rock is Adamantine</p> <style>    h1 {text-align: center;} p {text-align: center;} div {text-align: center;} </style> </body> </html>";

const char webpageDetectX[] = "<html><body><p style=\"font-size:10vw; padding: 10px; border: 5px solid red;\">Rock is Xirang</p> <style>    h1 {text-align: center;} p {text-align: center;} div {text-align: center;} </style> </body> </html>";

const char webpageDetectT[] = "<html><body><p style=\"font-size:10vw; padding: 10px; border: 5px solid red;\">Rock is Thiotimoline</p> <style>    h1 {text-align: center;} p {text-align: center;} div {text-align: center;} </style> </body> </html>";

const char webpageDetectN[] = "<html><body><p style=\"font-size:10vw; padding: 10px; border: 5px solid red;\">Rock is Netherite</p> <style>    h1 {text-align: center;} p {text-align: center;} div {text-align: center;} </style> </body> </html>";

const char webpageDetectError[] = "<html><body><p style=\"font-size:10vw; padding: 10px; border: 5px solid red;\">Rock Undetectable</p> <style>    h1 {text-align: center;} p {text-align: center;} div {text-align: center;} </style> </body> </html>";

//handle function assignments
//Return the web page
void handleRoot()
{
  server.send(200, F("text/html"), webpageHome);
}

IPAddress ip;

void moveRight() { //DONE

    server.send(200, F("text/html"), webpageRight);
    //digitalWrite(3, LOW);
    //digitalWrite(4, HIGH);

    digitalWrite(pin_DIRFR, HIGH);  //RDIR
    digitalWrite(pin_DIRFL, LOW);  //LDIR
    digitalWrite(pin_DIRBR, LOW);  //RDIR
    digitalWrite(pin_DIRBL, HIGH);  //LDIR

    analogWrite(pin_PWMFR, motorSpeed);  //REN
    analogWrite(pin_PWMFL, motorSpeedFaulty);  //LEN
    analogWrite(pin_PWMBR, motorSpeed);  //REN
    analogWrite(pin_PWMBL, motorSpeedFaulty);  //LEN

    server.send(200, F("text/plain"), F("RIGHT")); //i think this updates URL to be ...ip.../r as the current URL, not sure
}

void moveLeft() { //DONE

    server.send(200, F("text/html"), webpageLeft);
    //digitalWrite(3, HIGH);
    //digitalWrite(4, LOW); 

    digitalWrite(pin_DIRFR, LOW);  //RDIR
    digitalWrite(pin_DIRFL, HIGH);  //LDIR
    digitalWrite(pin_DIRBR, HIGH);  //RDIR
    digitalWrite(pin_DIRBL, LOW);  //LDIR 

    analogWrite(pin_PWMFR, motorSpeed);  //REN
    analogWrite(pin_PWMFL, motorSpeedFaulty);  //LEN
    analogWrite(pin_PWMBR, motorSpeed);  //REN
    analogWrite(pin_PWMBL, motorSpeedFaulty);  //LEN

    server.send(200, F("text/plain"), F("LEFT")); //i think this updates URL to be ...ip.../r as the current URL, not sure
}

void moveFront() { //DONE

    server.send(200, F("text/html"), webpageFront);

    //Serial.print(F("Working!"));

    digitalWrite(pin_DIRFR, LOW);  //RDIR
    digitalWrite(pin_DIRFL, LOW);  //LDIR
    digitalWrite(pin_DIRBR, LOW);  //RDIR
    digitalWrite(pin_DIRBL, LOW);  //LDIR 
    
    analogWrite(pin_PWMFR, motorSpeed);  //REN
    analogWrite(pin_PWMFL, motorSpeedFaulty);  //LEN
    analogWrite(pin_PWMBR, motorSpeed);  //REN
    analogWrite(pin_PWMBL, motorSpeedFaulty);  //LEN


    server.send(200, F("text/plain"), F("FRONT")); //i think this updates URL to be ...ip.../r as the current URL, not sure
}

void moveStop() { //DONE

    server.send(200, F("text/html"), webpageStop);
    //digitalWrite(3, LOW);
    //digitalWrite(4, LOW);

    digitalWrite(pin_DIRFR, LOW);  //RDIR
    digitalWrite(pin_DIRFL, LOW);  //LDIR
    digitalWrite(pin_DIRBR, LOW);  //RDIR
    digitalWrite(pin_DIRBL, LOW);  //LDIR 

    analogWrite(pin_PWMFR, 0);  //REN
    analogWrite(pin_PWMFL, 0);  //LEN
    analogWrite(pin_PWMBR, 0);  //REN
    analogWrite(pin_PWMBL, 0);  //LEN

    server.send(200, F("text/plain"), F("STOP")); //i think this updates URL to be ...ip.../r as the current URL, not sure
}

void moveBack() { //DONE

    server.send(200, F("text/html"), webpageBack);

    digitalWrite(pin_DIRFR, HIGH);  //RDIR
    digitalWrite(pin_DIRFL, HIGH);  //LDIR
    digitalWrite(pin_DIRBR, HIGH);  //RDIR
    digitalWrite(pin_DIRBL, HIGH);  //LDIR 

    analogWrite(pin_PWMFR, motorSpeed);  //REN
    analogWrite(pin_PWMFL, motorSpeedFaulty);  //LEN
    analogWrite(pin_PWMBR, motorSpeed);  //REN
    analogWrite(pin_PWMBL, motorSpeedFaulty);  //LEN



    server.send(200, F("text/plain"), F("BACK")); //i think this updates URL to be ...ip.../r as the current URL, not sure
}

void moveTurnR() { //DONE

    server.send(200, F("text/html"), webpageTurnR);
    //digitalWrite(3, LOW);
    //digitalWrite(4, LOW);

    digitalWrite(pin_DIRFR, HIGH);  //RDIR
    digitalWrite(pin_DIRFL, LOW);  //LDIR
    digitalWrite(pin_DIRBR, HIGH);  //RDIR
    digitalWrite(pin_DIRBL, LOW);  //LDIR 

    analogWrite(pin_PWMFR, motorSpeed);  //REN
    analogWrite(pin_PWMFL, motorSpeedFaulty);  //LEN
    analogWrite(pin_PWMBR, motorSpeed);  //REN
    analogWrite(pin_PWMBL, motorSpeedFaulty);  //LEN

    server.send(200, F("text/plain"), F("TURN")); //i think this updates URL to be ...ip.../r as the current URL, not sure
}

void moveTurnL() { //DONE

    server.send(200, F("text/html"), webpageTurnL);
    //digitalWrite(3, LOW);
    //digitalWrite(4, LOW);

    digitalWrite(pin_DIRFR, LOW);  //RDIR
    digitalWrite(pin_DIRFL, HIGH);  //LDIR
    digitalWrite(pin_DIRBR, LOW);  //RDIR
    digitalWrite(pin_DIRBL, HIGH);  //LDIR 

    analogWrite(pin_PWMFR, motorSpeed);  //REN
    analogWrite(pin_PWMFL, motorSpeedFaulty);  //LEN
    analogWrite(pin_PWMBR, motorSpeed);  //REN
    analogWrite(pin_PWMBL, motorSpeedFaulty);  //LEN

    server.send(200, F("text/plain"), F("TURN")); //i think this updates URL to be ...ip.../r as the current URL, not sure
}

void moveFrontRight() { //DONE

    server.send(200, F("text/html"), webpageFrontRight);
    //digitalWrite(3, LOW);
    //digitalWrite(4, LOW);

    digitalWrite(pin_DIRFR, HIGH);  //RDIR
    digitalWrite(pin_DIRFL, LOW);  //LDIR
    digitalWrite(pin_DIRBR, LOW);  //RDIR
    digitalWrite(pin_DIRBL, HIGH);  //LDIR 

    analogWrite(pin_PWMFR, 0);  //REN
    analogWrite(pin_PWMFL, motorSpeedFaulty);  //LEN
    analogWrite(pin_PWMBR, motorSpeed);  //REN
    analogWrite(pin_PWMBL, 0);  //LEN


    server.send(200, F("text/plain"), F("FRONTRIGHT")); //i think this updates URL to be ...ip.../r as the current URL, not sure
}

void moveFrontLeft() { //DONE

    server.send(200, F("text/html"), webpageFrontLeft);
    //digitalWrite(3, LOW);
    //digitalWrite(4, LOW);

    digitalWrite(pin_DIRFR, LOW);  //RDIR
    digitalWrite(pin_DIRFL, HIGH);  //LDIR
    digitalWrite(pin_DIRBR, HIGH);  //RDIR
    digitalWrite(pin_DIRBL, LOW);  //LDIR 

    analogWrite(pin_PWMFR, motorSpeed);  //REN
    analogWrite(pin_PWMFL, 0);  //LEN
    analogWrite(pin_PWMBR, 0);  //REN
    analogWrite(pin_PWMBL, motorSpeedFaulty);  //LEN

    server.send(200, F("text/plain"), F("FRONTLEFT")); //i think this updates URL to be ...ip.../r as the current URL, not sure
}

void moveBackRight() {  //DONE

    server.send(200, F("text/html"), webpageBackRight);
    //digitalWrite(3, LOW);
    //digitalWrite(4, LOW);
    
    digitalWrite(pin_DIRFR, HIGH);  //RDIR
    digitalWrite(pin_DIRFL, LOW);  //LDIR
    digitalWrite(pin_DIRBR, LOW);  //RDIR
    digitalWrite(pin_DIRBL, HIGH);  //LDIR 

    analogWrite(pin_PWMFR, motorSpeed);  //REN
    analogWrite(pin_PWMFL, 0);  //LEN
    analogWrite(pin_PWMBR, 0);  //REN
    analogWrite(pin_PWMBL, motorSpeedFaulty);  //LEN


    server.send(200, F("text/plain"), F("BACKRIGHT")); //i think this updates URL to be ...ip.../r as the current URL, not sure
}

void moveBackLeft() { //DONE

    server.send(200, F("text/html"), webpageBackLeft);
    //digitalWrite(3, LOW);
    //digitalWrite(4, LOW);

    digitalWrite(pin_DIRFR, LOW);  //RDIR
    digitalWrite(pin_DIRFL, HIGH);  //LDIR
    digitalWrite(pin_DIRBR, HIGH);  //RDIR
    digitalWrite(pin_DIRBL, LOW);  //LDIR 

    analogWrite(pin_PWMFR, 0);  //REN
    analogWrite(pin_PWMFL, motorSpeedFaulty);  //LEN
    analogWrite(pin_PWMBR, motorSpeed);  //REN
    analogWrite(pin_PWMBL, 0);  //LEN

    server.send(200, F("text/plain"), F("BACKLEFT")); //i think this updates URL to be ...ip.../r as the current URL, not sure
}

//Generate a 404 response with details of the failed request
void handleNotFound()
{
  String message = F("File Not Found\n\n"); 
  message += F("URI: ");
  message += server.uri();
  message += F("\nMethod: ");
  message += (server.method() == HTTP_GET) ? F("GET") : F("POST");
  message += F("\nArguments: ");
  message += server.args();
  message += F("\n");
  for (uint8_t i = 0; i < server.args(); i++)
  {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, F("text/plain"), message);
}

//Magnetic Sensor Code
String magneticSensor() {

    //long init_measure = backgroundMagnetic();

    String magDirect;
    long measure = 0;

    for(int i = 0; i < 10; i++){
      measure += analogRead(pinMagnetic);
      measure -= init_measure;
    }

    measure /= 10;  

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

  if (measure > 18){
    magDirect = "South";
    southCount += 1;
    northCount = 0;
    noneCount = 0; 
    if (DEBUG_Magnetic)
      Serial.println("South");
    return magDirect;
  }
  else if(measure < -18){
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

  float timeout_duration = 0.1;     // in seconds
  bool detection_failed = false;

  // starts sampling the values into samples
  for (int i = 0; i < SAMPLE_CYCLE; i++) {
    long low_duration = pulseIn(pinIR, LOW, timeout_duration * 1000000);
    long high_duration = pulseIn(pinIR, HIGH, timeout_duration * 1000000);
    total_duration += high_duration + low_duration;

    if (total_duration == 0) {
      detection_failed = true;
    }

    if (IR_DETAILED_DEBUG_MODE && !detection_failed) {
      Serial.print("IR - low_duration: ");
      Serial.print(low_duration);
      Serial.print(", high_duration: ");
      Serial.print(high_duration);
      Serial.print(", total_duration: ");
      Serial.println(total_duration);
    }
  }

  if (detection_failed){
    if (DEBUG_IR)
      Serial.println("IR - Detection failed.");
      return -1;
  }

  // takes mean of the samples
  total_duration_averaged = total_duration / SAMPLE_CYCLE;
  frequency = 1000000.0 / total_duration_averaged;

  if (DEBUG_IR)
  {
    Serial.print("IR - frequency: ");
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

  // added by stanly: modified to shorted the failed timing
  float timeout_duration = 0.1;     // in seconds

  Htime = pulseIn(pinRadio, HIGH, timeout_duration * 1000000);   //read high time
  Ltime = pulseIn(pinRadio, LOW, timeout_duration * 1000000);     //read low time
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

/*int ultrasonicSensor () {

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

} */

# pragma endregion


String DetectMineral()
{
    float current_time;

    
    
    if (PrintExecutionTime)
    {
      Serial.println();
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

    /*if (USE_ULTRA)
    {
      //ultraVal = (ultrasonicSensor()/1000);
      if (PrintExecutionTime)
      {
        Serial.print("Ultra: ");
        Serial.print(millis() - current_time);
        Serial.print(" ms");
        Serial.println();
        current_time = millis();
      }
    }*/
    
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
    }

    if (PrintExecutionTime)
    {
      Serial.println();
    }

    //Mineral type conditionals
    //Adamantine
    if (magDirect == "North" && radioVal == 151) {
        server.send(200, F("text/html"), webpageDetectA); 
        return "Adamantine";
    }
    //Xirang
    else if (magDirect == "South" && radioVal == 239) {
        server.send(200, F("text/html"), webpageDetectX);
        return "Xirang";
    }
    //Thiotimoline
    else if (irType == "Thiotimoline") { // irVal == "Thiotimoline"
        server.send(200, F("text/html"), webpageDetectT);
        return "Thiotimoline";
    }
    //Netherite
    else if (irType == "Netherite" && ultraVal == 40) { // irVal == "Netherite"
        server.send(200, F("text/html"), webpageDetectN);
        return "Netherite";
    }
    //gaborium
    else if (radioVal == 151 && ultraVal == 40) {
        server.send(200, F("text/html"), webpageDetectG);
        return "Gaborium";
    }
    //lathwaite
    else if (radioVal == 239) { //maybe 
        server.send(200, F("text/html"), webpageDetectL);
        return "Lathwaite";
    }
    //No rock
    else {
        server.send(200, F("text/html"), webpageDetectError);
        return "";
    }

    //server.send(200, F("text/plain"), F("DETECT"));
}

void detect() //detect function which we put in handle
{
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

  String RockType = DetectMineral();
  if (RockType != "")
  {
    Serial.print("Rock: ");
    Serial.println(RockType);
  }
  else
  {
    Serial.println("No rock detected.");
  }

  if (ContinuousDetection)
    delay(ContinuousDetectionInterval);

}



void setup() { //will contain the handles

  if (StandaloneEnvironment)
  {
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

  else
  {

    //configuring the pins to OUTPUT:
    Serial.begin(9600);
    pinMode(pin_DIRFR, OUTPUT);
    pinMode(pin_DIRFL, OUTPUT);
    pinMode(pin_DIRBR, OUTPUT);
    pinMode(pin_DIRBL, OUTPUT);

    pinMode(pin_PWMFR, OUTPUT);
    pinMode(pin_PWMFL, OUTPUT);
    pinMode(pin_PWMBR, OUTPUT);
    pinMode(pin_PWMBL, OUTPUT);  

    pinMode(pinMagnetic, INPUT);
    pinMode(pinIR, INPUT); 
    pinMode(pinRadio, INPUT);
    pinMode(pinUltra, INPUT);
    
    Serial.println("Hello! This is Rover!");

    //Wait 10s for the serial connection before proceeding
    //This ensures you can see messages from startup() on the monitor
    //so this is just a time lag
    //Remove this for faster startup when the USB host isn't attached
      while (!Serial && millis() < 10000);  

    //arduino has a built in timer and continues to run as the program works. So millis() outputs time in milliseconds
    //so if serial.begin() is not 0 and program time is less than 10000ms then we start webserver
    //so only between 9600-10000 do we do the webserver connections

    //output the following statement to serial monitor
      Serial.println(F("\nStarting Web Server"));

    //Check WiFi shield is present
      if (WiFi.status() == WL_NO_SHIELD)
      {
          Serial.println(F("WiFi shield not present"));
          while (true); //so we loop in this state as long as this is true
      }

    //Configure the static IP address if group number is set
      if (groupNumber) //so if groupnumber != 0 aka is anything other than 0 we execute
          WiFi.config(IPAddress(192,168,0,groupNumber+1));
          //WiFi.config(IPAddress(146,169,132,249));

    // attempt to connect to WiFi network
      Serial.print(F("Connecting to WPA SSID: "));
      Serial.println(ssid);
    //the above 2 statements are all outputted to serial monitor

      while (WiFi.begin(ssid, pass) != WL_CONNECTED) //wifi.begin() is used to connect to wifi network ssid using the password pass.
    //so while loop repeats until wifi is connected which happens when Wifi.begin() = WL_CONNECTED
      {
          delay(500); //repeat reconnection function every 500ms
          Serial.print('.'); //output . if doesn't connect
      }

      Serial.println("Rover is connected to: ");
      Serial.println(ssid);
      //Code above is used to connect wifi shield to EERover network automatically at startup

      //Adding the handle callbacks based on URL:

      //Register the callbacks to respond to HTTP requests
    //so this is similar to the URL thingy where at the end of the URL we check if its /on,/off do determine what function to execute
      server.on(F("/"), handleRoot); //so server is on which is found by a / at the URL so we execute handleRoot which is a func that sends the webpage
      
      //handle situation if input is right (r)
      server.on(F("/r"), moveRight);

      //handle situation if input is left (l)
      server.on(F("/l"), moveLeft);
    //so we keep the website on the server by changing URL to /on and executing function ledON
      
      //handle situation if input is front (f)
      server.on(F("/f"), moveFront);

      //handle situation if input is immediate STOP (stop)
      server.on(F("/stop"), moveStop);

      //handle situation if input is BACK (back)
      server.on(F("/back"), moveBack);

      //handle situation if input is TURN (turn)
      server.on(F("/turnr"), moveTurnR);

      //handle situation if input is TURN (turn)
      server.on(F("/turnl"), moveTurnL);

      //handle situation if input is FRONTRIGHT (FR)
      server.on(F("/fr"), moveFrontRight);

      //handle situation if input is FRONTLEFT (FL)
      server.on(F("/fl"), moveFrontLeft);

      //handle situation if input is BACKRIGHT (BR)
      server.on(F("/br"), moveBackRight);

      //handle situation if input is BACKLEFT (BL)
      server.on(F("/bl"), moveBackLeft);

      server.on(F("/detect"), detect);
      
      //so essentially we will go in one of these directions, so once we click button we continue in this path, until we hit stop or another button which sends a new URL.

    //so server.on() executed a callback to function ledOFF if /off is present.
    //server.handleClient() runs the necessary callback by doing the HTTP stuff and calling the required function

      server.onNotFound(handleNotFound); //if no URL handle is found then we execute handleNotFound function
    
      server.begin();

    
      Serial.print(F("HTTP server started @ "));
      Serial.println(static_cast<IPAddress>(WiFi.localIP()));
  //}

      if (DEBUG_Magnetic) {
        Serial.println("Background Magnetic measuring...");
      }

    for(int i = 0; i < 20; i++) {

        init_measure += analogRead(pinMagnetic);
        delay(100);
    }

    init_measure /= 20; 

    Serial.print("calibratioin complete: ");
    Serial.println(init_measure);

    /*
      if (DEBUG_Magnetic) {
          Serial.print("adc value = ");
          Serial.println(init_measure);
      }
      */
  }

}



void loop () {
  if (!StandaloneEnvironment)
  {
    //ip = WiFi.localIP();
    //Serial.println(ip);
    server.handleClient(); //this command checks handle in URL and executes the required callback.
    //The function assigned to a specific handle is given in void setup(); 
  }
  else
  {
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

  String RockType = DetectMineral();
  if (RockType != "")
  {
    Serial.print("Rock: ");
    Serial.println(RockType);
  }
  else
  {
    Serial.println("No rock detected.");
  }

  if (ContinuousDetection)
    delay(ContinuousDetectionInterval);
  }

}
/*

void loop () 
{
  
}
*/
