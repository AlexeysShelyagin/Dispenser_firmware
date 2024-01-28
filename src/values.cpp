#include "values.h"

void Values::load(){
    //EEPROM.get(EEPROM_SETTINGS_ADDR, *val1*);
    //EEPROM.get(EEPROM_SETTINGS_ADDR + 2, *val2*);
}

void Values::save(){
    //EEPROM.put(EEPROM_SETTINGS_ADDR, *val1*);
    //EEPROM.put(EEPROM_SETTINGS_ADDR + 2, *val2*);

    EEPROM.commit();
}

void Values::clear(){
    for (int i = 0; i < EEPROM_SIZE; i++)
        EEPROM.write(i, 0);

    EEPROM.commit();
}