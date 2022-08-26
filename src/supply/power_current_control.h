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
        timeToStartIncrement(500),
        timeToIncrement(3)
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
                     auto decrement = currentSensor->current - current;
                     decrement = decrement / 5 * CONF_CURRENT_DIFF_VOLTAGE_INCREMENT_K;
                     if (decrement < 0.1){
                         decrement = 0.1;
                     }
                     powerControl->target_voltage -= decrement;
                     overload = true;
                     timeToStartIncrement.reset();
                     can_increment = false;
                } else if (currentSensor->current < current && powerControl->target_voltage < voltage)
                {
                    timeToStartIncrement.start_if_not();
                    if (timeToStartIncrement.poll()){
                        can_increment = true;
                    };
                    if (can_increment && timeToIncrement.poll()){
                        powerControl->target_voltage += 0.1;
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
        TimeDelay timeToStartIncrement;
        TimeInterval timeToIncrement;
};