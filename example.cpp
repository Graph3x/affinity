#include <iostream>
#include <cassert>
#include "./pyro/pyro.h"
#include "./comms/comms.h"

void run_pyro(IPyroChannel *pyro){
    std::cout << pyro->getStatus();
    std::cout << "\n";
    std::cout << pyro->blow();
    std::cout << "\n";
    std::cout << pyro->getStatus();
    std::cout << "\n";
}

void run_comms(ICommunicator *comms){
    std::string data = "Hello World!";
    comms->sendData(data);
}

int main()
{
    DummyChannel pyro = DummyChannel();
    run_pyro(&pyro);
    LillyGoSIM800L comms = LillyGoSIM800L();
    run_comms(&comms);
    return 0;
}