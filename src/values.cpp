#include "values.h"

void Values::init_EEPROM(){
    EEPROM.begin(eeprom_size);
}

void Values::load(){
    for(uint8_t i = 0; i < DISPENSE_SLOTS_N; i++)
        EEPROM.get(i * 2, ammounts[i]);
    
    EEPROM.get(DISPENSE_SLOTS_N * 2, weight_factor);
    EEPROM.get(DISPENSE_SLOTS_N * 2 + sizeof(float), reference_mass);
    if(reference_mass == 0)
        reference_mass = DEFAULT_REFERENCE_MASS;
    
    EEPROM.get(DISPENSE_SLOTS_N * 2 + sizeof(float) * 2, firmware_update);
}

void Values::save(){
    for(uint8_t i = 0; i < DISPENSE_SLOTS_N; i++)
        EEPROM.put(i * 2, ammounts[i]);

    EEPROM.put(DISPENSE_SLOTS_N * 2, weight_factor);
    EEPROM.put(DISPENSE_SLOTS_N * 2 + sizeof(float), reference_mass);

    EEPROM.put(DISPENSE_SLOTS_N * 2 + sizeof(float) * 2, firmware_update);

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
    for (uint16_t i = 0; i < eeprom_size; i++)
        EEPROM.write(i, 0);

    EEPROM.commit();
    dump();
    load();
}