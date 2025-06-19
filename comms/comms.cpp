#include "comms.h"
#include <iostream>

int DummyComms::sendData(std::string data)
{
    std::cout << "->" << data << std::endl;
    return 0;
}

LillyGoSIM800L::LillyGoSIM800L(int rst, int pwrkey, int powerOn,
                               int tx, int rx, int baudRate)
    : rstPin{rst},
      pwrkeyPin{pwrkey},
      powerOnPin{powerOn},
      txPin{tx},
      rxPin{rx},
      baudRate{baudRate}
{
}

std::string LillyGoSIM800L::waitForResponse()
{
    long timeout = millis() + 5000;
    while (millis() < timeout)
    {
        if (modem.available())
        {
            std::string line = modem.readStringUntil('\n');
            line.trim();
            if (line.length() > 0)
            {
                Serial.println(">> " + line);
            }
            break;
        }
    }
}

int LillyGoSIM800L::powerOn()
{
    pinMode(powerOnPin, OUTPUT);
    digitalWrite(powerOnPin, HIGH);

    pinMode(rst_pin, OUTPUT);
    digitalWrite(rstPin, HIGH);

    pinMode(pwrkey_pin, OUTPUT);
    digitalWrite(pwrkeyPin, HIGH);
    delay(100);
    digitalWrite(pwrkeyPin, LOW);
    delay(1000);
    digitalWrite(pwrkeyPin, HIGH);
    delay(2000);
    modem.begin(baudRate, SERIAL_8N1, rxPin, txPin);
    modem.println("AT");
    waitForResponse(); // TODO error handling
}

int LillyGoSIM800L::getSignalStrength()
{
    return 0; // TODO
}

int LillyGoSIM800L::getStatus()
{
    return modem.available();
}

int LillyGoSIM800L::connect()
{
    modem.println("AT+CGATT=1");
    waitForResponse();
    modem.println("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");
    waitForResponse();
    modem.println("AT+SAPBR=3,1,\"APN\",\"internet\"");
    waitForResponse();
    modem.println("AT+SAPBR=1,1");
    waitForResponse();
}

int LillyGoSIM800L::disconnect()
{
    return 0; // TODO
}

int LillyGoSIM800L::sendData(std::string data)
{
    //TODO -> UDP
    modem.println("AT+HTTPINIT");
    waitForResponse();
    modem.println("AT+HTTPPARA=\"CID\",1");
    waitForResponse();
    modem.println("AT+HTTPPARA=\"URL\",\"http://iaunmpihlkraveseldwgry0el6rux0g7k.oast.fun\"");
    waitForResponse();
    modem.println("AT+HTTPACTION=0");
    waitForResponse();
    modem.println("AT+HTTPTERM");
    waitForResponse();
    return 0;
}

int LillyGoSIM800L::asyncSendData(std::string data)
{
    return 0; // TODO
}