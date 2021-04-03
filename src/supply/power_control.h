#pragma once
#include <Wire.h>
#include <MCP4725.h>
#include "config.h"
#include "util/timer.h"
DebugModule(debug_pc, "PowerControl");
struct PowerCalibration{
    float max_percentage = 0;
    int zero_raw;
    int max_raw;
};

class PowerControl{
    public:
        PowerControl(int mcp_address, int voltage_control_pin, float max_voltage):
        checkInterval(10),
        MCP(mcp_address),
        voltage_in_pin(voltage_control_pin),
        lastError(""),
        max_voltage(max_voltage){}
        
        void init(){
            if (!initialized){
                log_info(debug_pc, "Initializing mcp");
                MCP.begin();
                pinMode(voltage_in_pin, INPUT);
                initialized = true;
            }
        }
        void setPercentage(float value){
            init();
            if (!calibrating){
                if (error){
                    log_info(debug_pc, "Error: " + lastError);
                    return;
                }
                if (value >= calibration.max_percentage){
                    value = calibration.max_percentage;
                }
            }
            if (value < 0){
                value = 0;
            }
            auto result = MCP.setPercentage(value);
            if (result){
                setError("set percentage error overload:" + String(value));
                error = true;
            }
        }

        void poll(){
            init();
            if (!calibrating){
                if (checkInterval.poll()){
                    auto current = get_current_voltage();
                    auto increment = target_voltage - current;
                    increment = increment / 5;
                    percentage += increment;
                    if (abs(increment) > 0.05){
                        log_info(debug_pc, "changed " + String(percentage) + " by " + String(increment));
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
            //log_info("Power control[" + String(percentage) + "]: " + String(result));
        }

        int get_raw_value(){
            long res = 0;
            for (int i=0; i<5; i++){
                res += analogRead(voltage_in_pin);
            }
            return res / 5;
        }

        float get_current_voltage(){
            auto k = (float) CONF_MAX_VOLTAGE / calibration.max_raw;
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
                    log_info(debug_pc, "Calibration done due to overload:" + String(i));
                    delay(4000);
                    setError("Calibration error overload");
                }
                delay(50);
            }
            calibration.max_raw = get_raw_value();
            if (calibration.max_raw >= CONF_MAX_ADC){
                calibration.max_raw = CONF_MAX_ADC;
            }
            setPercentage(0);
            calibrating = false;
            log_info(debug_pc, "Calibration done (z:" + String(calibration.zero_raw) + " m:" + String(calibration.max_raw) + " mp:" + String(calibration.max_percentage) + ")");
        }

    private:
        void setError(String errorToSet){
            lastError = errorToSet;
            error = true;
        }
        
        TimeInterval checkInterval;
        MCP4725 MCP;
        int voltage_in_pin;
        String lastError;
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
