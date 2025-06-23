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

int heightData[] = {0, 0, 0, 0, 0, 0, 2, 5, 7, 9, 15, 20, 36, 39, 41, 42, 41, 37, 34, 31, 28, 25, 22, 19, 16, 13, 11, 8, 5, 3};
int accelUDData[] = {0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
int pinData[] = {1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

DummyArraySensor gpsHeight = DummyArraySensor(heightData, 30);
DummyArraySensor baroHeight = DummyArraySensor(heightData, 30);
DummyArraySensor accelUD = DummyArraySensor(accelUDData, 30);
DummyArraySensor startPin = DummyArraySensor(pinData, 30);

TrajectoryController trajectoryController = TrajectoryController(baroHeight, gpsHeight, accelUD);

void setup()
{
  comms.powerOn();
  comms.connect();
}

void loopy()
{
  timer.getTime();

  if (timer.timeSinceLaunch() == 0 && startPin.readValue() == 0)
  {
    logger.logln("$ LAUNCH");
    timer.launch();
    pyro.unlock();
  }

  int status = trajectoryController.tick();
  const char *stringStatus = statusToString(status);
  logger.logln(stringStatus);

  // primary pyro
  if (status == DESCENDING && pyro.getStatus() == READY)
  {
    logger.logln("$ PRIMARY PYRO TRIGGERED");
    pyro.blow();
  }

  // backup pyro
  if (timer.timeSinceLaunch() > 20 && pyro.getStatus() == READY)
  {
    logger.logln("$ BACKUP PYRO TRIGGERED");
    pyro.blow();
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