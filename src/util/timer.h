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

class TimeDelay{
    private:
        unsigned long old = 0;
        unsigned long period;
        
    public:
        TimeDelay(long period):
        period(period){}
        
        void start_if_not(){
            if (!is_started()){
                start();
            }
        }

        void start(){
            old = millis();
        }

        void stop(){
            old = 0;
        }

        bool is_started(){
            return old != 0;
        }

        bool poll(){
            if (old == 0){
                return false;
            }
            auto m = millis();
            if (m - old > period){
                old = 0;
                return true;
            }
            return false;
        }
};