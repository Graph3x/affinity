#include "launch_pin.h"

#define BUZZER_PIN 15

LaunchPin::LaunchPin(int pin) : pin{pin} {}

void LaunchPin::init()
{
    if(status != UN_INITIALIZED){
        return;
    }

    pinMode(pin, INPUT_PULLDOWN);
    status = UNARMED;
}

char LaunchPin::getStatus()
{
    if(status == UN_INITIALIZED){
        init();
    }
    int currentValue = digitalRead(pin);

    cache[writer % 10] = currentValue;
    writer++;

    bool pinIn = false;
    for(int i= 0; i < 10; i++){
        if(cache[i] != 0){
            pinIn = true;
        }
    }

    if(status == UNARMED && pinIn){
        status = ARMED;
    }
    else if(status == ARMED && !pinIn){
        status = TRIGGERED;
        digitalWrite(BUZZER_PIN, HIGH);
    }

    return status;
}