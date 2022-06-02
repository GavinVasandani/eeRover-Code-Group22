#define USE_WIFI_NINA         false
#define USE_WIFI101           true
#include <WiFiWebServer.h>

const char ssid[] = "EEERover";
const char pass[] = "exhibition";
const int groupNumber = 3;

WiFiWebServer server(80);

//Establishing the PIN numbers
const int pin_DIRL = 3; //pin to switch ON and keep motor Left on HIGH so if ON then Direction L
const int pin_DIRR = 4; //pin to switch ON and keep motor Right on HIGH so if ON then Direction R
const int pin_PWML = 9; //pin to adjust power going to left motor using analogue write
const int pin_PWMR = 10; //pin to adjust power going to right motor using analogue write
int motorSpeed = 200;
double motorTime = 1000;

//modifying webpage to include R, L, F, STOP
const char webpage[] = "<html><head><style>\
.btn {background-color: inherit;padding: 100px 100px;font-size: 50px;}\
.btn:hover {background: #eee;}\
</style></head>\
<body>\
<button class=\"btn\" onclick=\"moveRight()\">Move Right</button>\
<button class=\"btn\" onclick=\"moveLeft()\">Move Left</button>\
<button class=\"btn\" onclick=\"moveFront()\">Move Front</button>\
<button class=\"btn\" onclick=\"moveStop()\">Stop</button>\
<br>MOVE STATE: <span id=\"state\">OFF</span>\
</body>\
<script>\
var xhttp = new XMLHttpRequest();\
xhttp.onreadystatechange = function() {if (this.readyState == 4 && this.status == 200) {document.getElementById(\"state\").innerHTML = this.responseText;}};\
function moveRight() {xhttp.open(\"GET\", \"/r\"); xhttp.send();}\
function moveLeft() {xhttp.open(\"GET\", \"/l\"); xhttp.send();}\
function moveFront() {xhttp.open(\"GET\", \"/f\"); xhttp.send();}\
function moveStop() {xhttp.open(\"GET\", \"/stop\"); xhttp.send();}\
</script></html>";

//handle function assignments
//Return the web page
void handleRoot()
{
  server.send(200, F("text/html"), webpage);
}

void moveRight() {
    digitalWrite(3, LOW);
    digitalWrite(4, HIGH);

    //we can tweak these motorSpeed values so that they can be more customizable instead of a constant at 200
    analogWrite(PIN_PWML, motorSpeed);    
    analogWrite(PIN_PWMR, motorSpeed);  
    delay(motorTime);                               //Run the motors for the specified time
    analogWrite(PIN_PWML, 0);    
    analogWrite(PIN_PWMR, 0);

    server.send(200, F("text/plain"), F("RIGHT")); //i think this updates URL to be ...ip.../r as the current URL, not sure
}

void moveLeft() {
    digitalWrite(3, HIGH);
    digitalWrite(4, LOW);

    analogWrite(PIN_PWML, motorSpeed);    
    analogWrite(PIN_PWMR, motorSpeed);  
    delay(motorTime);                               //Run the motors for the specified time. This is in milliseconds
    analogWrite(PIN_PWML, 0);    
    analogWrite(PIN_PWMR, 0);

    server.send(200, F("text/plain"), F("LEFT")); //i think this updates URL to be ...ip.../r as the current URL, not sure
}

void moveFront() {
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH);

    analogWrite(PIN_PWML, motorSpeed);    //so this switches on motor power for the intensity given by the motor speed
    analogWrite(PIN_PWMR, motorSpeed);  
    delay(motorTime);                               //Run the motors for the specified time
    analogWrite(PIN_PWML, 0);    
    analogWrite(PIN_PWMR, 0);

    server.send(200, F("text/plain"), F("FRONT")); //i think this updates URL to be ...ip.../r as the current URL, not sure
}

void moveStop() {
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);

    analogWrite(PIN_PWML, motorSpeed);    
    analogWrite(PIN_PWMR, motorSpeed);  
    delay(motorTime);                               //Run the motors for the specified time
    analogWrite(PIN_PWML, 0);    
    analogWrite(PIN_PWMR, 0);

    server.send(200, F("text/plain"), F("STOP")); //i think this updates URL to be ...ip.../r as the current URL, not sure
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

void setup() { //will contain the handles

//configuring the pins to OUTPUT:
    pinMode(pin_DIRL, OUTPUT);
    pinMode(pin_DIRR, OUTPUT);
    pinMode(pin_PWML, OUTPUT);
    pinMode(pin_PWMR, OUTPUT);

    Serial.begin(9600);
    Serial.println("Hello! This is Rover!")

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

    //so essentially we will go in one of these directions, so once we click button we continue in this path, until we hit stop or another button which sends a new URL.

  //so server.on() executed a callback to function ledOFF if /off is present.
  //server.handleClient() runs the necessary callback by doing the HTTP stuff and calling the required function

    server.onNotFound(handleNotFound); //if no URL handle is found then we execute handleNotFound function
  
    server.begin();
  
    Serial.print(F("HTTP server started @ "));
    Serial.println(static_cast<IPAddress>(WiFi.localIP()));
    
}


void loop () { //function looped constantly
    server.handleClient(); //this command checks handle in URL and executes the required callback.
    //The function assigned to a specific handle is given in void setup(); 
}