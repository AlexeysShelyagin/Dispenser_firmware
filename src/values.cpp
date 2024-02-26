#include "values.h"

void Values::init_EEPROM(){
    EEPROM.begin(eeprom_size);
}

void Values::load(){
    for(uint8_t i = 0; i < DISPENSE_SLOTS_N; i++)
        EEPROM.get(i * 2, ammounts[i]);
    
    EEPROM.get(DISPENSE_SLOTS_N * 2, weight_factor);
}

void Values::save(){
    for(uint8_t i = 0; i < DISPENSE_SLOTS_N; i++)
        EEPROM.put(i * 2, ammounts[i]);

    EEPROM.put(DISPENSE_SLOTS_N * 2, weight_factor);

    EEPROM.commit();

    Serial.println("EEPROM updated");
}

void Values::dump(){
    Serial.print("EEPROM dump:    ");
    for(uint16_t i = 0; i < eeprom_size; i++){
        Serial.print(EEPROM.read(i), HEX);
        Serial.print(" ");
    }
    Serial.println();
}

void Values::clear(){
    for (int i = 0; i < eeprom_size; i++)
        EEPROM.write(i, 0);

    EEPROM.commit();
    load();
}