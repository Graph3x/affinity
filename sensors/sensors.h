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