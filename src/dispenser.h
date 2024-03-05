#ifndef DISPENSER_H
#define DISPENSER_H

#include "config.h"
#include "stepper.h"
#include "HX711.h"

#include "display.h"
#include "encoder.h"
#include "values.h"

#ifndef INVERT_AUGER
    #define INVERT_AUGER 0
#endif
#ifndef INVERT_MIXER
    #define INVERT_MIXER 0
#endif

class Dispenser{
    Stepper stepper;
    HX711 weight;
    int mixer_pwm, mixer_in1, mixer_in2, mixer_stby;

    Display_SH1106 *display = nullptr;
    Encoder *encoder = nullptr;

    static void vibro(void *pvParameters);

    struct Vibro_data{
        Dispenser* dispenser;
        uint16_t work_t, jerk_t;
    };

public:
    Dispenser() = default;

    void init_stepper(int step_pin, int dir_pin, int en_pin, int microstepping);
    void init_mixer(int pwm_pin, int in1_pin, int in2_pin, int standby_pin);
    void init_weight(int dout_pin, int sck_pin, double factor);
    void init_display(Display_SH1106* display_);
    void init_encoder(Encoder* encoder_);

    void start_auger(double speed);
    void set_auger_speed(double speed);
    double get_auger_speed();
    void stop_auger();

    void start_mixer(uint8_t pwm = 255);
    void stop_mixer();
    
    void tare();
    double calibrate_weight(float reference);

    bool dispense(float ammount);
    void clean();

    void restore();
};

#endif