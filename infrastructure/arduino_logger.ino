#include "arduino_logger.h"

ArduinoLogger::ArduinoLogger(int autoBauds) : autoBauds{autoBauds} {}

void ArduinoLogger::begin(int baudRate)
{
    Serial.begin(baudRate);
}

void ArduinoLogger::log(const char *data)
{
    if (!started)
    {
        // TODO error returns
        if (autoBauds != 0)
        {
            begin(autoBauds);
        }
        started = true;
    }
    Serial.print(data);
}

void ArduinoLogger::logln(const char *data)
{
    if (!started)
    {
        // TODO error returns
        if (autoBauds != 0)
        {
            begin(autoBauds);
        }
        started = true;
    }
    Serial.println(data);
}