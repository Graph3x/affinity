#pragma once
class ISensor
{
public:
    virtual int readValue() = 0;
    virtual ~ISensor() = default;
};

class DummyArraySensor : public ISensor
{
public:
    DummyArraySensor(int *data, int dataSize);
    int readValue();

private:
    int *data;
    int dataSize;
    int current = -1;
};

class IBarometer
{
public:
    virtual float readTemperature() = 0;
    virtual float readPressure() = 0;
    virtual float readHeight() = 0;
    virtual ~IBarometer() = default;
};

struct AccelData {
    double x;
    double y;
    double z;
};

class IAccelerometer
{
public:
    virtual struct AccelData getAccel() = 0;
    virtual ~IAccelerometer() = default;
};