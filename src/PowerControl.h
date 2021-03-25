#pragma once
#include <Arduino.h>
#include <Wire.h>
#include <SoftwareWire.h>
#include <Adafruit_MCP4725.h>

#define MAX_VAL 4096

class PowerControl{
    public:
        PowerControl(uint8_t sda, uint8_t scl, uint8_t addr = 0x60){
            this -> sda = sda;
            this -> scl = scl;
            this-> addr = addr;
            this -> started = false;
        }
        void setVoltage(uint16_t value){
            if (!started){
                dac.begin(this -> addr);
                started = true;
            }
            dac.setVoltage(value, false);
        }
    private:
        Adafruit_MCP4725 dac;
        uint8_t sda;
        uint8_t scl;
        uint8_t addr;
        bool started;
};