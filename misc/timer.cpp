#include "timer.h"
#include <assert.h>

unsigned long DummyTimer::getTime()
{
    return currentVal++;
}

unsigned long DummyTimer::timeSinceLaunch()
{
    if (!launched)
    {
        return 0;
    }

    return currentVal - launchTime;
}

void DummyTimer::launch()
{
    assert(!launched);

    launchTime = currentVal;
    launched = true;
}

unsigned long DummyTimer::timeSincePyro()
{
    return currentVal - pyroTime;
}

void DummyTimer::blowPyro()
{
    assert(pyroTime == 0);
    pyroTime = currentVal;
}