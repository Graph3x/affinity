#pragma once
class ISensor{
    public:
        virtual int readValue() = 0;
        virtual ~ISensor() = default;
};

class DummyGrowingSensor : public ISensor{
    public:
        DummyGrowingSensor(int growthSpeed = 1, int initValue = 1);
        int readValue();
    private:
        int growthSpeed;
        int value;

};