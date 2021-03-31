#pragma once
//#include <StateMachine.h>
#include "debug.h"
#include "util/util.h"
#include <collections/SimpleList.h>
DebugModule(debug_sm, "StateMachine");
class State {
    public:
        SimpleList<State *> transitions;
};

class StateMachine{
    public:
        State * current;
        StateMachine(State * current):
        current(current)
        {}
        
        bool changeState(State * state){
            if (this -> current == state){
                return false;
            }
            if (this -> current -> transitions.index_of(state) != -1){
                log_info(debug_sm, "Changing state");
                current = state;
                pending.add(state);
                return true;
            } else {
                log_info(debug_sm, "State change is not allowed");
                return false;
            }
        }
        

        bool isChanged(){
           if (pending.get_size()){
               return true;
           }
           return false;
        }
        
        State * unHandledStateChange(){
            if (!pending.get_size()){
                return nil;
            }
            return pending.pop();
        } 

        void init(){
            
        }

    private:
        bool changed = false;
        SimpleList<State *> pending;
};