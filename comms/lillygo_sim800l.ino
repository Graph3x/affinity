#include "lillygo_sim800l.h"
#include <cstring>

 // TODO -> logger / handle fails
void PropagateError(String response)
{
    if (response != "OK")
    {
        Serial.println(response);
    }
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

// TODO delays
int LillyGoSIM800L::powerOn()
{

    if (status != OFFLINE && status != DOWN)
    {
        Serial.println("$ COMMS: [ERROR] NOT OFFLINE");
        return 1;
    }

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

    status = DISCONNECTED;
    return 0;
}

int LillyGoSIM800L::getStatus()
{
    return status;
}

int LillyGoSIM800L::connect()
{
    if (status != DISCONNECTED)
    {
        Serial.println("$ COMMS: [ERROR] NOT DISCONNECTED");
        return 1;
    }

    modem.println("AT+SAPBR=1,1");
    PropagateError(waitForLine());

    delay(3000);

    status = CONNECTED;
    return 0;
}

int LillyGoSIM800L::disconnect()
{

    if (status != CONNECTED)
    {
        Serial.println("$ COMMS: [ERROR] NOT CONNECTED");
        return 1;
    }

    modem.println("AT+SAPBR=0,1");
    PropagateError(waitForLine());
    
    status = DISCONNECTED;
    return 0;
}

int LillyGoSIM800L::HTTPGet(const char *url)
{

    if (status != CONNECTED)
    {
        Serial.println("$ COMMS: [ERROR] NOT CONNECTED");
        return 1;
    }

    modem.println("AT+HTTPINIT");
    PropagateError(waitForLine());

    modem.println("AT+HTTPPARA=\"CID\",1");
    PropagateError(waitForLine());

    char httpPara[220];
    strcpy(httpPara, "AT+HTTPPARA=\"URL\",\"");

    if (strlen(url) > 220)
    {
        return 1;
    }

    strcat(httpPara, url);
    strcat(httpPara, "\"");

    modem.println(httpPara);
    PropagateError(waitForLine());

    // TODO return status code from response
    modem.println("AT+HTTPACTION=0");
    PropagateError(waitForLine());
    PropagateError(waitForLine());

    modem.println("AT+HTTPTERM");
    PropagateError(waitForLine());
    return 0;
}

// TODO UDP