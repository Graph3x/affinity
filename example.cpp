#include "./comms/comms.h"
#include "./infrastructure/logging.h"

PrintLogger logger = PrintLogger();
DummyComms comms = DummyComms(logger);

void setup() {

  logger.logln("STARTING");

  comms.powerOn();
  comms.connect();

  logger.logln("STARTED LOOP");

  // limited verze loop() funkce
  for(int i = 0; i < 30; i++){
    
    //TEST EVENT LOOP
    
    comms.HTTPGet("http://tvojemama.com/data?=asdf");
  }

  logger.logln("ENDING LOOP");

  comms.disconnect();
  logger.logln("END");

}


void loop() {
}

int main(){
  setup();
  return 0;
}