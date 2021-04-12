#pragma once
#include <Arduino.h>

class TimeInterval{
    private:
        unsigned long old = 0;
        bool started = true;
        
    public:
        TimeInterval(long period):
        period(period){}
        unsigned long period;
        
        void reset(){
            old = millis();
        }

        void stop(){
            started = false;
        }

        void start(){
            started = true;
            old = millis();
        }

        bool poll(){
            if (!started){
                return false;
            }
            auto m = millis();
            if (m < old){
                old = m;
            }
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
        unsigned long delay_time;
        bool started = false;
        bool finished = false;
    public:
        TimeDelay(long delay_time):
        delay_time(delay_time){}
        
        void reset_and_start_if_not(){
            if (finished){
                reset_and_start();
            } else {
                start_if_not();
            }
        }

        void start_if_not(){
            if (!started){
                start();
            }
        }

        void reset_and_start(){
            reset();
            start();
        }
        void reset(){
            started = false;
            finished = false;
        }

        void start(){
            if (!finished){
                old = millis();
                started = true;
            }
        }

        void stop_and_reset(){
            started = false;
        }

        bool poll(){
            if (!started || finished){
                return false;
            }
            auto m = millis();
            if (m < old){
                old = m;
            }
            if (m - old > delay_time){
                finished = true;
                return true;
            }
            return false;
        }
};