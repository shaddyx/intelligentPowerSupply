#pragma once
#include <Arduino.h>
#include "config.h"
static bool _Debug_initialized = false;
class Debug{
    public:
        Debug(String name):
        name(name)
        {}
        void init(){
            if (!_Debug_initialized){
                Serial.begin(CONFIG_SERIAL_SPEED);
                _Debug_initialized = true;
                Serial.println("Initialized debug");
            }
        }

        void info(String value){
            print(value);
        }

        void debug(String value){
            #if CONFIG_DEBUG_LEVEL == DEBUG
                print(value);
            #endif
        }

    private: 
        String name;
        String format(String value){
            return "[" + name + "] " + value; 
        }

        void print(String value){
            init();
            Serial.println(format(value));
        }
        
};