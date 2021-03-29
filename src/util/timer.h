#pragma once
#include <Arduino.h>

class TimeInterval{
    private:
        unsigned long old = 0;
        
    public:
        TimeInterval(long period):
        period(period){}
        unsigned long period;
        
        void reset(){
            old = millis();
        }

        bool poll(){
            auto m = millis();
            if (m - old > period){
                old = m;
                return true;
            }
            return false;
        }
};