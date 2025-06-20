#pragma once
#include <iostream>
#include <HardwareSerial.h>

class ICommunicator
{
public:
    virtual int sendData(String data) = 0;
    virtual int powerOn();
    virtual int getStatus();
    virtual int connect();
    virtual int disconnect();
    virtual int sendData(String data);
    virtual int asyncSendData(String data); 
};

class DummyComms : public ICommunicator
{
public:
    int sendData(String data);
};

class LillyGoSIM800L : public ICommunicator
{
public:
    LillyGoSIM800L(int rst = 5, int pwrkey = 4, int powerOn = 23,
                   int tx = 27, int rx = 26, int baudRate = 115200);

    void waitForResponse();
    String waitForLine();
    int powerOn();
    int getStatus();
    int connect();
    int disconnect();
    int sendData(String data);
    int asyncSendData(String data);

private:
    int rstPin;
    int pwrkeyPin;
    int powerOnPin;
    int txPin;
    int rxPin;
    int baudRate;
    HardwareSerial modem = HardwareSerial(1);
};