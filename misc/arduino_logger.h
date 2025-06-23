#pragma once
#include "logging.h"

class ArduinoLogger : public ILogger
{
public:
    ArduinoLogger(int autoBauds = 0);
    void begin(int baudRate);
    void log(const char *data);
    void logln(const char *data);

private:
    bool started;
    int autoBauds;
};