#pragma once
#include <iostream>

class ICommunicator {
   public:
        virtual int send_data(std::string data) = 0;
        virtual bool has_data() = 0;
        virtual int read_data() = 0;
};

class DummyComms: public ICommunicator{
    public:
        int send_data(std::string data);
        bool has_data();
        int read_data();

};