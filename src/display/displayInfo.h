#pragma once
#include "display/liquid_crystal_display2x2.h"
#include "debug.h"
#include "util/float_util.h"
class DisplayInfo: public DisplayScreen{
    public:
        float v = 0;
        float c = 0;
        bool on = 0;
        DisplayInfo(Display * display):
        display(display){

        }
        
        void poll(){
            if (needRefresh){
                needRefresh = false;
                display->clear();
            }
            if (oldV != FloatUtil::round2(v) || oldC != FloatUtil::round2(c) || oldOn != on){
                display->printLine(0, "V:" + FloatUtil::floatToString(v, 5, 2) + " C:" + FloatUtil::floatToString(c, 5, 2) + "    ");
                String flags = "";
                if (on){
                    flags += "ON ";
                }
                flags += "                 ";
                display->printLine(1, flags);
                oldV = FloatUtil::round2(v);
                oldC = FloatUtil::round2(c);
                oldOn = on;
            }
        }

    private:
        float oldV = 0;
        float oldC = 0;
        bool oldOn = 0;
        Display * display;
        
};