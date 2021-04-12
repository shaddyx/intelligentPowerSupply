#pragma once
#include <Wire.h>
#include <MCP4725.h>
#include "config.h"
#include "util/timer.h"
class PowerSensor {
    public:
        float voltage;
        PowerSensor(int voltage_in_pin):
        voltage_in_pin(voltage_in_pin){}
        void init(){
            pinMode(voltage_in_pin, INPUT);
        }

        float poll(){
            voltage = get_current_voltage();
            return voltage;
        }
        int get_raw_value(){
            long res = 0;
            for (int i=0; i<5; i++){
                res += analogRead(voltage_in_pin);
                //delay(3);
            }
            return res / 5;
        }
    private:
        float get_current_voltage(){
            auto k = ((double) CONF_VREF_VOLTAGE / (double) CONF_MAX_ADC) * CONF_VOLTAGE_DIVIDER_K;
            return (double) get_raw_value() * k;
        }
        int voltage_in_pin;
};
