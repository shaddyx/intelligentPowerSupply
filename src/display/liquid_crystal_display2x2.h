#pragma once

#include "config.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "util/util.h"
#include <collections/SimpleList.h>
class DisplayScreen{
    public:
        bool needRefresh = true;
};

class Display {
    public:
        Display():
        lcd(CONF_DISPLAY_ADDRESS,CONF_DISPLAY_X_SIZE, CONF_DISPLAY_Y_SIZE)
        {

        }
        void init()
        {
            this -> lcd.init();
            this -> lcd.backlight();
            // this -> lcd.setCursor(1,0);
            // this -> lcd.print("hello everyone");
            // this -> lcd.setCursor(1,1);
            // this -> lcd.print("konichiwaa");
        }

        void clear(){
            this->lcd.clear();
        }

        void printLine(int row, String line){
            this -> lcd.setCursor(0, row);
            this -> lcd.print(line);
        }
        
        void refresh_all(){
            for (int i=0; i<screens.get_size(); i++){
                screens.get(i)->needRefresh = true;
            }
        }
        void add_component(DisplayScreen * screen){
            screens.add(screen);
        }

    private:
        LiquidCrystal_I2C lcd;
        SimpleList <DisplayScreen *> screens;
};