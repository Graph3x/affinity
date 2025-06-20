#include "comms.h"
#include <HardwareSerial.h>
#include <iostream>

void PropagateError(String response)
{
    if (response != "OK")
    {
        Serial.println(response);
    }
}

int DummyComms::sendData(String data)
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

void LillyGoSIM800L::waitForResponse()
{
    long timeout = millis() + 5000;
    while (millis() < timeout)
    {
        while (modem.available())
        {
            String line = modem.readStringUntil('\n');
            line.trim();
            if (line.length() > 0)
            {
                Serial.println(">> " + line);
            }
        }
    }
}

String LillyGoSIM800L::waitForLine()
{
    long timeout = millis() + 5000;
    while (millis() < timeout)
    {
        if (modem.available())
        {
            String line = modem.readStringUntil('\n');
            line.trim();
            if (line.length() > 0)
            {
                return line;
            }
        }
    }
}

int LillyGoSIM800L::powerOn()
{
    pinMode(powerOnPin, OUTPUT);
    digitalWrite(powerOnPin, HIGH);

    pinMode(rstPin, OUTPUT);
    digitalWrite(rstPin, HIGH);

    pinMode(pwrkeyPin, OUTPUT);
    digitalWrite(pwrkeyPin, HIGH);
    delay(100);
    digitalWrite(pwrkeyPin, LOW);
    delay(1000);
    digitalWrite(pwrkeyPin, HIGH);
    delay(3000);

    modem.begin(baudRate, SERIAL_8N1, rxPin, txPin);
    modem.println("AT");
    waitForResponse();
    modem.println("ATE0");
    waitForResponse();

    delay(2000);

    modem.println("AT+CGATT=1");
    PropagateError(waitForLine());
    modem.println("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");
    PropagateError(waitForLine());
    modem.println("AT+SAPBR=3,1,\"APN\",\"internet\"");
    PropagateError(waitForLine());

    return 0;
}


int LillyGoSIM800L::getStatus()
{
    return 0; // TODO
}

int LillyGoSIM800L::connect()
{
    modem.println("AT+SAPBR=1,1");
    PropagateError(waitForLine());

    delay(3000);
    return 0;
}

int LillyGoSIM800L::disconnect()
{
    modem.println("AT+SAPBR=0,1");
    PropagateError(waitForLine());
    return 0;
}

// TODO -> UDP
int LillyGoSIM800L::sendData(String data)
{
    modem.println("AT+HTTPINIT");
    PropagateError(waitForLine());

    modem.println("AT+HTTPPARA=\"CID\",1");
    PropagateError(waitForLine());

    modem.println("AT+HTTPPARA=\"URL\",\"http://iaunmpihlkraveseldwgry0el6rux0g7k.oast.fun/?data=" + data + "\"");
    PropagateError(waitForLine());

    modem.println("AT+HTTPACTION=0");
    PropagateError(waitForLine());
    PropagateError(waitForLine());

    modem.println("AT+HTTPTERM");
    PropagateError(waitForLine());
    return 0;
}

int LillyGoSIM800L::asyncSendData(String data)
{
    return 0; // TODO
}