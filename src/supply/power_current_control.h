#pragma once
#include "power_control.h"
#include "current_sensor.h"
class PowerCurrentControl{
    public:
        float current;
        float voltage;
        
        PowerCurrentControl(PowerControl * powerControl, CurrentSensor * currentSensor):
        powerControl(powerControl),
        currentSensor(currentSensor)
        {
        }

        void poll(){

        }
    private:
        PowerControl * powerControl;
        CurrentSensor * currentSensor;
};