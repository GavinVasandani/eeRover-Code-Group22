
#include <Wire.h>
#include <ArduinoWiFi.h>

//Arduino pin definitions      
const int PIN_PWML = 9;    //Power supply pins for left motor
const int PIN_PWMR = 10; //Power supply pins for right motor
const int PIN_DIRL = 3;  //Pin for Left Motor
const int PIN_DIRR = 4;  //Pin for Right Motor

//So power supplies (pins 9,10) feed into each motor so we can adjust speed. Whether motor is on or off depends on pins (3,4)

//Default motor speed (PWM value)
int motorSpeed = 200; //Max=255
//Default motor run time (in milliseconds).  
double motorTime = 1000; //Goes forward 3cm with that time. //So motor moves in bursts of 1 second

//variables
//unsigned long frequency=0;
//int rock=0;

//Arduino setup routine - runs once on powerup
//So void setup is a setup routine that only occurs once and that's during powerup
void setup(){
   
    //Set up the pin directions
    //Used to configure pins so that they output something
    pinMode(PIN_PWML, OUTPUT);
    pinMode(PIN_PWMR, OUTPUT);
    pinMode(PIN_DIRL, OUTPUT);
    pinMode(PIN_DIRR, OUTPUT);
    //pinMode(LED_BUILTIN, OUTPUT);
    
    //Set up the serial interface for debugging
    //These are the startup statements that are outputted
    Serial.begin(9600);
    Serial.println("Hello from Carina");
    
    //Start the wifi connection
    Wifi.begin();
    Wifi.println(F("WebServer Server is up"));
}

//Arduino loop routine - runs continuously after setup
//so void loop is like the void main(), it executed continuously as long as the arduino is ON.
void loop() {
while(Wifi.available()){ //so as long as wifi is present/connected to EE Rover network, then execute.
    webproc();  //Process wifi requests
  }
delay(500);  //If the connection breaks, wait and try again. //So we continuously recheck wifi until the arduino is actually switched off
}
 
void webproc() { //function that is executed when wifi is available and executed
  String command = Wifi.readStringUntil('/'); // read the first part of the URL (after arduino/)
  //Serial.println("URL RECIEVED");
  if (command == "web") {  // web is the url to access the webpage
  //so if the URL is: 196.02.10.16/web then the following is executed:
     command = Wifi.readStringUntil('/'); //so we reassign command to the term after the next /

    Wifi.println(F("HTTP/1.1 200 OK"));
    Wifi.println(F("Content-Type: text/html"));
    Wifi.println();
    Wifi.println(F("<html>"));
    Wifi.print(F("<body>"));
    //we output this html page i think when command is 'web' as we want to output a webpage
    if (command!=""){ //so if the URL is: 196.02.10.16/web/    , then this isn't executed
    //if it is 196.02.10.16/web/... then it is executed
          if (command=="stop"){   //stop the motors, so this is 196.02.10.16/web/stop
            analogWrite(PIN_PWML, 0);    
            analogWrite(PIN_PWMR, 0); //so these functions set motor power to 0 so motors are essentially switched off

          }else if (command=="detect") //detect and print the result in the webpage.
          //function prints the output of the detection so outputs what type of rock it is when detect is inputted through the URL
          { 
           
           for (int i=0; i<2; i++){
           dec();}
          
            
          }else if (command=="l" or command=="f" or command=="r" or command=="b")  //rover has to move.
          //the other possible URLs are all movement based
          //so 196.02.10.16/web/l , moves rover left, 196.02.10.16/web/r , moves rover right...
          
          { 
             Serial.println(F("got direction"));
             movebug(command); //runs movebug function
          }
  
          }
     }
}


void movebug(String& movement){
double y,x; //variables for the correct time/
int denominator; 

String com=Wifi.readStringUntil('/'); //so this assigns the word after the second / to com

if (movement == "l"){
      denominator=90;
      digitalWrite(3, HIGH);  //Set the direction controls, so this ON the left motor and OFF the right
      digitalWrite(4, LOW);
   }else if (movement == "r"){ //Motor configuration to move right
      denominator=90;
      digitalWrite(3, LOW);
      digitalWrite(4, HIGH);
   }else if (movement == "f"){ //Motor config to move forward
      denominator=3;
      digitalWrite(3, HIGH);
      digitalWrite(4, HIGH); 
   }else{ //Motor config to stop
      denominator=3;
      digitalWrite(3, LOW);
      digitalWrite(4, LOW); 
    }
      //if com is nothing l,r,f then we check if it is ty for touch yes
      //nvm we do the set of if statements above first and then the set of if statements below

    if (com=="ty"){ //onTouch just start the motors
      analogWrite(PIN_PWML, motorSpeed);  //so PIN to power left motor is assigned and the duty cycle is motorSpeed/255
      analogWrite(PIN_PWMR, motorSpeed);  //same for right, so we essentially move in bursts based on motorSpeed (GO OVER)
    }
    else{ //calculate desired time.
      com=Wifi.readStringUntil('/'); //NOT SURE GO OVER
      x=com.toFloat(); //maybe after URL so we do 196.02.16/l/ty/10 means we move left and touch yes for 10 seconds
      //so we have analogue write which powers the motors continuously until the time y is done and then we reset.
      //hence we only move for a specific amount of time
      y=x*(motorTime)/denominator;
      analogWrite(PIN_PWML, motorSpeed);    
      analogWrite(PIN_PWMR, motorSpeed);  
      delay(y);                               //Run the motors for the specified time
      analogWrite(PIN_PWML, 0);    
      analogWrite(PIN_PWMR, 0);
     }
 
Wifi.print (F("rover's movement"));
Wifi.print(F("</body>"));
Wifi.println(F("</html>"));
Wifi.print(DELIMITER); // very important to end the communication! 
}

void sensor (int pin_name, double voltage, int& rock, unsigned long& frequency,int len, double thr, bool& test1) //int pin_name, double voltage
{
byte prevVal,newVal,test;
unsigned long sum=0;
int i,rep=0;
bool done;
int k;
unsigned long readingTime;
unsigned long StartTime;
long CurrentTime;
byte Reading[len];

    frequency=0;
    rock=0;
    StartTime=micros();  //finding reading + putting array time
    Reading[0]=analogRead(pin_name)*(thr/1023.0);
    CurrentTime=micros();
    readingTime=CurrentTime-StartTime;
    
      for (int i=1; i<len; i++){  //get 400 samples
         Reading[i]=analogRead(pin_name)*(thr/1023.0);
      }

while (test1==true and i<len){
  if (Reading[i]>=45){
    test1=false;
  }
  i++;
}
 
       i=0;
       while (i<len){
 
       for (int k=i; k<len; k++){ //finding 0 with rising next
         if (Reading[k]<=voltage and Reading[k+1]>voltage){ 
           StartTime=k*readingTime;
            i=k+1;
           break;}
      }

           if (i<len){ 
            done=true;
            newVal=Reading[i];
            i++;
          
          while (done==true and i<len){
           prevVal=newVal;
             newVal=Reading[i];//take new voltage
            if (prevVal<=voltage and newVal>voltage) //when we find again that the voltage passed through voltage rising keep that time. 
             {
               done=false;
               rep++;
               CurrentTime=i*readingTime;
             sum=sum+CurrentTime-StartTime;
                                         }
                   i++;
                  }
        }
             }


  if (sum>0){
    sum=sum/rep;
    frequency=1000000/sum;
  }else{
   frequency=0;
  }
}


void sensor1(int& rock, unsigned long& frequency)
{ 
    bool test=true;
    sensor(0,50,rock,frequency,200,250,test);
    if (frequency>=60 and frequency<=80){
        rock=1;
    }else if (frequency>=85 and frequency<=95){
        rock=2;
    }else if (frequency>=100 and frequency<=120){ 
       rock=3;
    }else if (frequency>=130 and frequency<=150){ 
        rock=4;
    }else{
       long sum=0;
       float k;
       for (int i=1; i<=10; i++){
       k=analogRead(A2)*(5.0/1023.0);
       sum=sum+k;}
       k=sum/10;

   if (k>=3 and test==false){ 
    rock=2;
    frequency=113;
    }else{
      rock=0;
    }

}
}

void sensor2 (int& rock, unsigned long& frequency)
{
   bool test=true;
   sensor(1,8,rock,frequency,300,10,test);
   if (frequency>=260 and frequency<=280){
       rock=5;
    }else if (frequency>=420 and frequency<=450){
       rock=6;
    }else {
       long sum=0;
       float k;
       for (int i=1; i<=10; i++){
       k=analogRead(A2)*(5.0/1023.0);
       sum=sum+k;}
       k=sum/10;

   if (k>=3){ 
    rock=6;
    frequency=40;
   }else{
      rock=0;
      frequency=0;
      }
    }
}

void dec()
{
  int rock=0;
  unsigned long frequency=0;                           
  sensor1(rock,frequency);
  if (rock==0){
     sensor2(rock,frequency);}   
          if (rock!=0){
          Wifi.print(F("Frequency is:"));
          Wifi.print (frequency);
          Wifi.print(F(" Rock is:"));
          Wifi.print (rock);
          Wifi.print(F("</body>"));
          Wifi.println(F("</html>"));
          Wifi.print(DELIMITER); // very important to end the communication! 
          }else{
          Wifi.println(F("No rock"));
          Wifi.print(F("</body>"));
          Wifi.println(F("</html>"));
          Wifi.print(DELIMITER); // very important to end the communication!
          }
}