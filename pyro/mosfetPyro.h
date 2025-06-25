#pragma once
#include "pyro.h"

class SimpleMosfetChannel : public IPyroChannel
{
public:
    SimpleMosfetChannel(int pin);
    int getStatus();
    int blow();
    void lock();
    void unlock();

protected:
    void setStatus(int new_status);

private:
    int status = LOCKED;
    int pin = 0;
};