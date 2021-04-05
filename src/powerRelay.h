#pragma once
#include <Arduino.h>

class PowerRelay{
    public:
        bool on = false;
        PowerRelay(uint8_t pin):
        pin(pin)
        {

        }
        void init(){
            if (!initialized){
                initialized = true;
                pinMode(pin, OUTPUT);
            }
            poll();
        }
        void poll(){
            digitalWrite(pin, !on);
        }
    private: 
        uint8_t pin;
        bool initialized = false;
};