#pragma once
#include <Arduino.h>
#include "config.h"
class CurrentSensor{
    public:
        CurrentSensor(uint8_t pin, float acs_offset):
        pin(pin),
        acs_offset(acs_offset)
        {}

        void init(){
            if (!initialized){
                initialized = true;
                pinMode(pin, INPUT);
            }
        };

        float get_current(){
            init();
            auto k = ((double) CONF_VREF_VOLTAGE / (double) CONF_MAX_ADC) * CONF_CURRENT_DIVIDER_K;
            auto value = analogRead(pin);
            Serial.println("Current: " + String(value));
            float res = ((double) value * k - acs_offset) * 10;
            if(res < 0){
                res = 0;
            }
            return res;
        }

    private:
        uint8_t pin;
        float acs_offset;
        bool initialized = false;
};