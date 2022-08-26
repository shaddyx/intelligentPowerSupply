#pragma once
//#include <StateMachine.h>
#include "debug.h"
#include "util/util.h"
#include <collections/SimpleList.h>
DebugModule(debug_sm, "StateMachine");
enum {STATE_TYPE_SIMPLE, STATE_TYPE_CONDITIONAL};

class State {
    public:
        int type = STATE_TYPE_SIMPLE;
        SimpleList<State *> transitions;
};

class ConditionalState: public State {
    public:
        int type = STATE_TYPE_CONDITIONAL;
        ConditionalState(ParametrizedCallback<bool, State *> cb):
        cb(cb)
        {}
        ParametrizedCallback<bool, State *> cb;
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
            auto canChange = this -> current -> transitions.index_of(state) != -1;

            if (this -> current -> type == STATE_TYPE_CONDITIONAL){
                canChange = canChange && ((ConditionalState *) this -> current) -> cb(state);
            }
            if (canChange){
                forceState(state);
                return true;
            } else {
                log_info(debug_sm, "State change is not allowed");
                return false;
            }
            //log_info(debug_sm, "Unknown state type: " + String(this -> current -> type));
            return false;
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
        void forceState(State * state){
            log_info(debug_sm, "Changing state");
            current = state;
            pending.add(state);
        }
        bool changed = false;
        SimpleList<State *> pending;
};