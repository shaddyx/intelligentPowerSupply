#pragma once
#include "power_control.h"
#include "current_sensor.h"
class PowerCurrentControl{
    public:
        float current;
        float voltage;
        bool overload;

        PowerCurrentControl(PowerControl * powerControl, CurrentSensor * currentSensor, PowerSensor * powerSensor):
        checkInterval(1),
        powerControl(powerControl),
        currentSensor(currentSensor),
        powerSensor(powerSensor)
        {
        }
        void init(){

        }
        void poll(){
            overload = false;
            if (!powerControl->calibrated){
                return;
            }
            if (checkInterval.poll()){
                if (currentSensor->current > current){
                     auto increment = currentSensor->current - current;
                     powerControl->target_voltage -= increment / 5 * CONF_CURRENT_DIFF_VOLTAGE_INCREMENT_K;
                     overload = true;
                } else if (currentSensor->current < current && powerControl->target_voltage < voltage)
                {
                    auto increment = voltage - powerControl->target_voltage;
                    powerControl->target_voltage += increment / 5;
                }
                if (powerControl->target_voltage < 0){
                    powerControl->target_voltage = 0;
                }
                if (powerControl->target_voltage > voltage){
                    powerControl->target_voltage = voltage;
                }
            }    
        }
        
    private:
        TimeInterval checkInterval;
        PowerControl * powerControl;
        CurrentSensor * currentSensor;
        PowerSensor * powerSensor;
};