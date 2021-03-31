#pragma once
#include "display/liquid_crystal_display2x2.h"
#include "debug.h"
#include "util/float_util.h"
class DisplayInfo: public DisplayScreen{
    public:
        float v = 0;
        float c = 0;
        DisplayInfo(Display * display):
        display(display){

        }
        
        void poll(){
            if (oldV != FloatUtil::round2(v) || oldC != FloatUtil::round2(c)){
                needRefresh = true;
            }
            if (needRefresh){
                display->clear();
                display->printLine(0, "V: " + String(v) + " C: " + String(c));
                needRefresh = false;
                oldV = FloatUtil::round2(v);
                oldC = FloatUtil::round2(c);
            }
        }

    private:
        float oldV = 0;
        float oldC = 0;
        Display * display;
        
};