#include "comms/comms.h"
#include "misc/logging.h"
#include "pyro/pyro.h"
#include "misc/timer.h"
#include "sensors/sensors.h"
#include "sensors/trajectory_controller.h"

#include <assert.h>
#include <cstring>
#include <cstdint>

#define AURP_VERSION 1
#define AURP_SIZE 18

const uint16_t version = 71;

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
  while (comms.getStatus() != DISCONNECTED)
  {
    comms.powerOn();
  }

  while (comms.getStatus() != CONNECTED)
  {
    comms.connect();
  }

  comms.connectUDP("", "");
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

  if (comms.getStatus() == CONNECTED)
  {
    comms.prepUDP(AURP_SIZE);
  }

  else if (comms.getStatus() == UDP_READY)
  {
    // TODO

    uint8_t packet[AURP_SIZE];
    unsigned long timestamp = timer.getTime();

    float temperature = 0.07;
    float pressure = 0.07;

    std::memcpy(&packet, &version, sizeof(uint16_t));
    std::memcpy(&packet[4], &timestamp, sizeof(unsigned long));
    std::memcpy(&packet[4], &timestamp, sizeof(float));
    std::memcpy(&packet[4], &timestamp, sizeof(float));

    comms.sendUDP(packet, AURP_SIZE);
  }
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