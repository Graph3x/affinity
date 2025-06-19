#include <iostream>
#include "./pyro/pyro.h"

int main()
{

    DummyChannel pyro = DummyChannel();
    std::cout << pyro.get_status();
    std::cout << "\n";
    std::cout << pyro.blow();
    std::cout << "\n";
    std::cout << pyro.get_status();
    std::cout << "\n";

    return 0;
}