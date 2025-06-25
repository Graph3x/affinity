#pragma once
#include "timer.h"

class MillisTimer: public ITimer
{
public:
    unsigned long getTime();
    unsigned long timeSinceLaunch();
    void launch();
    void launch();
    void blowPyro();

private:
    unsigned long launchTime = 0;
    unsigned long pyroTime = 0;
    bool launched = false;
};