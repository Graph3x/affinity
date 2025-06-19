#pragma once
#include <iostream>

class ICommunicator
{
public:
    virtual int sendData(std::string data) = 0;
};

class DummyComms : public ICommunicator
{
public:
    int sendData(std::string data);
};

class LillyGoSIM800L : public ICommunicator
{
public:
    LillyGoSIM800L(int rst = 5, int pwrkey = 4, int powerOn = 23,
                   int tx = 27, int rx = 26, int baudRate = 115200);
    
    std::string waitForResponse();
    int powerOn();
    int getSignalStrength();
    int getStatus();
    int connect();
    int disconnect();
    int sendData(std::string data);
    int asyncSendData(std::string data);

private:
    int rstPin;
    int pwrkeyPin;
    int powerOnPin;
    int txPin;
    int rxPin;
    int baudRate;
    HardwareSerial modem(1);
};