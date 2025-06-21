#include "./comms/comms.h"
#include "./infrastructure/logging.h"
#include "./pyro/pyro.h"
#include "./infrastructure/timer.h"

PrintLogger logger = PrintLogger();
DummyComms comms = DummyComms(logger);
DummyChannel pyro = DummyChannel(logger);
DummyTimer timer = DummyTimer();

void setup()
{
  comms.powerOn();
  comms.connect();
}

void loopy()
{
  unsigned long tickTime = timer.getTime();

  //launch conditions met
  if(timer.timeSinceLaunch() == 0 && tickTime >= 6){
    logger.logln("LAUNCH");
    timer.launch();
    pyro.unlock();
  }

  if(timer.timeSinceLaunch() > 10){
    if(pyro.getStatus() == READY){
      pyro.blow();
    }
  }

  //TODO async
  comms.HTTPGet("http://tvojemama.com/data?=asdf");
  
}

int main()
{
  logger.logln("STARTED SETUP");
  setup();
  logger.logln("ENDED SETUP");

  logger.logln("STARTED LOOP");
  for (int i = 0; i < 30; i++)
  {
    loopy();
  }
  logger.logln("ENDED LOOP");

  comms.disconnect();

  return 0;
}