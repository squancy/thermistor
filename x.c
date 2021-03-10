// Thermistor Example #3 from the Adafruit Learning System guide on Thermistors 
// https://learn.adafruit.com/thermistor/overview by Limor Fried, Adafruit Industries
// MIT License - please keep attribution and consider buying parts from Adafruit
 
// which analog pin to connect
#define THERMISTORPIN A0         
// resistance at 25 degrees C
#define THERMISTORNOMINAL 10000      
// temp. for nominal resistance (almost always 25 C)
#define TEMPERATURENOMINAL 25   
// how many samples to take and average, more takes longer
// but is more 'smooth'
#define NUMSAMPLES 5
// The beta coefficient of the thermistor (usually 3000-4000)
#define BCOEFFICIENT 3950
// the value of the 'other' resistor
#define SERIESRESISTOR 10000   

// which analog pin to connect
#define THERMISTORPIN2 A1         
// resistance at 25 degrees C
#define THERMISTORNOMINAL2 10000      
// temp. for nominal resistance (almost always 25 C)
#define TEMPERATURENOMINAL2 25   
// how many samples to take and average, more takes longer
// but is more 'smooth'
#define NUMSAMPLES2 5
// The beta coefficient of the thermistor (usually 3000-4000)
#define BCOEFFICIENT2 3950
// the value of the 'other' resistor
#define SERIESRESISTOR2 10000
 
int samples[NUMSAMPLES];
int samples2[NUMSAMPLES];
int samples3[NUMSAMPLES];
 
void setup(void) {
  Serial.begin(9600);
  analogReference(EXTERNAL);
}

// Takes [limit] number of samples with 10ms delays
void takeSamples(int limit, int *arr, int tpin) {
  for (int i = 0; i < limit; i++) {
   arr[i] = analogRead(tpin);
   delay(10);
  }
}
 
void loop(void) {
  uint8_t i;
  uint8_t i2;
  float average;
  float average2;
 
  // take N samples in a row, with a slight delay
  takeSamples(NUMSAMPLES, samples, THERMISTORPIN);
  takeSamples(NUMSAMPLES, samples2, THERMISTORPIN2);
  
  // average all the samples out
  average = 0;
  for (i=0; i< NUMSAMPLES; i++) {
     average += samples[i];
  }
  average /= NUMSAMPLES;
 
  Serial.print("Average analog reading "); 
  Serial.println(average);
  
  // convert the value to resistance
  average = 1023 / average - 1;
  average = SERIESRESISTOR / average;
  Serial.print("Thermistor resistance "); 
  Serial.println(average);

  // average all the samples out
  average2 = 0;
  for (i2=0; i2< NUMSAMPLES2; i++) {
     average2 += samples2[i2];
  }
  average2 /= NUMSAMPLES2;
 
  Serial.print("Average analog reading "); 
  Serial.println(average2);
  
  // convert the value to resistance
  average2 = 1023 / average2 - 1;
  average2 = SERIESRESISTOR2 / average2;
  Serial.print("Thermistor resistance "); 
  Serial.println(average2);
  
  float steinhart;
  steinhart = average / THERMISTORNOMINAL;     // (R/Ro)
  steinhart = log(steinhart);                  // ln(R/Ro)
  steinhart /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
  steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
  steinhart = 1.0 / steinhart;                 // Invert
  steinhart -= 273.15;                         // convert absolute temp to C

  float steinhart2;
  steinhart2 = average / THERMISTORNOMINAL;     // (R/Ro)
  steinhart2 = log(steinhart2);                  // ln(R/Ro)
  steinhart2 /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
  steinhart2 += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
  steinhart2 = 1.0 / steinhart;                 // Invert
  steinhart2 -= 273.15;                         // convert absolute temp to C
  
  Serial.print("Temperature "); 
  Serial.print(steinhart);
  Serial.println(" *C");

  Serial.print("Temperature "); 
  Serial.print(steinhart2);
  Serial.println(" *C");
  
  delay(1000);
}

