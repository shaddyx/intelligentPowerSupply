
#include <JC_Button.h>   

class SupplyButton{
    public:
        SupplyButton(uint8_t pin):
        button(pin){
        }
        void init(){
            if (!initialized){
                button.begin();
                initialized = true;
            }
            
        }
        
        void poll(){
            button.read();
        }

        bool pressed(){
            return button.wasPressed();
        }
    private: 
        Button button;
        bool initialized = false;
};