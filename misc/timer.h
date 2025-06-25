#pragma once

class ITimer
{
public:
    virtual unsigned long getTime() = 0;
    virtual unsigned long timeSinceLaunch() = 0;
    virtual unsigned long timeSincePyro() = 0;
    virtual void launch() = 0;
    virtual void blowPyro() = 0;
    virtual ~ITimer() = default;
};

class DummyTimer: public ITimer
{
public:
    unsigned long getTime();
    unsigned long timeSinceLaunch();
    unsigned long timeSincePyro();
    void launch();
    void blowPyro();

private:
    unsigned long currentVal = 0;
    unsigned long launchTime = 0;
    unsigned long pyroTime = 0;
    bool launched = false;
};