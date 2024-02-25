#ifndef DISPENSER_H
#define DISPENSER_H

#include "config.h"
#include "stepper.h"
#include "HX711.h"

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

    void vibro(double rot_speed, int work_time, int jerk_time);

public:
    Dispenser() = default;

    void init_stepper(int step_pin, int dir_pin, int en_pin, int microstepping);
    void init_mixer(int pwm_pin, int in1_pin, int in2_pin, int standby_pin);
    void init_weight(int dout_pin, int sck_pin);

    void start_auger(double speed, bool dir = 0);
    void stop_auger();

    void start_mixer(uint8_t pwm = 255);
    void stop_mixer();
};

#endif