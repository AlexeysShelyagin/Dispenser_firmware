#include "stepper.h"

Stepper::Stepper(int step_pin_, int dir_pin_, int en_pin_){
    step_pin = step_pin_;
    dir_pin = dir_pin_;
    en_pin = en_pin_;
}

void Stepper::init(uint8_t pwm_channel_, uint8_t microstepping_divider){
    pwm_channel = pwm_channel_;
    microstep = microstepping_divider;

    pinMode(step_pin, OUTPUT);
    pinMode(dir_pin, OUTPUT);
    pinMode(en_pin, OUTPUT);

    ledcSetup(pwm_channel, 0, STEP_PWM_RESOLUTION);
}

void Stepper::enable(){
    if(en_pin == -1)
        return;
    digitalWrite(en_pin, 0);
}

void Stepper::disable(){
    if(en_pin == -1)
        return;
    digitalWrite(en_pin, 1);
}

void Stepper::set_speed(double rpm){
    digitalWrite(dir_pin, (rpm < 0));

    rpm = max(min(rpm, (double) MAX_STEPPER_RPM), (double) -MAX_STEPPER_RPM);
    current_rpm = rpm;
    rpm = abs(rpm);
    double freq = rpm * STEPS_PER_REVOLUTION * microstep;

    ledcChangeFrequency(pwm_channel, freq, STEP_PWM_RESOLUTION);
}

void Stepper::set_acceleration(double rpm_per_s){
    accel = rpm_per_s;
}

bool Stepper::accelerate_to(double target_rpm){
    if(accel == 0)
        return 0;
    
    if(accel_last_time == 0){
        accel_last_time = millis();
        return 1;
    }

    if(accel_last_time == millis())
        return 1;

    double new_rpm = current_rpm + accel *  (double) (millis() - accel_last_time) / 1000.0;

    if(abs(new_rpm) > abs(target_rpm)){
        new_rpm = target_rpm;
        set_speed(new_rpm);

        accel_last_time = 0;
        return 0;
    }

    set_speed(new_rpm);

    accel_last_time = millis();
    return 1;
}

void Stepper::run(){
    ledcAttachPin(step_pin, pwm_channel);
    ledcWrite(pwm_channel, STEP_PWM_DUTY);
}

void Stepper::stop(){
    ledcDetachPin(step_pin);
}