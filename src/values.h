#ifndef VALUES_H
#define VALUES_H

#include <EEPROM.h>

#include "config.h"

class Values{
    uint16_t eeprom_size = DISPENSE_SLOTS_N * 2;
public:
    // put values here
    uint16_t ammounts[DISPENSE_SLOTS_N];

    Values() = default;

    void init_EEPROM();

    void load();
    void save();

    void clear();
};

extern Values *values;

#endif