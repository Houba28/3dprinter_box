#include <EEPROM.h>
int menu;
int menu_pos;
int pos;
int max_temp =  -1;
int fan_speed;
int target_temp;
bool light = false;
int light_pin = 8;
int last_temp_reading;
long int display_last_update;
float temp_term;
int temp_HTU;
int hum_HTU;
int mode = 0;
String modes[2] = {"HEAT", "COLD"};
int lights_state = 0;
String lights[2] = {"ON", "OFF"};
String get_menu_line(String input_word, int * input_number, String mark);
String get_menu_temp(String input_word, int * input_number, int * input_number2);
String get_mode();
String get_line_str(String input_word, String state);
String menu_item_list[3][6] = {{"5","   3D printer box", get_menu_temp("Temperature",&temp_HTU, &target_temp), get_menu_line("Fan",&fan_speed, "%"), get_menu_line("humidity" ,&hum_HTU, "%"), get_menu_line("Gases",123, " ")},
  {"4","Return", get_mode(), get_line_str("Lights",lights[lights_state]) ,"Settings"},
  {"3","Return", get_menu_line("Max temp",&max_temp, " ") , "Save"}
  };
  

void setup(void) {
  Serial.begin(115200);
  encoder_prepare();
  menu = 0;
  pos = 0;
  //pinMode(light_pin, OUTPUT);
  //thermistor
  analogReference(EXTERNAL);

//temp and hum sensor
init_HTUsensor();
last_temp_reading = -2000;
display_last_update = -250;
EEPROM.get(0,max_temp);
if(max_temp == -1){
  Serial.println("saving eeprom default");
  EEPROM.put(0, 40);
  max_temp=40;
}
}

void loop(void) {
  if((millis() - last_temp_reading )> 1000){
  read_term();
  read_htu();
  last_temp_reading = millis();
  }
  if ((millis()-display_last_update) > 500) {
    display_update();
    display_last_update = millis();
  }
  read_encoder();
}
