#include "pyro.h"
#include "../misc/logging.h"

DummyChannel::DummyChannel(ILogger &logger) : logger{logger} {}

int DummyChannel::getStatus()
{
    return status;
}

void DummyChannel::setStatus(int new_status)
{
    status = new_status;
}

int DummyChannel::blow()
{
    if (getStatus() != pyro::READY)
    {
        logger.logln("$ PYRO: Invalid pyro blow");
        return -1;
    }

    logger.logln("$ PYRO: EJECT!");
    setStatus(BLOWN);

    return BLOWN;
}

void DummyChannel::lock()
{
    if (getStatus() == pyro::READY){
        setStatus(LOCKED);
    }
}

void DummyChannel::unlock()
{
    if (getStatus() == pyro::LOCKED){
        setStatus(READY);
    }
}