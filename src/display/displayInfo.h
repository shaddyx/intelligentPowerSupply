#pragma once
#include "display/liquid_crystal_display2x2.h"
#include "debug.h"
Debug displayInfo_debug("DisplayInfo");
class DisplayInfo: public DisplayScreen{
    public:
        float v = 0;
        float c = 0;
        DisplayInfo(Display * display):
        display(display){

        }
        
        void poll(){
            if (oldV != v || oldC != c){
                needRefresh = true;
            }
            if (needRefresh){
                display->clear();
                display->printLine(0, "V: " + String(v) + " C: " + String(c));
                needRefresh = false;
                oldV = v;
                oldC = c;
            }
        }

    private:
        float oldV = 0;
        float oldC = 0;
        Display * display;
        
};