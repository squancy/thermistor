// Analog pins for thermistors
#define PIN1 A0
#define PIN2 A1

// Resistance at 25 degrees C
#define THERMISTORNOMINAL 10000      

// Temp. for nominal resistance (almost always 25 C)
#define TEMPERATURENOMINAL 25   
#define NUMSAMPLES 5

// The beta coefficient of the thermistor (usually 3000-4000)
#define BCOEFFICIENT 3950

// The value of the 'other' resistor
#define SERIESRESISTOR 10000    
 
int samples[NUMSAMPLES];

void takeSamples(int limit, int *arr, int tpin) {
  for (int i = 0; i < limit; i++) {
    arr[i] = analogRead(tpin);
    delay(10);
  }
}

float calcAverage(int limit, int *arr) {
  float average = 0;
  for (int i = 0; i < limit; i++) {
    average += arr[i];
  } 
    
  average /= limit;
  Serial.print("Average analog reading "); 
  Serial.println(average);

  // convert the value to resistance
  average = 1023 / average - 1;
  average = SERIESRESISTOR / average;
  Serial.print("Thermistor resistance "); 
  Serial.println(average);

  return average;
}

float calcSteinhart(float average) {
  float steinhart;
  steinhart = average / THERMISTORNOMINAL;     // (R/Ro)
  steinhart = log(steinhart);                  // ln(R/Ro)
  steinhart /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
  steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
  steinhart = 1.0 / steinhart;                 // Invert
  steinhart -= 273.15;                         // convert absolute temp to C
  
  Serial.print("Temperature "); 
  Serial.print(steinhart);
  Serial.println(" *C");
  
  return steinhart;
}

void measureThermistor(int pin) {
  float average;
  float steinhart;
 
  // take N samples in a row, with a slight delay
  takeSamples(NUMSAMPLES, samples, pin);
  
  // average all the samples out
  average = calcAverage(NUMSAMPLES, samples);
    
  steinhart = calcSteinhart(average);
}
 
void setup(void) {
  Serial.begin(9600);
  analogReference(EXTERNAL);
}

void loop(void) {
  measureThermistor(PIN1);
  measureThermistor(PIN2);

  delay(1000);
}
