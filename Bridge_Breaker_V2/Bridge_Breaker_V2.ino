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
      graphics_main(file_sd_present());//Update main display SD card state
      if (button1.pressed()) {
        if (file_sd_present())//Prevent state change if SD card is missing
        {
          state = 1;
        }
        else
        {
         graphics_display_error(0x5D);//Display alert to user that there is an SD error
      delay(1000);
        }
      }
      led_set_mode(1);
      break;

    case 1:

      file_number = file_find_usable_number(); //Acquire usable file number for name
      file_open(file_number);//Init file with usable number
      seconds = 0;
      daq_start_test();//Tare sensors
      state = 2;
      led_set_mode(2);
      break;

    case 2:
led_set_mode(2);
if (button1.pressed()) { //End test
        state = 0;
      }
      unsigned long poll_current_millis = millis(); //Polling interval for sensors, when interval is reached, DAQ is polled for new values.
      if (poll_current_millis - poll_previous_millis >= poll_interval)
      {
        poll_previous_millis = poll_current_millis;
        daq_poll(); //Poll DAQ
        file_print(file_number, seconds, daq_get_force(), daq_get_distance_mm()); //Write Data to SD card
        graphics_test(file_number, seconds, daq_get_force(), daq_get_distance_mm(),file_sd_present()); //Write Data to LCD
        seconds = seconds + poll_interval / 1000.0; // Calculate seconds
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
  led_task(); //Update LEDs
  file_task(); //Handle SD card state and config
}
