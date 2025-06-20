#include "comms.h"
#include "../infrastructure/logging.h"

DummyComms::DummyComms(ILogger &logger) : logger{logger} {}

int DummyComms::powerOn()
{
    logger.logln("$ COMMS: POWER ON");
    return 0;
}

int DummyComms::getStatus()
{
    return 0;
}

int DummyComms::connect()
{
    logger.logln("$ COMMS: CONNECT");
    return 0;
}

int DummyComms::disconnect()
{
    logger.logln("$ COMMS: DISCONNECT");
    return 0;
}

int DummyComms::HTTPGet(const char *url)
{
    logger.log("> COMMS: GET ");
    logger.logln(url);
    return 0;
}