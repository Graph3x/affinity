#include "millis_timer.h"

unsigned long MillisTimer::getTime()
{
    return millis();
}

unsigned long MillisTimer::timeSinceLaunch()
{
    if (!launched)
    {
        return 0;
    }

    return millis() - launchTime;
}

void MillisTimer::launch()
{
    if (!launched)
    {
        launchTime = millis();
        launched = true;
    }
}
