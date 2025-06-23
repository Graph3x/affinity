#pragma once
#include "../misc/logging.h"

enum CommsStatus
{
    OFFLINE,
    DISCONNECTED,
    CONNECTED,
    COMMS_BUSY,
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

private:
    ILogger &logger;
    int status = OFFLINE;
};