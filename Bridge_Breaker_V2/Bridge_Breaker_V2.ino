//#include <RotaryEncoderWithButton.h>
//RotaryEncoderWithButton rotary(18, 19, 2);
#include <Button.h>

uint8_t state = 0;
long delta;
unsigned long poll_previous_millis = 0;
const long poll_interval = 250;
float seconds;
uint16_t file_number;
Button button1(2);

void setup() {
  // put your setup code here, to run once
  Serial.begin(9600);
  //rotary.begin();
  button1.begin();
  graphics_start();
  led_start();
  file_start();
  daq_start();
  led_set_mode(1);
}

void loop() {
  switch (state)
  {
    case 0:
      graphics_main(file_sd_present());
      if (button1.pressed()) {
        if (file_sd_present())
        {
          state = 1;
        }
        else
        {
         graphics_display_error(0x5D);
      delay(1000);
        }
      }
      led_set_mode(1);
      break;

    case 1:

      file_number = file_find_usable_number();
      file_open(file_number);
      seconds = 0;
      daq_start_test();
      state = 2;
      led_set_mode(2);
      break;

    case 2:
led_set_mode(2);
if (button1.pressed()) {
        state = 0;
      }
      unsigned long poll_current_millis = millis();
      if (poll_current_millis - poll_previous_millis >= poll_interval)
      {
        poll_previous_millis = poll_current_millis;
        daq_poll();
        file_print(file_number, seconds, daq_get_force(), daq_get_distance_mm());
        graphics_test(file_number, seconds, daq_get_force(), daq_get_distance_mm(),file_sd_present());
        seconds = seconds + poll_interval / 1000.0;
      }
      break;

    case 200:
      graphics_display_error(0x01);
      delay(1000);
      state = 0;
      break;

    case 201:
     
      state = 0;
      break;

    default:;
  }
  led_task();
  file_task();
}
