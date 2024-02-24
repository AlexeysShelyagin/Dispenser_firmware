#ifndef CONFING_H
#define CONFIG_H

//-----------------------PIN SETUP--------------------------

#define SA_PIN 18
#define SB_PIN 19
#define SW_PIN 5

#define STEP_PIN 17
#define DIR_PIN 16
#define ENABLE_PIN 23

#define WEIGHT_DOUT_PIN 27
#define WEIGHT_SCK_PIN 14

#define MIXER_PWM 32
#define MIXER_IN1 25
#define MIXER_IN2 33
#define MIXER_STANDBY 26

#define DISPLAY_ADDRESS 0x3C 

//----------------------------------------------------------


//-------------------DISPLAY PARAMETERS---------------------

#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64
#define FONT_SIZE 1

#define FONT_HEIGHT 14
#define FONT_H_OFFSET 12
#define ROW_HEIGHT 18

#define DISP_UPDATE_INTERVAL 1000

//----------------------------------------------------------


//-------------------------OTHER----------------------------

#define I2C_HIGH_PRFORMANCE

#define EEPROM_SIZE 2


#define VIBRO_SPEED_RATIO 5


//-------------------------OPTIMIZATION----------------------------

#ifdef I2C_HIGH_PRFORMANCE

#define I2C_CLKRATE 400000

#endif

//----------------------------------------------------------

#endif