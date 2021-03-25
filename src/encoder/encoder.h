#pragma once
#include <RotaryEncoder.h>
#include "config.h"


class Encoder{
    public:
        Encoder(uint8_t clk, uint8_t dt):
        _encoder(clk, dt, RotaryEncoder::LatchMode::FOUR3)
        {}

        bool is_right(){
            return latest_direction == RotaryEncoder::Direction::CLOCKWISE;
        }

        bool is_left(){
            return latest_direction == RotaryEncoder::Direction::COUNTERCLOCKWISE;
        }

        void poll(){
            _encoder.tick();
            auto direction = _encoder.getDirection();
            if (latest_direction == direction){
                latest_direction = RotaryEncoder::Direction::NOROTATION;
                return;
            }
            latest_direction = direction;
        }

    private:
        RotaryEncoder::Direction latest_direction = RotaryEncoder::Direction::NOROTATION;
        RotaryEncoder _encoder;
};