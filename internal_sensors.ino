
#include <Wire.h>
#include "SparkFunHTU21D.h"

// inicialize from lib
HTU21D HTUsensor;
void init_HTUsensor(){
HTUsensor.begin();
}
void read_htu(){
// načtení informací o teplotě a vlhkosti
  // do proměnných
  temp_HTU = HTUsensor.readTemperature();
  hum_HTU = HTUsensor.readHumidity();
  if (temp_HTU > 125 | hum_HTU > 100) {
    Serial.println("error commuicating with HTU sensor!");
  }
  else {
   //printing temp
//   Serial.print("HTU21D | TEMP: ");
//   Serial.print(temp_HTU);
//   Serial.print(" deg C | ");
//   Serial.print("HUM: ");
//   Serial.print(hum_HTU);
//   Serial.print("%");
//   Serial.println();
  }
}

