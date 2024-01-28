#ifndef VALUES_H
#define VALUES_H

#include <EEPROM.h>

#include "config.h"

class Values{
public:
    // put values here

    Values() = default;

    void load();
    void save();

    void clear();
};

extern Values *values;

#endif