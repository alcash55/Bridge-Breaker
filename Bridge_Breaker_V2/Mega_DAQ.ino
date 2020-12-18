#include <Wire.h>

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

sensor_data external;
float initial_steps;
float initial_weight;
void daq_start()
{
Wire.begin();
}

void daq_poll()
{
Wire.requestFrom(8, sensor_poll_size);    // request X bytes from daq, save to local structure
  if(Wire.available()>=sensor_poll_size) {
    Wire.readBytes(external.byteArray,sensor_poll_size);
  }  
}

void daq_start_test()
{
daq_poll();
initial_steps = external.sensors.encoder_steps;
initial_weight = external.sensors.weight;
}

float daq_get_steps()
{
return external.sensors.encoder_steps - initial_steps;
}

float daq_get_distance_mm()
{
return (((external.sensors.encoder_steps - initial_steps)*5.0)/1000.0);
}

float daq_get_distance_cm()
{
return (((external.sensors.encoder_steps - initial_steps)*5.0)/10000.0);
}

float daq_get_weight()
{
return external.sensors.weight - initial_weight;
}

float daq_get_force()
{
return ((external.sensors.weight - initial_weight)*4.4822162);
}
