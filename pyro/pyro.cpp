#include "pyro.h"
#include <iostream>

enum {
    READY,
    BLOWN
};

int IPyroChannel::get_status()
{
    return status;
}

void IPyroChannel::set_status(int new_status)
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
    if (get_status() != READY)
    {
        return 1;
    }

    std::cout << "Pyro Ejection\n";
    set_status(BLOWN);

    return 0;
}

int SimpleMosfetChannel::blow()
{
    if (get_status() != READY)
    {
        return 1;
    }
    // TODO debug only !!!
    // pinMode(pin,OUTPUT);
    // digitalWrite(pin, HIGH);

    set_status(BLOWN);

    return 0;
}