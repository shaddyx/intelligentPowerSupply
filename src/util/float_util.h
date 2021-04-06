#pragma once
#include <Arduino.h>

class FloatUtil{
    public:
        static float round2(float a){
            return roundf((double)a * 100) / 100;
        }
        static String floatToString(float value, uint8_t width, uint8_t precision){
            char buffer[20];
            dtostrf(value, width - 1, precision, buffer);
            String res(buffer);
            while (res.length() < width){
                res = "0" + res;
            }
            return res;
        }
};
