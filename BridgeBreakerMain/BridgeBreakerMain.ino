void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
ledConf();
}

void loop() {
  // put your main code here, to run repeatedly:

setLEDState(0x00);
delay(1000);
setLEDState(0xF0);
delay(1000);
setLEDState(0x00);
delay(1000);
setLEDState(0x0F);
delay(1000);
}
