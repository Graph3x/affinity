#pragma once
#include "../misc/logging.h"

enum pyro
{
    READY,
    BLOWN,
    LOCKED,
    OFF
};

class IPyroChannel
{
public:
    virtual int getStatus() = 0;
    virtual int blow() = 0;
    virtual void lock() = 0;
    virtual void unlock() = 0;
    virtual void off() = 0;
    virtual ~IPyroChannel() = default;

protected:
    virtual void setStatus(int new_status) = 0;
};

class DummyChannel : public IPyroChannel
{
public:
    DummyChannel(ILogger &logger);
    int getStatus();
    int blow();
    void lock();
    void unlock();
    void off();

protected:
    void setStatus(int new_status);

private:
    int status = LOCKED;
    ILogger &logger;
};
