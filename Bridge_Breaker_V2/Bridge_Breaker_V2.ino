#include <RotaryEncoderWithButton.h>
RotaryEncoderWithButton rotaryEncoder(18,19,2);

void setup() {
  // put your setup code here, to run once
  Serial.begin(9600);
  graphics_start();
  led_start();
  file_start();
  daq_start();
  graphics_main_screen();
 }

void loop() {

led_task();
file_task();
}
