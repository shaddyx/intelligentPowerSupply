#pragma once
class Callback {
    void (*f_ptr)();
    void operator()(){
        (*f_ptr)();
    }
};
#define CB(name) void(*name)()


#define array_len(x)  (sizeof(x) / sizeof((x)[0]))

template <class T>
class ClassCallback{
    public:
        T * pthis;
        void (T::*f_ptr)();
        void operator()(){
            (*pthis.*f_ptr)();
        }
};
