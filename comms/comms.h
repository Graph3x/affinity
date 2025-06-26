#pragma once
#include "../misc/logging.h"
#include <stdlib.h>
#include <stdint.h>

enum CommsStatus
{
    OFFLINE,
    DISCONNECTED,
    CONNECTED,
    COMMS_BUSY,
    UDP_READY,
    PREPPING_UDP,
    DOWN
};

class ICommunicator
{
public:
    virtual int powerOn() = 0;
    virtual int getStatus() = 0;
    virtual int connect() = 0;
    virtual int disconnect() = 0;
    virtual int HTTPGet(const char *url) = 0;
    int connectUDP(const char *ip, const char *port);
    int sendUDP(const uint8_t *data);
    int prepUDP(size_t length);
    int disconnectUDP();
    virtual ~ICommunicator() = default;
};

class DummyComms : public ICommunicator
{
public:
    DummyComms(ILogger &logger);
    int powerOn();
    int getStatus();
    int connect();
    int disconnect();
    int HTTPGet(const char *url);
    int connectUDP(const char *ip, const char *port);
    int sendUDP(const uint8_t *data);
    int prepUDP(size_t length);
    int disconnectUDP();

private:
    ILogger &logger;
    int status = OFFLINE;
    size_t udpLength;
};