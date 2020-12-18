#define greenLED 13
#define redLED 12

bool led_red_state;
bool led_green_state;
bool led_red_last_state;
bool led_green_last_state;
uint8_t led_mode;

unsigned long led_previous_millis = 0;
const long led_interval = 500;

void led_start() //Configures led pin mode
{
  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);
}

void led_set_mode(uint8_t ledMode) //Changes mode for LEDs
{
  led_mode = ledMode;
}

void led_task() //Checks led interval and changes LED states to match different modes
{
unsigned long led_current_millis = millis();
if(led_current_millis - led_previous_millis >= led_interval)
{
  led_previous_millis = led_current_millis;
  switch (led_mode)
  {
    case 0: //Both off
      led_red_state = false; 
      led_green_state = false;
      break;
    case 1: //Green on, Red off
      led_red_state = false;
      led_green_state = true;
      break;
    case 2: //Green on, Red flashing
      led_red_state = !led_red_state;
      led_green_state = true;
      break;
    case 3: //Red on, Green off
      led_red_state = true;
      led_green_state = false;
      break;
    case 4: //Red on, Green flashing
      led_red_state = true;
      led_green_state = !led_green_state;
      break;
    default: //Undefined, so both set to off
      led_red_state = false;
      led_green_state = false;
  }
  if (led_green_state != led_green_last_state) //Only update digital pins if state changed, time saved is negligible but it is still time being saved.
    digitalWrite(greenLED, led_green_state);
  if (led_red_state != led_red_last_state)
    digitalWrite(redLED, led_red_state);

  led_red_last_state = led_red_state;
  led_green_last_state = led_green_state;
}
}
