#include "timer.h"

class MillisTimer: public ITimer
{
public:
    unsigned long getTime();
    unsigned long timeSinceLaunch();
    void launch();

private:
    unsigned long launchTime = 0;
    bool launched = false;
};