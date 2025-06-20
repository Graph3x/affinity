#include "arduino_dummies.h"
#include <chrono>
#include <thread>
#include <iostream>

void delay(unsigned long ms){
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}


void pinMode(int pin, int mode){
    std::cout << "Set pin ";
    std::cout << pin;
    std::cout << " to mode ";
    std::cout << mode;
    std::cout << "\n";
}

void digitalWrite(int pin, int value){
    std::cout << "Wrote pin ";
    std::cout << pin;
    std::cout << " to value ";
    std::cout << value;
    std::cout << "\n";
}