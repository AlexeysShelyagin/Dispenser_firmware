#include "values.h"

void Values::init_EEPROM(){
    EEPROM.begin(eeprom_size);
}

void Values::load(){
    for(uint8_t i = 0; i < DISPENSE_SLOTS_N; i++)
        EEPROM.get(i * 2, ammounts[i]);
    
}

void Values::save(){
    for(uint8_t i = 0; i < DISPENSE_SLOTS_N; i++)
        EEPROM.put(i * 2, ammounts[i]);

    EEPROM.commit();

    Serial.println("EEPROM updated");
}

void Values::clear(){
    for (int i = 0; i < eeprom_size; i++)
        EEPROM.write(i, 0);

    load();
    EEPROM.commit();
}