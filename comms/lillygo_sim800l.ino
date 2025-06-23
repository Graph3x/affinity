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

    if (strlen(url) > 220)
    {
        return 1;
    }

    modem.println("AT+HTTPINIT");
    PropagateError(waitForLine());

    modem.println("AT+HTTPPARA=\"CID\",1");
    PropagateError(waitForLine());

    char httpPara[250];
    strcpy(httpPara, "AT+HTTPPARA=\"URL\",\"");
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

// TODO
int LillyGoSIM800L::connectUDP(const char *ip, const char *port)
{

    if (strlen(ip) + strlen(port) > 220)
    {
        return 1;
    }

    modem.println("AT+CIPSHUT");
    waitForResponse();

    modem.println("AT+CIPMUX=0");
    waitForResponse();

    modem.println("AT+CSTT=\"internet\",\"\",\"\"");
    waitForResponse();

    modem.println("AT+CIICR");
    waitForResponse();

    modem.println("AT+CIFSR");
    waitForResponse();

    char connCommand[250];
    strcpy(connCommand, "AT+CIPSTART=\"UDP\",\"");
    strcat(connCommand, ip);
    strcat(connCommand, "\",\"");
    strcat(connCommand, port);
    strcat(connCommand, "\"");

    modem.println(connCommand);
    waitForResponse();

    return 0;
}

int LillyGoSIM800L::sendUDP(const uint8_t *data, size_t length)
{
    modem.print("AT+CIPSEND=");
    modem.println(length);
    waitForResponse();

    for (size_t i = 0; i < length; ++i)
    {
        modem.write(data[i]);
    }

    modem.write(26);
    waitForResponse();

    return 0;
}
