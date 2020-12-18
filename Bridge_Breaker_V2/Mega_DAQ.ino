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

//No parameters
//No return
void daq_start() // Start I2C, no assignment on bus because this is the master
{
Wire.begin();
}

//No parameters
//No return
void daq_poll() // Poll DAQ Arduino for sensor status
{
Wire.requestFrom(8, sensor_poll_size);    // request X bytes from daq, save to local structure
  if(Wire.available()>=sensor_poll_size) {
    Wire.readBytes(external.byteArray,sensor_poll_size);
  }  
}

//No parameters
//No return
void daq_start_test() // Poll DAQ Arduino and set initial values to tare reading
{
daq_poll();
initial_steps = external.sensors.encoder_steps;
initial_weight = external.sensors.weight;
}

//No parameters
//Returns float, steps of encoder
float daq_get_steps() // Return encoder steps
{
return external.sensors.encoder_steps - initial_steps;
}

//No parameters
//Returns float, distance in mm
float daq_get_distance_mm() // Return encoder steps in millimeters (Steps * 5.0 micrometers) / 1000 = millimeters
{
return (((external.sensors.encoder_steps - initial_steps)*5.0)/1000.0);
}

//No parameters
//Returns float, distance in cm
float daq_get_distance_cm() // Return encoder steps in centimeters (Steps * 5.0 micrometers) / 10000 = centimeters
{
return (((external.sensors.encoder_steps - initial_steps)*5.0)/10000.0);
}

//No parameters
//Returns float, weight in lbs
float daq_get_weight() // Return weight in pounds
{
return external.sensors.weight - initial_weight;
}

//No parameters
//Returns float, force in newtons
float daq_get_force() // Return force in newtons
return ((external.sensors.weight - initial_weight)*4.4822162);
}
