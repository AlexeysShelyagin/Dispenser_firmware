#ifndef STEPPER_H
#define STEPPER_H

#include <Arduino.h>

#define STEP_PWM_RESOLUTION 8
#define STEP_PWM_DUTY 16
#define STEPS_PER_REVOLUTION 200
#define MAX_STEPPER_RPS 5

class Stepper{
    int step_pin, dir_pin, en_pin;
    uint8_t pwm_channel;
    uint8_t microstep;

    double accel = 0, current_rps = 0;
    uint64_t accel_last_time = 0;

public:
    Stepper(int step_pin_, int dir_pin_, int en_pin_ = -1);

    void init(uint8_t pwm_channel, uint8_t microstepping_divider = 1);

    void enable();
    void disable();

    void set_speed(double rps);
    void set_acceleration(double rps_per_s);

    bool accelerate_to(double target_rps);

    void run();
    void stop();

    void step(int8_t dir = 1);

    double current_speed();
    double current_accel();
};

#endif