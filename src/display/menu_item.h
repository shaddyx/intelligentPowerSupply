#pragma once
#include <Arduino.h>
#include "util/util.h"
class MenuItem{
    public:
        String caption;
        int type;
        int id;
        MenuItem(String caption, int type, int id):
        caption(caption),
        type(type),
        id(id)
        {}

};