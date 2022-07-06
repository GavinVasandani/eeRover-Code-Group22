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

const int pin_DIRFR = 2; //faulty
const int pin_DIRFL = 8;
const int pin_DIRBR = 9;
const int pin_DIRBL = 13;

const int pin_PWMFR = 3;
const int pin_PWMFL = 6;
const int pin_PWMBR = 11;
const int pin_PWMBL = 12;

int motorSpeed = 200;
//int motorSpeedR = 220;
double motorTime = 1000;

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
const char webpageTurn[] = "<html><body><p style=\"font-size:10vw; padding: 10px; border: 5px solid red;\">Current Instruction is: Turn</p> <style>    h1 {text-align: center;} p {text-align: center;} div {text-align: center;} </style> </body> </html>";

//this is a custom webpage just to show current instruction is front
const char webpageFrontRight[] = "<html><body><p style=\"font-size:10vw; padding: 10px; border: 5px solid red;\">Current Instruction is: Front-Right</p> <style>    h1 {text-align: center;} p {text-align: center;} div {text-align: center;} </style> </body> </html>";

//this is a custom webpage just to show current instruction is stop
const char webpageFrontLeft[] = "<html><body><p style=\"font-size:10vw; padding: 10px; border: 5px solid red;\">Current Instruction is: Front-Left</p> <style>    h1 {text-align: center;} p {text-align: center;} div {text-align: center;} </style> </body> </html>";

//this is a custom webpage just to show current instruction is Back
const char webpageBackLeft[] = "<html><body><p style=\"font-size:10vw; padding: 10px; border: 5px solid red;\">Current Instruction is: Back-Left</p> <style>    h1 {text-align: center;} p {text-align: center;} div {text-align: center;} </style> </body> </html>";

//this is a custom webpage just to show current instruction is Turn
const char webpageBackRight[] = "<html><body><p style=\"font-size:10vw; padding: 10px; border: 5px solid red;\">Current Instruction is: Back-Right</p> <style>    h1 {text-align: center;} p {text-align: center;} div {text-align: center;} </style> </body> </html>";

//handle function assignments
//Return the web page
void handleRoot()
{
  server.send(200, F("text/html"), webpageHome);
}

void moveRight() { //DONE

    server.send(200, F("text/html"), webpageRight);
    //digitalWrite(3, LOW);
    //digitalWrite(4, HIGH);

    digitalWrite(pin_DIRFR, HIGH);  //RDIR
    digitalWrite(pin_DIRFL, LOW);  //LDIR
    digitalWrite(pin_DIRBR, LOW);  //RDIR
    digitalWrite(pin_DIRBL, HIGH);  //LDIR 

    analogWrite(pin_PWMFR, motorSpeed);  //REN
    analogWrite(pin_PWMFL, motorSpeed);  //LEN
    analogWrite(pin_PWMBR, motorSpeed);  //REN
    analogWrite(pin_PWMBL, motorSpeed);  //LEN



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
    analogWrite(pin_PWMFL, motorSpeed);  //LEN
    analogWrite(pin_PWMBR, motorSpeed);  //REN
    analogWrite(pin_PWMBL, motorSpeed);  //LEN

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
    analogWrite(pin_PWMFL, motorSpeed);  //LEN
    analogWrite(pin_PWMBR, motorSpeed);  //REN
    analogWrite(pin_PWMBL, motorSpeed);  //LEN


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
    analogWrite(pin_PWMFL, motorSpeed);  //LEN
    analogWrite(pin_PWMBR, motorSpeed);  //REN
    analogWrite(pin_PWMBL, motorSpeed);  //LEN



    server.send(200, F("text/plain"), F("BACK")); //i think this updates URL to be ...ip.../r as the current URL, not sure
}

void moveTurn() { //DONE

    server.send(200, F("text/html"), webpageTurn);
    //digitalWrite(3, LOW);
    //digitalWrite(4, LOW);

    digitalWrite(pin_DIRFR, HIGH);  //RDIR
    digitalWrite(pin_DIRFL, LOW);  //LDIR
    digitalWrite(pin_DIRBR, HIGH);  //RDIR
    digitalWrite(pin_DIRBL, LOW);  //LDIR 

    analogWrite(pin_PWMFR, motorSpeed);  //REN
    analogWrite(pin_PWMFL, motorSpeed);  //LEN
    analogWrite(pin_PWMBR, motorSpeed);  //REN
    analogWrite(pin_PWMBL, motorSpeed);  //LEN

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
    analogWrite(pin_PWMFL, motorSpeed);  //LEN
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
    analogWrite(pin_PWMBL, motorSpeed);  //LEN

    server.send(200, F("text/plain"), F("FRONTLEFT")); //i think this updates URL to be ...ip.../r as the current URL, not sure
}

void moveBackRight() {  //DONE

    server.send(200, F("text/html"), webpageBackRight);
    //digitalWrite(3, LOW);
    //digitalWrite(4, LOW);
    
    digitalWrite(pin_DIRFR, LOW);  //RDIR
    digitalWrite(pin_DIRFL, HIGH);  //LDIR
    digitalWrite(pin_DIRBR, HIGH);  //RDIR
    digitalWrite(pin_DIRBL, LOW);  //LDIR 

    analogWrite(pin_PWMFR, 0);  //REN
    analogWrite(pin_PWMFL, motorSpeed);  //LEN
    analogWrite(pin_PWMBR, motorSpeed);  //REN
    analogWrite(pin_PWMBL, 0);  //LEN


    server.send(200, F("text/plain"), F("BACKRIGHT")); //i think this updates URL to be ...ip.../r as the current URL, not sure
}

void moveBackLeft() { //DONE

    server.send(200, F("text/html"), webpageBackLeft);
    //digitalWrite(3, LOW);
    //digitalWrite(4, LOW);

    digitalWrite(pin_DIRFR, HIGH);  //RDIR
    digitalWrite(pin_DIRFL, LOW);  //LDIR
    digitalWrite(pin_DIRBR, LOW);  //RDIR
    digitalWrite(pin_DIRBL, HIGH);  //LDIR 

    analogWrite(pin_PWMFR, motorSpeed);  //REN
    analogWrite(pin_PWMFL, 0);  //LEN
    analogWrite(pin_PWMBR, 0);  //REN
    analogWrite(pin_PWMBL, motorSpeed);  //LEN


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

void setup() { //will contain the handles

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
    server.on(F("/turn"), moveTurn);

    //handle situation if input is FRONTRIGHT (FR)
    server.on(F("/fr"), moveFrontRight);

    //handle situation if input is FRONTLEFT (FL)
    server.on(F("/fl"), moveFrontLeft);

    //handle situation if input is BACKRIGHT (BR)
    server.on(F("/br"), moveBackRight);

    //handle situation if input is BACKLEFT (BL)
    server.on(F("/bl"), moveBackLeft);

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