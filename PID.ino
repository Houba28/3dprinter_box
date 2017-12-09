//term
// which analog pin to connect
#define THERMISTORPIN A0         
// resistance at 25 degrees C
#define THERMISTORNOMINAL 100000      
// temp. for nominal resistance (almost always 25 C)
#define TEMPERATURENOMINAL 25   
// how many samples to take and average, more takes longer
// but is more 'smooth'
#define NUMSAMPLES 10
// The beta coefficient of the thermistor (usually 3000-4000)
#define BCOEFFICIENT 3950
// the value of the 'other' resistor
#define SERIESRESISTOR 1000    
  
uint16_t samples[NUMSAMPLES];


void read_term(){
  uint8_t i;
  float average;
 
  // take N samples in a row, with a slight delay
  for (i=0; i< NUMSAMPLES; i++) {
   samples[i] = analogRead(THERMISTORPIN);
   delay(10);
  }
 
  // average all the samples out
  average = 0;
  for (i=0; i< NUMSAMPLES; i++) {
     average += samples[i];
  }
  average /= NUMSAMPLES;
 
  //Serial.print("Average analog reading "); 
  //Serial.println(average);
 
  // convert the value to resistance
  average = 1023 / average - 1;
  average = SERIESRESISTOR / average;
  //Serial.print("Thermistor resistance "); 
  //Serial.println(average);
 
  temp_term = average / THERMISTORNOMINAL;     // (R/Ro)
  temp_term = log(temp_term);                  // ln(R/Ro)
  temp_term /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
  temp_term += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
  temp_term = 1.0 / temp_term;                 // Invert
  temp_term -= 273.15;                         // convert to C
 
  //Serial.print("Temperature "); 
  //Serial.print(int(temp_term));
  //Serial.println(" *C");
}
void keep_target(){
  if(mode == 1){
    digitalWrite(vent_fan_pin, HIGH);
    
  }
  else{
    digitalWrite(vent_fan_pin, LOW);
    
}
}

