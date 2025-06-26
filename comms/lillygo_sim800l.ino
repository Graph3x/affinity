#include "lillygo_sim800l.h"
#include <cstring>

// TODO actual solid system to recover from failiures.
void LillyGoSIM800L::CheckResponse(String response, String expected)
{
    if (response != expected)
    {
        if (commsDebug)
        {
            Serial.println(response);
        }
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

String LillyGoSIM800L::readLine()
{
    if (modem.available())
    {
        String line = modem.readStringUntil('\n');
        line.trim();
        return line;
    }

    return "";
}

String LillyGoSIM800L::waitForLine(long timeout)
{
    long waitEnd = millis() + timeout;
    while (millis() < waitEnd)
    {
        String line = readLine();
        if (line.length() > 0)
        {
            return line;
        }
    }
    return "";
}

int LillyGoSIM800L::powerOn()
{

    if (getStatus() != OFFLINE)
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
    CheckResponse(waitForLine(), "AT");
    CheckResponse(waitForLine());
    CheckResponse(waitForLine(), "+CPIN: READY");
    CheckResponse(waitForLine(), "Call Ready");
    CheckResponse(waitForLine(), "SMS Ready");

    modem.println("ATE0");
    CheckResponse(waitForLine(), "ATE0");
    CheckResponse(waitForLine());

    delay(5000);

    modem.println("AT+CGATT=1");
    CheckResponse(waitForLine());
    modem.println("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");
    CheckResponse(waitForLine());
    modem.println("AT+SAPBR=3,1,\"APN\",\"internet\"");
    CheckResponse(waitForLine());

    status = DISCONNECTED;
    return 0;
}

int LillyGoSIM800L::getStatus()
{
    if (status == COMMS_BUSY)
    {
        if (readLine() != "")
        {
            status = CONNECTED;
        }
    }

    if (status == PREPPING_UDP)
    {
        if (readLine() == ">")
        {
            status = UDP_READY;
        }
    }

    return status;
}

int LillyGoSIM800L::connect()
{
    if (getStatus() != DISCONNECTED)
    {
        Serial.println("$ COMMS: [ERROR] NOT DISCONNECTED");
        return 1;
    }

    modem.println("AT+SAPBR=1,1");
    CheckResponse(waitForLine());

    delay(3000);

    status = CONNECTED;
    return 0;
}

int LillyGoSIM800L::disconnect()
{

    if (getStatus() != CONNECTED)
    {
        Serial.println("$ COMMS: [ERROR] NOT CONNECTED");
        return 1;
    }

    modem.println("AT+CIPSHUT");
    CheckResponse(waitForLine(), "SHUT OK");

    modem.println("AT+SAPBR=0,1");
    CheckResponse(waitForLine());

    status = DISCONNECTED;
    return 0;
}

int LillyGoSIM800L::HTTPGet(const char *url)
{

    if (getStatus() != CONNECTED)
    {
        Serial.println("$ COMMS: [ERROR] NOT CONNECTED");
        return 1;
    }

    if (strlen(url) > 220)
    {
        return 1;
    }

    modem.println("AT+HTTPINIT");
    CheckResponse(waitForLine());

    modem.println("AT+HTTPPARA=\"CID\",1");
    CheckResponse(waitForLine());

    char httpPara[250];
    strcpy(httpPara, "AT+HTTPPARA=\"URL\",\"");
    strcat(httpPara, url);
    strcat(httpPara, "\"");

    modem.println(httpPara);
    CheckResponse(waitForLine());

    modem.println("AT+HTTPACTION=0");
    CheckResponse(waitForLine());
    String response = waitForLine().substring(15, 18);

    modem.println("AT+HTTPTERM");
    CheckResponse(waitForLine());
    return response.toInt();
}

int LillyGoSIM800L::connectUDP(const char *ip, const char *port)
{

    if (getStatus() != CONNECTED)
    {
        return 1;
    }

    if (strlen(ip) + strlen(port) > 220)
    {
        return 1;
    }

    modem.println("AT+CIPSHUT");
    CheckResponse(waitForLine(), "SHUT OK");

    modem.println("AT+CIPMUX=0");
    CheckResponse(waitForLine());

    modem.println("AT+CSTT=\"internet\",\"\",\"\"");
    CheckResponse(waitForLine());

    modem.println("AT+CIICR");
    CheckResponse(waitForLine());

    modem.println("AT+CIFSR");
    ipAddr = waitForLine();

    char connCommand[250];
    strcpy(connCommand, "AT+CIPSTART=\"UDP\",\"");
    strcat(connCommand, ip);
    strcat(connCommand, "\",\"");
    strcat(connCommand, port);
    strcat(connCommand, "\"");

    modem.println(connCommand);
    CheckResponse(waitForLine());
    CheckResponse(waitForLine(), "CONNECT OK");
    return 0;
}

int LillyGoSIM800L::disconnectUDP()
{
    modem.println("AT+CIPCLOSE");
    waitForLine();
    return 0;
}

int LillyGoSIM800L::prepUDP(size_t length)
{
    if (getStatus() != CONNECTED)
    {
        return 1;
    }

    modem.print("AT+CIPSEND=");
    modem.println(length);

    udpLength = length;
    status = PREPPING_UDP;
    return 0;
}

int LillyGoSIM800L::sendUDP(const uint8_t *data)
{
    if (getStatus() != UDP_READY)
    {
        return 1;
    }

    for (size_t i = 0; i < udpLength; ++i)
    {
        modem.write(data[i]);
    }

    modem.write(26);
    if (waitForLine(20) == "")
    {
        status = COMMS_BUSY;
    };
    status = CONNECTED;
    return 0;
}
