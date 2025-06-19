#pragma once
#include "comms.h"

class CommDummy: public ICommunicator{
    public:
        bool has_data();

};