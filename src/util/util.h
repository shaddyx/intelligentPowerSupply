#pragma once
// A callback contains both a function and a pointer to arbitrary data
// that will be passed as argument to the function.
struct Callback {
    Callback(void (*f)() = 0)
        : function(f){}
    void (*function)();
};

#define CB(name) void(*name)()


#define array_len(x)  (sizeof(x) / sizeof((x)[0]))
