#ifndef VALUES_H
#define VALUES_H

#include <EEPROM.h>

#include "config.h"

class Values{
    uint16_t eeprom_size = DISPENSE_SLOTS_N * 2 + sizeof(float) * 3 + sizeof(bool);
public:
    uint8_t dispenser_mode = Dispenser_modes::NONE;
    String mac_addr = "";
    
    // put values here
    uint16_t ammounts[DISPENSE_SLOTS_N];

    uint16_t ammount = 0;
    float weight_factor;
    float reference_mass;
    bool firmware_update = false;
    float feed_speed;

    Values() = default;

    void init_EEPROM();

    void load();
    void save();
    void dump();

    void clear();
};

extern Values *values;

#endif