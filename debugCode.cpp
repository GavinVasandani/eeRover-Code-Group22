const int INTERVAL = 4;

const int pin_DIRFR = 7; //faulty
const int pin_DIRFL = 8;
const int pin_DIRBR = 9;
const int pin_DIRBL = 10;

const int pin_PWMFR = 3;
const int pin_PWMFL = 6;
const int pin_PWMBR = 11;
const int pin_PWMBL = 12;

void front() {


}

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
    Serial.print(F("Working!"));
    analogWrite(pin_PWMFR, 100);  //REN
    analogWrite(pin_PWMFL, 100);  //LEN
    analogWrite(pin_PWMBR, 100);  //REN
    analogWrite(pin_PWMBL, 100);  //LEN

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
  
  //Serial.println("Hello! This is Rover!");

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