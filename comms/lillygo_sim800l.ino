#include "lillygo_sim800l.h"
#include <cstring>

// TODO -> logger + handle fails
void PropagateError(String response, String expected = "OK")
{
    if (response != expected)
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

// TODO lower delays?
int LillyGoSIM800L::powerOn()
{

    if (getStatus() != OFFLINE && getStatus() != DOWN)
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
    if(status == COMMS_BUSY){
        if(readLine() != ""){
            status = CONNECTED;
        }
    }

    if(status == PREPPING_UDP){
        if(readLine() == ">"){
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
    PropagateError(waitForLine());

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
    PropagateError(waitForLine());

    modem.println("AT+SAPBR=0,1");
    PropagateError(waitForLine());

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
    PropagateError(waitForLine());

    modem.println("AT+CIPMUX=0");
    PropagateError(waitForLine());

    modem.println("AT+CSTT=\"internet\",\"\",\"\"");
    PropagateError(waitForLine());

    modem.println("AT+CIICR");
    PropagateError(waitForLine());

    modem.println("AT+CIFSR");
    PropagateError(waitForLine());

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

int LillyGoSIM800L::disconnectUDP()
{
    modem.println("AT+CIPCLOSE");
    waitForLine();
    return 0;
}

int LillyGoSIM800L::prepUDP(size_t length)
{
    if(getStatus() != CONNECTED){
        return 1;
    }

    modem.print("AT+CIPSEND=");
    modem.println(length);
    status = PREPPING_UDP;
    return 0;
}

int LillyGoSIM800L::sendUDP(const uint8_t *data, size_t length)
{
    if(getStatus() != UDP_READY){
        return 1;
    }

    for (size_t i = 0; i < length; ++i)
    {
        modem.write(data[i]);
    }

    modem.write(26);
    if(waitForLine(20) == ""){
        status = COMMS_BUSY;
    };
    status = CONNECTED;
    return 0;
}
