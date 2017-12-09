

static int pinA = 2; // Our first hardware interrupt pin is digital pin 2
static int pinB = 3; // Our second hardware interrupt pin is digital pin 3
volatile byte aFlag = 0; // let's us know when we're expecting a rising edge on pinA to signal that the encoder has arrived at a detent
volatile byte bFlag = 0; // let's us know when we're expecting a rising edge on pinB to signal that the encoder has arrived at a detent (opposite direction to when aFlag is set)
volatile byte oldEncPos = 0; //stores the last encoder position value so we can compare to the current reading and see if it has changed (so we know when to print to the serial monitor)
volatile byte reading = 0; //somewhere to store the direct values we read from our interrupt pins before checking to see if we have moved a whole detent
int pinSW  = 5;
int stavSW;

bool temp_change = false;


void encoder_prepare(){
  pinMode(pinA, INPUT_PULLUP); // set pinA as an input, pulled HIGH to the logic voltage (5V or 3.3V for most cases)
  pinMode(pinB, INPUT_PULLUP); // set pinB as an input, pulled HIGH to the logic voltage (5V or 3.3V for most cases)
  attachInterrupt(0,PinA,RISING); // set an interrupt on PinA, looking for a rising edge signal and executing the "PinA" Interrupt Service Routine (below)
  attachInterrupt(1,PinB,RISING); // set an interrupt on PinB, looking for a rising edge signal and executing the "PinB" Interrupt Service Routine (below)
}

void PinA(){
  cli(); //stop interrupts happening before we read pin values
  reading = PIND & 0xC; // read all eight pin values then strip away all but pinA and pinB's values
  if(reading == B00001100 && aFlag) { //check that we have both pins at detent (HIGH) and that we are expecting detent on this pin's rising edge
    if(temp_change ){
      if( max_temp > 0){
      max_temp -=1;  }
    }
    else{
    pos --; //decrement the encoder's position count
    if(pos < 0){
      pos = 0;
    }}
    bFlag = 0; //reset flags for the next turn
    aFlag = 0; //reset flags for the next turn
    
  }
  else if (reading == B00000100) bFlag = 1; //signal that we're expecting pinB to signal the transition to detent from free rotation
  sei(); //restart interrupts
}

void PinB(){
  cli(); //stop interrupts happening before we read pin values
  reading = PIND & 0xC; //read all eight pin values then strip away all but pinA and pinB's values
  if (reading == B00001100 && bFlag) { //check that we have both pins at detent (HIGH) and that we are expecting detent on this pin's rising edge
    if(temp_change){
      if(max_temp < 60){
      max_temp +=1;  }
    }
    else{
    pos ++; //increment the encoder's position count
    if(pos > menu_item_list[menu][0].toInt()-1){
      pos = menu_item_list[menu][0].toInt()-1;
    } }
    bFlag = 0; //reset flags for the next turn
    aFlag = 0; //reset flags for the next turn
 
  }
  else if (reading == B00001000) aFlag = 1; //signal that we're expecting pinA to signal the transition to detent from free rotation
  sei(); //restart interrupts
}

void read_encoder(){
     if(oldEncPos != pos) {
    Serial.println(pos);
    oldEncPos = pos;
  }
  stavSW = digitalRead(pinSW);
  if (stavSW == 0) {
    if (menu == 0){
    menu += 1;
    pos = 0;
    }
    else if(pos == 3){
    menu = 2;
    pos = 0;
    }
    else if(pos == 0 && menu != 0){
    menu -= 1;
    }
    else if(menu == 1 && pos == 1){
      switch_mode();
    }
    else if(menu == 1 &&  pos == 2){
     //switch_lights(); 
    }
    else if(menu == 2 &&  pos == 2){
      EEPROM.put(0,max_temp);
      Serial.print("max_temp saved! ");
      Serial.println(max_temp);
    }
    
    if(menu == 2 &&  pos == 1 && temp_change == false){
      temp_change = true;
    }
    else if(menu == 2 &&  pos == 1 && temp_change){
      temp_change = false;
    } 
  delay(200);
  }
}




