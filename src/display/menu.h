#pragma once
#include "menu_item.h"
#include "debug.h"


Debug menu_debug("Menu");
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
            menu_debug.info("Initializing menu");
            menu_debug.info("OK:" + items[0].caption);
        }

        int countCyrrentType(){
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
                    num ++;
                if (items[i].type == type){
                    if (num == number){
                        return i;
                    }
                }
            }
            return -1;
        }

        int findId(int id){
            for (int i=0; i<N; i++){
                if (items[i].id == id){
                    return i;
                }
            }
            return -1;
        }

        void next(){
            auto res = findType(current_type, current + 1);
            if (res != -1){
                current = res;
            }
        }

        void prev(){
            auto res = findType(current_type, current - 1);
            if (res != -1){
                current = res;
            }
        }
        
        void enter(){
            auto currentItem = items[current];
            auto res = findType(currentItem.id);
            if (res != -1){
                current = res;
            }
        }

        int getSelectedId(){
            auto res = findType(current_type, current);
            if (res == -1){
                return -1;
            }
            return items[res].id;
        }

        MenuItem * getScreenLine(int line){
            int number = countType(current_type);
            
            while (screen_cursor < current - screen_size){
                screen_cursor ++;
            }
            
            while (screen_cursor > current + screen_size || screen_cursor > number){
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