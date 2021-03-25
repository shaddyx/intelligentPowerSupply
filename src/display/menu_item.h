#pragma once
#include <Arduino.h>
#include "util/util.h"
class MenuItem{
    public:
        String caption;
        int type;
        int id;
        bool canEnter;
        MenuItem(String caption, int type, int id, bool canEnter = false):
        caption(caption),
        type(type),
        id(id),
        canEnter(canEnter)
        {}

};