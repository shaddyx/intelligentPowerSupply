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
            setPercentage(percentage);
            //power_control_debug.info("Power control[" + String(percentage) + "]: " + String(result));
        }

        int get_raw_value(){
            return analogRead(voltage_in_pin);
        }

        float get_current_voltage(){
            return get_raw_value();
        }

        bool ready(){
            return true;
        }

        void calibrate(){
            calibrating = true;
            setPercentage(0);
            delay(500);
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
            calibrating = false;
            power_control_debug.info("Calibration done (z:" + String(calibration.zero_raw) + " m:" + String(calibration.max_raw) + " mp:" + String(calibration.max_percentage) + ")");
        }

    private:
        MCP4725 MCP;
        int voltage_in_pin;
        float max_voltage;
        bool initialized = false;
    public:
        PowerCalibration calibration;
        bool calibrated = false;
        bool calibrating = false;
        float percentage = 0;
        float target_voltage = 0;
        bool error = false;
};
