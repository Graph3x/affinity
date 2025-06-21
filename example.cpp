#include "comms/comms.h"
#include "infrastructure/logging.h"
#include "pyro/pyro.h"
#include "infrastructure/timer.h"
#include "sensors/sensors.h"
#include "sensors/trajectory_controller.h"

#include <assert.h>

PrintLogger logger = PrintLogger();

DummyComms comms = DummyComms(logger);
DummyChannel pyro = DummyChannel(logger);
DummyTimer timer = DummyTimer();

DummyGrowingSensor gpsHeight = DummyGrowingSensor(2);
DummyGrowingSensor baroHeight = DummyGrowingSensor(1, 2);
DummyGrowingSensor accel = DummyGrowingSensor(0, 1);

TrajectoryController trajectoryController = TrajectoryController(baroHeight, gpsHeight, accel);

void setup()
{
  comms.powerOn();
  comms.connect();
}

void loopy()
{
  unsigned long tickTime = timer.getTime();

  // TODO lauch detection pin
  if (timer.timeSinceLaunch() == 0 && tickTime >= 6)
  {
    logger.logln("$ LAUNCH");
    timer.launch();
    pyro.unlock();
  }

  // simulate apogee
  if (tickTime == 15)
  {
    logger.logln("$ APOGEE");
  }

  // primary pyro
  if (trajectoryController.tick() == DESCENDING)
  {
    assert(timer.timeSinceLaunch() != 0);
    logger.logln("$ PRIMARY PYRO TRIGGERED");
    pyro.blow();
  }

  // backup pyro
  if (timer.timeSinceLaunch() > 20)
  {
    if (pyro.getStatus() == READY)
    {
      logger.logln("$ BACKUP PYRO TRIGGERED");
      pyro.blow();
    }
  }

  // TODO async and udp
  comms.HTTPGet("http://example.com/data?=asdf");
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