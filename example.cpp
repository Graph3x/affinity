#include "./comms/comms.h"

LillyGoSIM800L comms = LillyGoSIM800L();

void setup() {
  Serial.begin(115200);
  delay(3000);

  int x = comms.powerOn();
  x = comms.connect();

  Serial.println("STARTING");

  for(int i = 0; i < 10; i++){
    x = comms.sendData("asdf");
  }

  x = comms.disconnect();
  Serial.println("DONE");
}


void loop() {
}