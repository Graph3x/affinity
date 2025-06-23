#include "logging.h"
#include <iostream>

void PrintLogger::log(const char *data)
{
    std::cout << data;
}

void PrintLogger::logln(const char *data)
{
    std::cout << data << std::endl;
}

void DummyLogger::log(const char *data)
{
    (void)data;
}
void DummyLogger::logln(const char *data)
{
    (void)data;
}