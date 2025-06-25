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

    setStatus(pyro::BLOWN);

    return 0;
}

void SimpleMosfetChannel::lock()
{
    if (getStatus() == pyro::READY){
        setStatus(LOCKED);
    }
}

void SimpleMosfetChannel::unlock()
{
    if (getStatus() == pyro::LOCKED){
        setStatus(READY);
    }
}

void SimpleMosfetChannel::off()
{
    if (getStatus() == pyro::BLOWN)
    {
        digitalWrite(pin, HIGH);
    }

    status = OFF;

}