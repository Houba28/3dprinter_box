#include "U8glib.h"

U8GLIB_SSD1306_128X64 myOled(U8G_I2C_OPT_NONE);

void draw_screen(){
  myOled.setFont(u8g_font_profont12);
  int i = 1;
  for(i; i <= menu_item_list[menu][0].toInt(); i++){
   
  if(menu == 0){
   myOled.setPrintPos(0, -2+(i*12));
   }
  else{
     if(i == pos+1){
   myOled.setPrintPos(0, -5+(i*15));
      myOled.print(">");
  }
   myOled.setPrintPos(10, -5+(i*15));
  }
   myOled.print(menu_item_list[menu][i]);
}  
}

String get_menu_line(String input_word, int * input_number, String mark){
  
  String final_str= input_word;
  for(int i = 0; i < (128/6)-(input_word.length()+2+String(*input_number).length()+mark.length()); i++){
final_str += " ";
  }
final_str += String(*input_number);
final_str += mark;
  return final_str;
}



String get_menu_temp(String input_word, int * input_number, int * input_number2){
  
  String final_str= input_word;
  for(int i = 0; i < (128/6)-(input_word.length()+4+String(*input_number).length()+String(*input_number2).length()); i++){
final_str += " ";
  }
final_str += String(*input_number);
final_str += "/";
final_str += String(*input_number2);
final_str += "*C";
  return final_str;
}

String get_mode(){

return  "Switch mode   "+ modes[mode];
}

String get_line_str(String input_word, String * state){
   String final_str = input_word;
  for(int i = 0; i < (128/6)-(input_word.length()+3+(*state).length()); i++){
final_str += " ";
  }
final_str += *state;
return final_str;
}
void display_update(){
  
    menu_item_list[0][2] = get_menu_temp("Temperature", &temp_HTU, &target_temp);
    menu_item_list[0][3] = get_menu_line("Fan",&fan_speed, " ");
    menu_item_list[0][4] = get_menu_line("Humidity" ,&hum_HTU, "%");
    menu_item_list[0][5] = get_menu_line("Gases",123, " ");
    menu_item_list[2][2] = get_menu_line("Max temp",&max_temp, " ");
    menu_item_list[1][2] = get_mode();
    menu_item_list[1][3] = get_line_str("Lights", &lights[lights_state]);
    myOled.firstPage();
    do {
      draw_screen();
    } while( myOled.nextPage() );
}

