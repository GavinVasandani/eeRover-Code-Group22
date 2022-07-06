#include <LiquidCrystal.h>

 

int Htime;              //integer for storing high time

int Ltime;                //integer for storing low time

float Ttime;            // integer for storing total time of a cycle

float frequency;        //storing frequency

 

void setup(){

  Serial.begin(9600);

  pinMode(8, INPUT);

}

 

void loop(){

  Htime = pulseIn(8, HIGH);   //read high time

  Ltime = pulseIn(8, LOW);     //read low time

 

  Ttime = Htime + Ltime;

  frequency = 1000000/ (Ttime); //getting frequency with Ttime is in Micro seconds

 

  if ((frequency>230) && (frequency<250)){ //if frequency is within these values it is correct

    frequency = 239;

  }

  else if ((frequency>140)&&(frequency<160)){ //if frequency is within these values it is correct

    frequency = 151;

  }

 

  Serial.println(frequency);

}