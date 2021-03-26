#pragma once
#include <StateMachine.h>
#include "debug.h"
#include "util/util.h"
class MainStateMachine;
using MainCallBack = ClassCallback<MainStateMachine>;
Debug main_statemachine_debug("MainStateMachine");

class MainStateMachine{

    public:
        void init(){
            StateMachine machine = StateMachine();
            mcb.f_ptr = &MainStateMachine::cb;
            mcb.pthis = this;
            //State * IDLE = machine.addState(); 
        }
        
        void cb(){
            main_statemachine_debug.debug("OOOOK");
        }
        
    private:
       MainCallBack mcb;
       void idleState(){
           
       }
};