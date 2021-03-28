#pragma once
#include <Wire.h>
#include <MCP4725.h>
#include "config.h"
Debug power_control_debug("PowerControl");
class PowerControl{
    public:
        PowerControl():
        MCP(0x60){}
        float percentage = 0;
        void poll(){
            if (!initialized){
                MCP.begin();
                initialized = true;
            }
            auto result = MCP.setPercentage(percentage);
            power_control_debug.info("Power control[" + String(percentage) + "]: " + String(result));
        }
    private:
        MCP4725 MCP;
        bool initialized = false;

};
