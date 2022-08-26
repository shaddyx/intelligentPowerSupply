#pragma once

class ChargerAlgo{
    public:
        bool finished = false;
        virtual void init();
        virtual void poll(); 
        virtual void stop();
        virtual void pause();
        virtual void resume();

};


class SimpleCharger: public ChargerAlgo{
    public:
        float max_current;
        float max_voltage;
        virtual void init(){
            
        }
};

