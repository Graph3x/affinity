#pragma once

class ILogger
{
public:
    virtual void log(const char *data) = 0;
    virtual void logln(const char *data) = 0;
    virtual ~ILogger() = default;
};

class PrintLogger : public ILogger
{
public:
    void log(const char *data);
    void logln(const char *data);
};

class DummyLogger : public ILogger
{
public:
    void log(const char *data);
    void logln(const char *data);
};