#pragma once
#include "encoder/encoder.h"
#include "display/liquid_crystal_display2x2.h"
#include "debug.h"
Debug displayparam_debug("DisplayParam");
template <class T>
class DisplayParam {
    public:
        bool needRefresh = true;
        DisplayParam(String caption, Display * display, Encoder * encoder, T current, T minValue, T maxValue, T increment, T fastIncrement):
        caption(caption),
        display(display),
        encoder(encoder),
        current(current),
        minValue(minValue),
        maxValue(maxValue),
        increment(increment),
        fastIncrement(fastIncrement)
        {}
        
        T get_inc(){
            T res = increment;
            if (millis() - last_ms < 200){
                res = fastIncrement;
            } 
            last_ms = millis();
            return res;
        }

        void poll(){
            if (encoder->is_left()){
                current += get_inc();
                displayparam_debug.info("Increment");
                if (current > maxValue){
                    current = maxValue;
                }
                needRefresh = true;
            } else if (encoder->is_right()){
                displayparam_debug.info("Decrement");
                current -= get_inc();
                if (current < minValue){
                    current = minValue;
                }
                needRefresh = true;
            }
            
            update();
        }

        void update(){
            if (needRefresh){
                display->clear();
                display->printLine(0, caption + ": " + String (current));
                needRefresh = false;
            }
        }
        
    private:
        String caption;
        Display * display;
        Encoder * encoder;
    public:
        T current;
    private:
        T minValue;
        T maxValue;
        T increment;
        T fastIncrement;
        unsigned long last_ms = 0;
};