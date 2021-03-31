#pragma once

class FloatUtil{
    public:
        static float round2(float a){
            return roundf((double)a * 100) / 100;
        }
};
