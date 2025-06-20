#include "mosfetPyro.h"

int SimpleMosfetChannel::getStatus()
{
    return status;
}

void SimpleMosfetChannel::setStatus(int new_status)
{
    status = new_status;
}

SimpleMosfetChannel::SimpleMosfetChannel(int pin) : pin{pin} {}

int SimpleMosfetChannel::blow()
{
    if (getStatus() != pyro::READY)
    {
        return 1;
    }

    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);

    // TODO maybe close channel after time-out? => save channel blow time
    // - same for dummy ig

    setStatus(pyro::BLOWN);

    return 0;
}