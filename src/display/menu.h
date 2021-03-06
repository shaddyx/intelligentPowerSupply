#pragma once
#include "menu_item.h"
#include "debug.h"
DebugModule(debug_menu, "Menu");
template<int N>
class Menu{
    public:
        int current = 0;
        int current_type = 0;
        int screen_cursor = 0;
        int screen_size = 2;
        Menu(MenuItem (&items)[N]):
        items(items)
        {}

        int getSize(){
            return N;
        }

        void init(){
            log_info(debug_menu, "Initializing menu");
            log_info(debug_menu, "OK:" + items[0].caption);
        }

        int countCurrentType(){
            return countType(current_type);
        }
        int countType(int type){
            int res = 0;
            for (int i=0; i<N; i++){
                if (items[i].type == type){
                    res ++;
                }
            }
            return res;
        }

        int findType(int type, int number){
            int num = -1;
            for (int i=0; i<N; i++){
                if (items[i].type == type){
                    num ++;
                    if (num == number){
                        return i;
                    }
                }
            }
            return -1;
        }

        void next(){
            int number = countCurrentType();
            if (current < number - 1){
                current ++;
            }
        }

        void prev(){
            if (current > 0){
                current --;
            }
        }
        MenuItem * findItem(int id){
        for (unsigned int i=0; i < N; i++){
            if (items[i].id == id){
                return &items[i];
            }
        }
        return 0;
    }
        
        bool enter(){
            auto menuItem = findCurrent();
            if (menuItem != 0){
                if (menuItem->canEnter){
                    log_info(debug_menu, "Old current_type: " + String(current_type) + " new: " + menuItem->id);
                    current_type = menuItem->id;
                    current = 0;
                    screen_cursor = 0;
                    return true;
                }
            }
            return false;
        }

        int getSelectedId(){
            auto currentElement = findCurrent();
            if (currentElement == 0){
                return -1;
            }
            return currentElement -> id;
        }

        MenuItem * findCurrent(){
            auto res = findType(current_type, current);
            if (res == -1){
                return 0;
            }
            return & items[res];
        }

        MenuItem * getScreenLine(int line){
            int number = countCurrentType();
            while (screen_cursor < current - screen_size + 1){
                screen_cursor ++;
            }
            
            while (screen_cursor >= current + screen_size - 1 || screen_cursor >= number){
                screen_cursor --;
            }

            auto i = findType(current_type, screen_cursor + line);
            if (i == -1){
                return 0;
            }
            return &items[i];
        }
    private:
        MenuItem (&items)[N];
};