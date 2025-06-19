#pragma once

enum pyro
{
    READY,
    BLOWN,
    LOCKED
};

class IPyroChannel
{
public:
    int getStatus();
    virtual int blow();

protected:
    void setStatus(int new_status);

private:
    int status = 0;
};

class SimpleMosfetChannel : public IPyroChannel
{
public:
    SimpleMosfetChannel(int pin);
    int blow();

private:
    int pin = 0;
};

class DummyChannel : public IPyroChannel
{
public:
    int blow();
};
