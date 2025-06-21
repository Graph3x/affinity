#include "comms.h"
#include "../infrastructure/logging.h"

DummyComms::DummyComms(ILogger &logger) : logger{logger} {}

int DummyComms::powerOn()
{
    if (status != OFFLINE && status != DOWN)
    {
        logger.logln("$ COMMS: [ERROR] NOT OFFLINE");
        return 1;
    }

    logger.logln("$ COMMS: POWER ON");
    status = DISCONNECTED;
    return 0;
}

int DummyComms::getStatus()
{
    return status;
}

int DummyComms::connect()
{
    if (status != DISCONNECTED)
    {
        logger.logln("$ COMMS: [ERROR] NOT DISCONNECTED");
        return 1;
    }

    logger.logln("$ COMMS: CONNECT");
    status = CONNECTED;
    return 0;
}

int DummyComms::disconnect()
{
    if (status != CONNECTED)
    {
        logger.logln("$ COMMS: [ERROR] NOT CONNECTED");
        return 1;
    }

    logger.logln("$ COMMS: DISCONNECT");
    status = DISCONNECTED;
    return 0;
}

int DummyComms::HTTPGet(const char *url)
{
    if (status != CONNECTED)
    {
        logger.logln("$ COMMS: [ERROR] NOT CONNECTED");
        return 1;
    }
    
    logger.log("> COMMS: GET ");
    logger.logln(url);
    
    return 0;
}