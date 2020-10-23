#define runLED 13
#define stopLED 12

void ledConf()
{
pinMode(runLED, OUTPUT);
pinMode(stopLED, OUTPUT);  
}

byte setLEDState(byte LEDState)
{

if(0x0F & LEDState)
Serial.println(F("runLED"));
if(0xF0 & LEDState)
Serial.println(F("stopLED"));
if(!(0xFF & LEDState))
Serial.println(F("No LED"));

digitalWrite(runLED,0x0F & LEDState);
digitalWrite(stopLED,0xF0 & LEDState);
}
