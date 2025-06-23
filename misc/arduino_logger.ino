#include "arduino_logger.h"

ArduinoLogger::ArduinoLogger(int autoBauds) : autoBauds{autoBauds} {}

void ArduinoLogger::begin(int baudRate)
{
    Serial.begin(baudRate);
    started = true;
}

void ArduinoLogger::log(const char *data)
{
    if (!started)
    {
        if (autoBauds == 0)
        {
            return;
        }
        begin(autoBauds);
    }

    Serial.print(data);
}

void ArduinoLogger::logln(const char *data)
{
    if (!started)
    {
        if (autoBauds == 0)
        {
            return;
        }
        begin(autoBauds);
    }
    Serial.println(data);
}