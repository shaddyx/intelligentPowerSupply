#pragma once
#include "config.h"
#include "SerialDebug.h" 
void _none(){

}
#if CONFIG_DEBUG_ENABLED == 1
    #include <Arduino.h>
    static bool _Debug_initialized = false;
    void _log_info(char * v, String c){
        if (!_Debug_initialized){
            Serial.begin(CONFIG_SERIAL_SPEED);
            _Debug_initialized = true;
        }
        Serial.println("[" + String(v) + "] " + c);
    }
    #define log_info(v, c) _log_info(v, c)
    //#define log_debug(v, c) _log_info(v, c)
    #define log_debug(v, c) _none()
    #define DebugModule(n, v) char n[] = v
#else
    #define log_info(v, c) _none()
    #define log_debug(v, c) _none()
    #define DebugModule(n, v) 
#endif