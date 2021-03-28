#pragma once
//#include <StateMachine.h>
#include "debug.h"
#include "util/util.h"
#include <collections/SimpleList.h>
Debug statemachine_debug("StateMachine");

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
            if (this -> current -> transitions.index_of(state) != -1){
                statemachine_debug.info("Changing state");
                current = state;
                pending.add(state);
                return true;
            } else {
                statemachine_debug.info("State change is not allowed");
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