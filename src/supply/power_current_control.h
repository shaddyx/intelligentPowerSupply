#pragma once
#include "power_control.h"
#include "current_sensor.h"
#include "util/timer.h"
class PowerCurrentControl{
    public:
        float current;
        float voltage;
        bool overload;
        bool can_increment = false;

        PowerCurrentControl(PowerControl * powerControl, CurrentSensor * currentSensor, PowerSensor * powerSensor):
        checkInterval(1),
        powerControl(powerControl),
        currentSensor(currentSensor),
        powerSensor(powerSensor),
        timeToIncrement(500)
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
                     timeToIncrement.reset();
                     can_increment = false;
                } else if (currentSensor->current < current && powerControl->target_voltage < voltage)
                {
                    timeToIncrement.start_if_not();
                    if (timeToIncrement.poll()){
                        can_increment = true;
                    };
                    if (can_increment){
                        auto increment = voltage - powerControl->target_voltage;
                        powerControl->target_voltage += increment / 5;
                    }
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
        TimeDelay timeToIncrement;
};