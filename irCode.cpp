int SAMPLE_CYCLE = 5;     // number of complete periods we sample and take median of
bool sampling_flag = false;

int AnalogPin = A0;


void StartSample()
{
  sampling_flag = true;
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(AnalogPin, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  long total_duration = 0;
  double frequency = 0;
  long total_duration_averaged = 0;  
  if (sampling_flag) {
    // starts sampling the values into samples
    for (int i = 0; i < SAMPLE_CYCLE; i++) {
      long low_duration = pulseIn(AnalogPin, LOW);
      long high_duration = pulseIn(AnalogPin, HIGH);
      total_duration += high_duration + low_duration;
      Serial.println(low_duration);
      Serial.println(high_duration);
      Serial.println(total_duration);
    }
    // takes median of the samples
    total_duration_averaged = total_duration / SAMPLE_CYCLE;
    Serial.println("total_duration_averaged");
    Serial.println(total_duration_averaged);
    frequency = 1000000.0 / total_duration_averaged;

    sampling_flag = false;
    // prints the median value
    Serial.print("frequency: ");
    Serial.println(frequency);
    Serial.println();
  }
  else {
    // set sample flag to true if input an 'a'
    if (Serial.available() > 0) {
      char input = Serial.read();
      if (input == 'a') {
        StartSample();
      }
    }
  }
  delay(500);
}