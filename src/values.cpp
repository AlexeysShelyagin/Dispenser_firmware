#include "values.h"

void Values::load(){
    for(uint8_t i = 0; i < DISPENSE_SLOTS_N; i++)
        EEPROM.get(i * 2, ammounts[i]);
    
}

void Values::save(){
    for(uint8_t i = 0; i < DISPENSE_SLOTS_N; i++)
        EEPROM.put(i * 2, ammounts[i]);

    EEPROM.commit();
}

void Values::clear(){
    uint16_t eeprom_size = DISPENSE_SLOTS_N * 2;
    for (int i = 0; i < eeprom_size; i++)
        EEPROM.write(i, 0);

    EEPROM.commit();
}