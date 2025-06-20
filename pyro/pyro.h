#pragma once
#include "../infrastructure/logging.h"

enum pyro
{
    READY,
    BLOWN,
    LOCKED
};

class IPyroChannel
{
public:
    virtual int getStatus() = 0;
    virtual int blow() = 0;

protected:
    virtual void setStatus(int new_status) = 0;
};

class DummyChannel : public IPyroChannel
{
public:
    DummyChannel(ILogger &logger);
    int getStatus();
    int blow();

protected:
    void setStatus(int new_status);

private:
    int status = 0;
    ILogger &logger;
};
