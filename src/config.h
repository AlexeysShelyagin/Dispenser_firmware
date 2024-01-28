#ifndef CONFING_H
#define CONFIG_H

#define SA_PIN 19
#define SB_PIN 18
#define SW_PIN 5

#define STEP_PIN 17
#define DIR_PIN 16
#define ENABLE_PIN 23

#define DISPLAY_ADDRESS 0x3C

#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64
#define FONT_SIZE 2

#define FONT_HEIGHT 7
#define ROW_HEIGHT 20

#define DISP_UPDATE_INTERVAL 1000

#define I2C_HIGH_PRFORMANCE

#define EEPROM_SIZE 2



#ifdef I2C_HIGH_PRFORMANCE

#define I2C_CLKRATE 400000

#endif

#endif