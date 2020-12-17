#define greenLED 13
#define redLED 12

bool led_red_state;
bool led_green_state;
bool led_red_last_state;
bool led_green_last_state;
uint8_t led_mode;

unsigned long led_previous_millis = 0;
const long led_interval = 500;

void led_start()
{
  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);
}

void led_set_mode(uint8_t ledMode)
{
  led_mode = ledMode;
}

void led_task()
{
unsigned long led_current_millis = millis();
if(led_current_millis - led_previous_millis >= led_interval)
{
  led_previous_millis = led_current_millis;
  switch (led_mode)
  {
    case 0:
      led_red_state = false;
      led_green_state = false;
      break;
    case 1:
      led_red_state = false;
      led_green_state = true;
      break;
    case 2:
      led_red_state = !led_red_state;
      led_green_state = true;
      break;
    case 3:
      led_red_state = true;
      led_green_state = false;
      break;
    case 4:
      led_red_state = true;
      led_green_state = !led_green_state;
      break;
    default:
      led_red_state = false;
      led_green_state = false;
  }
  if (led_green_state != led_green_last_state)
    digitalWrite(greenLED, led_green_state);
  if (led_red_state != led_red_last_state)
    digitalWrite(redLED, led_red_state);

  led_red_last_state = led_red_state;
  led_green_last_state = led_green_state;
}
}
