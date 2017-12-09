void switch_mode(){
  if(mode == 0){
    mode=1;
    target_temp = 0;
    fan_speed=255;
  }
  else{
    mode = 0;
    target_temp = max_temp;
  }
}
void switch_lights(){
  Serial.println("Switching");
  if (lights_state == 1){
    lights_state = 0;
    digitalWrite(light_pin, LOW);
  }
  else{
    lights_state = 1;
    digitalWrite(light_pin, HIGH);
  }
}

