class ITimer
{
public:
    virtual unsigned long getTime() = 0;
    virtual unsigned long timeSinceLaunch() = 0;
    virtual void launch() = 0;
};

class DummyTimer: public ITimer
{
public:
    unsigned long getTime();
    unsigned long timeSinceLaunch();
    void launch();

private:
    unsigned long currentVal = 0;
    unsigned long launchTime = 0;
    bool launched = false;
};