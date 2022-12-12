enum LightState {
  GREEN,
  ORANGE,
  RED
};

int TRAFFIC_LIGHT_ONE[3] = {21, 19, 18};
int TRAFFIC_LIGHT_TWO[3] = {26, 25, 33};

const int BUTTON_PIN = 2;

const long GREEN_PERIOD = 4000;
const long ORANGE_PERIOD = 2000;
const long RED_PERIOD = GREEN_PERIOD + ORANGE_PERIOD;

LightState update_state(long current_time, long previous_time, LightState current_state){
  long time_diff = current_time - previous_time;
  if (time_diff > RED_PERIOD && current_state == RED) return GREEN; 
  if (time_diff > ORANGE_PERIOD && current_state == ORANGE) return RED; 
  if (time_diff > GREEN_PERIOD && current_state == GREEN) return ORANGE; 
  return current_state;
}

void set_traffic_light(LightState state, int led_pins[3]){
  for (int i = 0; i < 3; i++) digitalWrite(led_pins[i], LOW);
  if (state == RED) digitalWrite(led_pins[RED], HIGH);
  if (state == ORANGE) digitalWrite(led_pins[ORANGE], HIGH);
  if (state == GREEN) digitalWrite(led_pins[GREEN], HIGH);
} 

bool button_pressed = false;
long last_button_press = millis();

bool process_button_press(bool button_pressed, long last_button_press, long current_time){
  const long BUTTON_DEBOUNCE_DELAY = RED_PERIOD;
  long time_diff = current_time - last_button_press;
  if (time_diff > BUTTON_DEBOUNCE_DELAY && button_pressed) return true;
  else return false;
}

void setup(){
  for (int i = 0; i < 3; i++) pinMode(TRAFFIC_LIGHT_ONE[i], OUTPUT);
  for (int i = 0; i < 3; i++) pinMode(TRAFFIC_LIGHT_TWO[i], OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
  Serial.begin(9600);
}

long current_time, previous_time_one, previous_time_two = millis();

LightState current_state_one = GREEN;
LightState previous_state_one = RED;
LightState current_state_two = RED;
LightState previous_state_two = GREEN;

void loop(){
  current_time = millis();
  button_pressed = digitalRead(BUTTON_PIN);
  if (process_button_press(button_pressed, last_button_press, current_time) 
      && current_state_one == RED) {
    previous_time_one = current_time + ORANGE_PERIOD - RED_PERIOD;
    current_state_two = ORANGE;
    last_button_press = millis();
    Serial.println("Button Pressed");
  } 

  current_state_one = update_state(current_time, previous_time_one, current_state_one);
  current_state_two = update_state(current_time, previous_time_two, current_state_two);
  
  if (current_state_one != previous_state_one){
    set_traffic_light(current_state_one, TRAFFIC_LIGHT_ONE);
    previous_state_one = current_state_one;
    previous_time_one = current_time;
  }
  if (current_state_two != previous_state_two){
    set_traffic_light(current_state_two, TRAFFIC_LIGHT_TWO);
    previous_state_two = current_state_two;
    previous_time_two = current_time;
  }

}
