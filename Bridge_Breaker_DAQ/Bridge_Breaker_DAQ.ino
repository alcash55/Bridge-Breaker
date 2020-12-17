// I2C Arduino as DAQ for Bridge Breaker
// Bridge Breaker V2
// Braydn Szymkiewicz

//Includes
#include <Wire.h>
#include <Encoder.h>
#include "HX711.h"

//Data structure for I2C sharing of data
typedef struct sensor_poll{
long encoder_steps;
float weight;
};

//Calculate size of DS
const int sensor_poll_size = sizeof(sensor_poll);

//Union for simplified access to data through byte array
typedef union sensor_data{
sensor_poll sensors;
byte byteArray[sensor_poll_size];
};

Encoder glass_Scale(2,3);
HX711 scale;
uint8_t dataPin = 4;
uint8_t clockPin = 5;
sensor_data external;

long old_position = -999;
float calibration_factor = 17950.15;

void setup() {
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onRequest(requestEvent); // register event
  scale.begin(dataPin, clockPin);
  scale.set_scale(calibration_factor);
  scale.tare();
}

void loop() {
  delay(50);
  long newPosition = glass_Scale.read();
  if (newPosition != old_position)
    old_position = newPosition;
    external.sensors.encoder_steps = old_position;
    external.sensors.weight = scale.get_units();
}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent() {
  Wire.write(external.byteArray,sensor_poll_size); // respond with message of sensor poll
  // as expected by master
}
