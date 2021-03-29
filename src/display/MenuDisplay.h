#pragma once
#include "menu.h"
#include "menu_item.h"
#include "debug.h"
#include "display/liquid_crystal_display2x2.h"

Debug menudisplay_debug("MenuDisplay");
template <int N>
class MenuDisplay: public DisplayScreen{
    public:
    MenuDisplay(Display & display, Menu<N> & menu):
    display(display),
    menu(menu){

    }

    void poll(){
        auto id = menu.getSelectedId();
        if (oldSelectedId != id){
            needRefresh = true;
        }
        if (needRefresh){
            display.clear();
            for (int i=0; i<CONF_DISPLAY_Y_SIZE; i++){
                MenuItem * m = menu.getScreenLine(i);
                if (m == 0){
                    return;
                }
                //menudisplay_debug.info("capt: " + m -> caption + " sel: " + String(id) + " id: " + String(m -> id));
                if (id == m -> id){
                    display.printLine(i, m -> caption + "<");
                } else {
                    display.printLine(i, m -> caption);
                }
            }
            oldSelectedId = id;
            needRefresh = false;
        }
    }
    void clear(){
        oldSelectedId = -1;
    }
    private:
        Display & display;
        Menu<N> & menu;
        int oldSelectedId = -1;

};