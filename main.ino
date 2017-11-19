//temp and hum sensor
#include <Wire.h>
#include "SparkFunHTU21D.h"

// inicialize from lib
HTU21D HTUsensor;
float temp_HTU;
float hum_HTU;
void read_htu(){
// načtení informací o teplotě a vlhkosti
  // do proměnných
  temp_HTU = HTUsensor.readTemperature();
  hum_HTU = HTUsensor.readHumidity();
  if (temp_HTU > 125 | hum_HTU > 100) {
   // Serial.println("error commuicating with HTU sensor!");
  }
  else {
   // printing temp
   // Serial.print("HTU21D | TEMP: ");
   // Serial.print(HTUsensor, 1);
   // Serial.print(" deg C | ");
   // Serial.print("HUM: ");
   // Serial.print(HTUsensor, 1);
   // Serial.print("%");
   // Serial.println();
  }
}

//encoder
int ABS_MAX_TEMP =  60;
  int menu_pos;
  int pos;
  
// proměnné pro nastavení propojovacích pinů
int pinCLK = 3;
int pinDT  = 4;
int pinSW  = 5;

// proměnné pro uložení pozice a stavů pro určení směru
// a stavu tlačítka
int poziceEnkod = 0;
int stavPred;
int stavCLK;
int stavSW;


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
float temp_term;

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

void read_encoder(){

  // načtení stavu pinu CLK
  stavCLK = digitalRead(pinCLK);
  // pokud je stav CLK odlišný od předchozího měření,
  // víme, že osa byla otočena
  if (stavCLK != stavPred) {
    // pokud stav pinu DT neodpovídá stavu pinu CLK,
    // byl pin CLK změněn jako první a rotace byla
    // po směru hodin, tedy vpravo
    if (digitalRead(pinDT) != stavCLK) {
      Serial.println("Rotace vpravo => | ");
      pos ++;
    }
    // v opačném případě, tedy pin DT byl změněn
    // jako první, se jedná o rotaci
    // proti směru hodin, tedy vlevo
    else {
      Serial.println("Rotace vlevo  <= | ");
      pos --;
    }
    // vytištění aktuální hodnoty pozice enkodéru
    //Serial.print("Pozice enkoderu: ");
    //Serial.println(poziceEnkod);
  }
  // uložení posledního stavu pinu CLK
  // jako reference pro další porovnávání
  stavPred = stavCLK;
  // načtení stavu pinu SW - tlačítko
  stavSW = digitalRead(pinSW);
  // v případě stisknutí vytiskni informaci
  // po sériové lince
  if (stavSW == 0) {
    //Serial.println("Stisknuto tlacitko enkoderu!");
    if (pos>0 || menu_pos == 0){
    menu_pos += 1;
    pos =0;
    }
    else{
    menu_pos -= 1;
    pos = 0;
    }
    Serial.print("menu ");
    Serial.println(menu_pos);
    delay(500);
  }
}

void show_display(int menu, int pos){
  if(menu == 0){
  Serial.print("term");
  Serial.print(temp_term);
  Serial.println("*C");
  Serial.print("HTU");
  Serial.print(temp_HTU);
  Serial.println("*C");

  Serial.print(hum_HTU);
  Serial.println("%");
  }
  else if(menu == 1){
    //showing menu:
    switch (pos){
      case 0:
        Serial.print(">");
        Serial.println(" Return");
        Serial.println("  PLA");
        Serial.println("  ABS");
        Serial.println("  Settings");
        
        break;
      case 1:
        Serial.println("  Return");
        Serial.print(">");
        Serial.println(" PLA");
        Serial.println("  ABS");
        Serial.println("  Settings");
        
        break;
      case 2:
        Serial.println("  Return");
        Serial.println("  PLA");
        Serial.print(">");
        Serial.println("  ABS");
        Serial.println("  Settings");
        
        break;
      case 3:
        Serial.println("  Return");
        Serial.println("  PLA");
        Serial.println("  ABS");
        Serial.print(">");
        Serial.println(" Settings");
        break;

    }
     
  }
  else if(menu >1){
    //settings menu
    switch(pos){
    case 0:
    Serial.print(">");
    Serial.print(" Return");
    Serial.print("  ABS temperature   ");
    Serial.print(ABS_MAX_TEMP);
    Serial.println("*C");
    Serial.println("  Save");
    break;
    
    case 1:
    Serial.print("  Return");
    Serial.print(">");
    Serial.print(" ABS temperature   ");
    Serial.print(ABS_MAX_TEMP);
    Serial.println("*C");
    Serial.println("  Save");
    break;
    case 2:
    Serial.print("  Return");
    Serial.print("  ABS temperature   ");
    Serial.print(ABS_MAX_TEMP);
    Serial.println("*C");
    Serial.print(">");
    Serial.println(" Save");
    break;
    }
   }
  }

void setup() {
  // komunikace přes sériovou linku rychlostí 9600 baud
  Serial.begin(9600);
  
  //encoder 
  // nastavení propojovacích pinů jako vstupních
  pinMode(pinCLK, INPUT);
  pinMode(pinDT, INPUT);
  // nastavení propojovacího pinu pro tlačítko
  // jako vstupní s pull up odporem
  pinMode(pinSW, INPUT_PULLUP);
  // načtení aktuálního stavu pinu CLK pro porovnávání
  stavPred = digitalRead(pinCLK);   
  //encoder end

//thermistor
// connect AREF to 3.3V and use that as VCC, less noisy!
  analogReference(EXTERNAL);

//temp and hum sensor
HTUsensor.begin();
  
} 

void loop() {
//read_htu();
//read_term();
read_encoder();
//show_display(menu_pos, pos);
}

