#include "comms.h"
#include "../misc/logging.h"

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
    if(status == COMMS_BUSY){
        status = CONNECTED;
    }

    else if(status == PREPPING_UDP){
        status = UDP_READY;
    }

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

int DummyComms::connectUDP(const char *ip, const char *port)
{
    if (status != CONNECTED)
    {
        logger.logln("$ COMMS: [ERROR] NOT CONNECTED");
        return 1;
    }
    logger.log("> COMMS: UDP connected to ");
    logger.log(ip);
    logger.log(":");
    logger.logln(port);

    return 0;
}

int DummyComms::sendUDP(const uint8_t *data)
{
    if (status != UDP_READY)
    {
        logger.logln("$ COMMS: [ERROR] NOT CONNECTED");
        return 1;
    }

    logger.logln("> COMMS: send packet");

    (void)data;

    status = CONNECTED;
    return 0;
}

int DummyComms::prepUDP(size_t length)
{
    if (status != CONNECTED)
    {
        logger.logln("$ COMMS: [ERROR] NOT CONNECTED");
        return 1;
    }
        udpLength = length;
        logger.logln("> COMMS: UDP prepped");

    status = PREPPING_UDP;

    return 0;
}

int DummyComms::disconnectUDP()
{
    if (status != CONNECTED)
    {
        logger.logln("$ COMMS: [ERROR] NOT CONNECTED");
        return 1;
    }

    logger.logln("> COMMS: UDP disconnected");

    return 0;
}
