/**********************************************
  EEERover Starter Example
  
  Based on AdvancedWebServer.ino - Simple Arduino web server sample for SAMD21 running WiFiNINA shield
  For any WiFi shields, such as WiFiNINA W101, W102, W13x, or custom, such as ESP8266/ESP32-AT, Ethernet, etc
  
  WiFiWebServer is a library for the ESP32-based WiFi shields to run WebServer
  Forked and modified from ESP8266 https://github.com/esp8266/Arduino/releases
  Forked and modified from Arduino WiFiNINA library https://www.arduino.cc/en/Reference/WiFiNINA
  Built by Khoi Hoang https://github.com/khoih-prog/WiFiWebServer
  Licensed under MIT license
  
  Copyright (c) 2015, Majenko Technologies
  All rights reserved.
  
  Redistribution and use in source and binary forms, with or without modification,
  are permitted provided that the following conditions are met:
  
  Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.
  
  Redistributions in binary form must reproduce the above copyright notice, this
  list of conditions and the following disclaimer in the documentation and/or
  other materials provided with the distribution.
  
  Neither the name of Majenko Technologies nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.
  
  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
  ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *********************************************/
#define USE_WIFI_NINA         false
#define USE_WIFI101           true
#include <WiFiWebServer.h>
//#include "webpagecode"

const char ssid[] = "EEERover";
const char pass[] = "exhibition";
const int groupNumber = 3; // Set your group number to make the IP address constant - only do this on the EEERover network

//Webpage to return when root is requested, defined in separate file
const char webpage[] = "<html><head><style>\
.btn {background-color: inherit;padding: 100px 100px;font-size: 50px;}\
.btn:hover {background: #eee;}\
</style></head>\
<body>\
<button class=\"btn\" onclick=\"ledOn()\">LED On</button>\
<button class=\"btn\" onclick=\"ledOff()\">LED Off</button>\
<br>LED STATE: <span id=\"state\">OFF</span>\
</body>\
<script>\
var xhttp = new XMLHttpRequest();\
xhttp.onreadystatechange = function() {if (this.readyState == 4 && this.status == 200) {document.getElementById(\"state\").innerHTML = this.responseText;}};\
function ledOn() {xhttp.open(\"GET\", \"/on\"); xhttp.send();}\
function ledOff() {xhttp.open(\"GET\", \"/off\"); xhttp.send();}\
</script></html>";

const char webpageON[] = "<html>
<body>
<h1>LED is ON!</h1>
</body>
</html>";

WiFiWebServer server(80);

//Return the web page
void handleRoot()
{
  server.send(200, F("text/html"), webpage);
}

//Switch LED on and acknowledge
void ledON()
{
  digitalWrite(LED_BUILTIN,1); //outputs 1 for LED aka makes LED = True which makes LED light up
  server.send(200, F("text/plain"), webpageON); //i think this updates URL to be ...ip.../ON
} 

//Switch LED off and acknowledge
void ledOFF()
{
  digitalWrite(LED_BUILTIN,0); //outputs 0 for LED so LED = False so LED switches off
  server.send(200, F("text/plain"), F("OFF")); //i think this updates URL to be ...ip.../ON
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

void setup() //Arduino Setup Routine which executes in the beginning ONCE.
{
  pinMode(LED_BUILTIN, OUTPUT); //LED_BUILTIN is the name to a specific pin, so we configure this pin to output
  digitalWrite(LED_BUILTIN, 0);//outputs 0 for LED so LED = False so LED switches off. So initially LED is off at startup

//Set up the serial interface for debugging
//These are the startup statements that are outputted
  Serial.begin(9600);

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

  //Register the callbacks to respond to HTTP requests
  //so this is similar to the URL thingy where at the end of the URL we check if its /on,/off do determine what function to execute
  server.on(F("/"), handleRoot);
  server.on(F("/on"), ledON);
  //so we keep the website on the server by changing URL to /on and executing function ledON
  server.on(F("/off"), ledOFF);
  //so server.on() executed a callback to function ledOFF if /off is present.
  //server.handleClient() runs the necessary callback by doing the HTTP stuff and calling the required function

  server.onNotFound(handleNotFound); //if no URL handle is found then we execute handleNotFound function
  
  server.begin();
  
  Serial.print(F("HTTP server started @ "));
  Serial.println(static_cast<IPAddress>(WiFi.localIP()));

  //so we load in all the possible handlers into void setup(). 
  //these handlers are in the form: server.on("/handleURL", handle func)
  //so we execute handle func when URL has /handleURL attached to it. 
  //each handle has its own func which is assigned elsewhere.
  //the correct handle to be executed at the correct time is done in void loop() where 
  //server.handleClient() checks current URL handle and executes the specific function.
  //but we need to assign the handle to the handle func in the void setup ()
}

//Call the server polling function in the main loop
void loop()
{
  //Serial.print("Gah");
  server.handleClient(); //
  //Serial.print("MMM");
}


void setup() {
...
  server.on(F("/on"), ledON);
...
}

        let frontButton = createButton(x:147, y:189, title:"Front", color:.systemBlue); //x:147
        view.addSubview(frontButton);

func createButton(x:Int, y:Int, title:String, color:UIColor) -> UIButton {
  let button = UIButton(frame: CGRect(x: x, y: y, width: 100, height: 50));
  button.configuration = createConfig(title:title, color:color);
  return button;
}

func createConfig(title : String, color : UIColor) -> UIButton.Configuration {
  var config: UIButton.Configuration = .filled();
  config.baseBackgroundColor = color;
  config.title = title;
  return config;
  }
}

frontButton.addTarget(self, action:#selector(BtnPressedFront), for: .touchDown);
frontButton.addTarget(self, action:#selector(BtnPressedDefault), for: [.touchUpInside, .touchUpOutside]);

@objc private func BtnPressedFront () {
  webview.load(URLRequest(url: URL(string: "http://192.168.0.4/f")!));
}