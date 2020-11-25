#include <Arduino.h>

const byte DialButton = 18;
volatile byte DialPress = LOW;
boolean testing = false;
unsigned long initMillis;
unsigned long currentMillis;

void setup() {
  // put your setup code here, to run once:
  pinMode(DialButton, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(DialButton), handleButton, FALLING);
  
  graphicsStart();
  filesystemStart();
  mainScreen();
}

void loop() {

if(DialPress)
{
DialPress=LOW;
testing = !testing;
initMillis=millis();
}

if(!testing)
{
mainScreenRun(false);  
}
else
{
mainScreenRun(true);
updateMainScreen(((currentMillis-initMillis)/1000.0), 0.0, 0.0);  
}


storageObserver();
currentMillis=millis();
}


void storageObserver()
{
fsChkStorage();
if(fsDeviceChange())
{
updateStorageGlyph(sdPresent(),usbPresent());
}
}

void handleButton()
{
DialPress = HIGH;  
}
