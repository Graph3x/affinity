#include <iostream>
#include <cassert>
#include "./pyro/pyro.h"
#include "./comms/comms.h"

void run_pyro(IPyroChannel *pyro){
    std::cout << pyro->get_status();
    std::cout << "\n";
    std::cout << pyro->blow();
    std::cout << "\n";
    std::cout << pyro->get_status();
    std::cout << "\n";
}

void run_comms(ICommunicator *comms){
    assert(!comms->has_data());
    std::string data = "Hello World!";
    comms->send_data(data);
}

int main()
{
    DummyChannel pyro = DummyChannel();
    run_pyro(&pyro);
    DummyComms comms = DummyComms();
    run_comms(&comms);
    return 0;
}