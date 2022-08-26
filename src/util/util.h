#pragma once
class Callback {
    public:
        void (*f_ptr)();
        void operator()(){
            (*f_ptr)();
        }
};


template <class R, class... T>
class ParametrizedCallback {
    public:
        ParametrizedCallback(R (*f_ptr)(T... params)):
        f_ptr(f_ptr){
            
        }
        R (*f_ptr)(T... params);
        R operator()(T... params){
            return (*f_ptr)(params...);
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

#define nil 0