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