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
    float max_voltage = 0;
};

class PowerControl{
    public:
        PowerControl(int mcp_address, int voltage_control_pin):
        checkInterval(1),
        MCP(mcp_address),
        voltage_in_pin(voltage_control_pin),
        lastError(""){}
        
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
                if (!calibration.max_percentage){
                    setError("Error, not calibrated");
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
                if (!error){
                    log_info(debug_pc, "Error setting value: " + String(value));
                }
                setError("set percentage error overload:" + String(value));
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
                        log_debug(debug_pc, "changed " + String(percentage) + " by " + String(increment));
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
                //delay(3);
            }
            return res / 5;
        }

        float get_current_voltage(){
            auto k = ((double) CONF_VREF_VOLTAGE / (double) CONF_MAX_ADC) * CONF_VOLTAGE_DIVIDER_K;
            return (double) get_raw_value() * k;
        }

        bool ready(){
            return true;
        }

        void calibrate(){
            calibrating = true;
            calibrated = false;
            setPercentage(0);
            delay(2500);
            calibration.zero_raw = get_raw_value();
            auto raw = set_calibration_value(50);
            if (raw > (CONF_MAX_ADC / 2)){
                setError("Calibration failed due to half overload: " + String(raw));
                return;
            }
            raw = set_calibration_value(100);
            calibration.max_raw = raw;
            if (calibration.max_raw >= CONF_MAX_ADC){
                calibration.max_raw = CONF_MAX_ADC;
            }
            calibration.max_voltage = get_current_voltage();
            setPercentage(0);
            calibrating = false;
            log_info(debug_pc, "Calibration done (z:" + String(calibration.zero_raw) 
            + " m:" + String(calibration.max_raw) 
            + " mp:" + String(calibration.max_percentage) 
            + " mv: " + String(calibration.max_voltage)+ ")");
            calibrated = true;
        }

    private:
        float set_calibration_value(float value){
                setPercentage(value);
                calibration.max_percentage = value;
                delay(100);
                auto raw = get_raw_value();
                if (raw >= CONF_MAX_ADC - 10){
                    log_info(debug_pc, "Calibration done due to overload:" + String(value));
                    setPercentage(0);
                    delay(1000);
                    setError("Calibration error overload");
                    return 0;
                }
                return raw;
        }

        void setError(String errorToSet){
            lastError = errorToSet;
            error = true;
        }
        
        TimeInterval checkInterval;
        MCP4725 MCP;
        int voltage_in_pin;
        String lastError;
        bool initialized = false;
    public:
        PowerCalibration calibration;
        bool calibrated = false;
        bool calibrating = false;
        float percentage = 0;
        float target_voltage = 0;
        bool error = false;
};
