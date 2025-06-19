#include "comms.h"
#include <iostream>


bool DummyComms::has_data(){
    return false;
}

int DummyComms::send_data(std::string data){
    std::cout << "->" << data << std::endl;
    return 0;
}


int DummyComms::read_data(){
    return 0;
}