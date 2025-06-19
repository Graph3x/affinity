#include "pyro.h"
#include <iostream>

enum {
    READY,
    BLOWN
};

int IPyroChannel::getStatus()
{
    return status;
}

void IPyroChannel::setStatus(int new_status)
{
    status = new_status;
}

int IPyroChannel::blow()
{
    return 1;
}

SimpleMosfetChannel::SimpleMosfetChannel(int pin) : pin{pin}
{
}

int DummyChannel::blow()
{
    if (getStatus() != READY)
    {
        return 1;
    }

    std::cout << "Pyro Ejection\n";
    setStatus(BLOWN);

    return 0;
}

int SimpleMosfetChannel::blow()
{
    if (getStatus() != READY)
    {
        return 1;
    }
    // TODO debug only !!!
    // pinMode(pin,OUTPUT);
    // digitalWrite(pin, HIGH);
    // TODO maybe close channel after time-out? => save channel blow time

    setStatus(BLOWN);

    return 0;
}