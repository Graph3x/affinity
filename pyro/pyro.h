#pragma once

class IPyroChannel
{
public:
    int get_status();
    void set_status(int new_status);
    virtual int blow();

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
