#ifndef CONFING_H
#define CONFIG_H

//-----------------------PIN SETUP--------------------------

#define SA_PIN 18
#define SB_PIN 19
#define SW_PIN 5

#define TOUCH_PANEL_PIN 4

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

#define FONT_HEIGHT 16
#define FONT_H_OFFSET 12
#define ROW_HEIGHT 18

#define BLINK_TIME 250

#define DISP_UPDATE_INTERVAL 1000

//----------------------------------------------------------


//-----------------------DISPENSING-------------------------

#define INVERT_MIXER 0
#define INVERT_AUGER 0
#define ACCURATE_SPEED_FACTOR 0.25

#define DEFAULT_FEED_SPEED 0.5
#define MIN_FEED_SPEED 0.2
#define MAX_FEED_SPEED 1.5

#define DISPENSING_PRECISION 1

#define VIBRO_IN_T 20
#define VIBRO_OUT_T 4
#define VIBRO_SPEED_RATIO 3
// (VIBRO_IN_T - VIBRO_SPEED_RATIO * VIBRO_OUT_T) must be > 0

//----------------------------------------------------------


//-------------------------OTHER----------------------------

#define I2C_HIGH_PRFORMANCE

#define MICROSTEPPING 32

#define DISPENSE_SLOTS_N 8
#define MAX_DISPENSE_AMMOUNT 500
#define DEFAULT_REFERENCE_MASS 50
#define MAX_REFERENCE_MASS 500

#define ENCODER_BUFFER_SIZE 20

//----------------------------------------------------------


//----------------------------------------------------------

#define FIRMWARE_VERSION "1.0.0"

//-------------------------About----------------------------


//--------------------------WiFi----------------------------

#define HOST_SSID "Dispenser"
#define HOST_PASSWORD "12345678"
#define WEB_SERVER_PORT 80

//----------------------------------------------------------


//-------------------------MODES----------------------------

#ifndef DISPENSER_MODES
#define DISPENSER_MODES

enum Dispenser_modes{
    NONE = 1,
    DISPENSE,
    TARE,
    CALIBRATE,
    RESTORE,
    CLEAN,
    MIX,
    UPDATE
};

#endif

//----------------------------------------------------------


//----------------------OPTIMIZATION------------------------

#ifdef I2C_HIGH_PRFORMANCE

#define I2C_CLKRATE 400000

#endif

//----------------------------------------------------------

#endif