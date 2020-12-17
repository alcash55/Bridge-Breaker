#include <RotaryEncoderWithButton.h>
RotaryEncoderWithButton rotaryEncoder(18,19,2);

void setup() {
  // put your setup code here, to run once
  led_start();
  led_set_mode(2);
  daq_start();
  graphics_start();
 }

void loop() {
  // put your main code here, to run repeatedly:
led_task();
}
