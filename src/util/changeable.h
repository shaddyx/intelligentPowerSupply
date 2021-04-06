#pragma once

template <class T>
class Changeable{
    public:
        Changeable(T value):
        value(value)
        {}
        T value;
        T old;
        bool isChanged(){
            bool res = value != old;
            old = value;
            return res;
        }

};