#pragma once
#include <Wire.h>
#include <MCP4725.h>
#include "config.h"
#include "util/timer.h"
struct PowerCalibration{
    float max_percentage = 0;
    int zero_raw;
    int max_raw;
};

Debug power_control_debug("PowerControl");
class PowerControl{
    public:
        PowerControl(int mcp_address, int voltage_control_pin, float max_voltage):
        checkInterval(10),
        MCP(mcp_address),
        voltage_in_pin(voltage_control_pin),
        max_voltage(max_voltage){}
        
        void init(){
            if (!initialized){
                power_control_debug.info("Initializing mcp");
                MCP.begin();
                pinMode(voltage_in_pin, INPUT);
                initialized = true;
            }
        }
        void setPercentage(float value){
            init();
            if (!calibrating){
                if (value >= calibration.max_percentage){
                    value = calibration.max_percentage;
                }
            }
            auto result = MCP.setPercentage(value);
            if (!result){
                error = true;
            }
        }

        void poll(){
            init();
            if (!calibrating){
                if (checkInterval.poll()){
                    auto current = get_current_voltage();
                    if (current <= CONF_MIN_VOLTAGE){
                        return;
                    }
                    auto increment = target_voltage - current;
                    increment = increment / 5;
                    percentage += increment;
                    if (abs(increment) > 0.1){
                        power_control_debug.info("changed " + String(percentage) + " by " + String(increment) );
                        if (percentage > 100){
                            percentage = 100;
                        }
                        if (percentage < 0){
                            percentage = 0;
                        }
                    }
                    
                    
                }
                setPercentage(percentage);
            }
            
            //power_control_debug.info("Power control[" + String(percentage) + "]: " + String(result));
        }

        int get_raw_value(){
            long res = 0;
            for (int i=0; i<5; i++){
                res += analogRead(voltage_in_pin);
            }
            return res / 5;
        }

        float get_current_voltage(){
            auto k = (float) CONF_MIN_VOLTAGE / calibration.zero_raw;
            return get_raw_value() * k;
        }

        bool ready(){
            return true;
        }

        void calibrate(){
            calibrating = true;
            setPercentage(0);
            delay(2500);
            calibration.zero_raw = get_raw_value();
            for (float i=0; i<=100; i+=0.5){
                setPercentage(i);
                calibration.max_percentage = i;
                auto raw = get_raw_value();
                if (raw >= CONF_MAX_ADC - 10){
                    power_control_debug.info("Calibration done due to overload:" + String(i));
                    break;
                }
                delay(50);
            }
            calibration.max_raw = get_raw_value();
            if (calibration.max_raw >= CONF_MAX_ADC - 10){
                calibration.max_raw = CONF_MAX_ADC - 10;
            }
            setPercentage(0);
            calibrating = false;
            power_control_debug.info("Calibration done (z:" + String(calibration.zero_raw) + " m:" + String(calibration.max_raw) + " mp:" + String(calibration.max_percentage) + ")");
        }

    private:
        MCP4725 MCP;
        int voltage_in_pin;
        float max_voltage;
        bool initialized = false;
        TimeInterval checkInterval;
    public:
        PowerCalibration calibration;
        bool calibrated = false;
        bool calibrating = false;
        float percentage = 0;
        float target_voltage = 0;
        bool error = false;
        
};
