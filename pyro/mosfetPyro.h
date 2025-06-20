#include "pyro.h"

class SimpleMosfetChannel : public IPyroChannel
{
public:
    SimpleMosfetChannel(int pin);
    int getStatus();
    int blow();

protected:
    void setStatus(int new_status);

private:
    int status = 0;
    int pin = 0;
};