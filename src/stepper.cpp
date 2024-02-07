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

void Stepper::set_speed(double rps){
    digitalWrite(dir_pin, (rps < 0));

    rps = max(min(rps, (double) MAX_STEPPER_RPS), (double) -MAX_STEPPER_RPS);
    current_rps = rps;
    rps = abs(rps);
    double freq = rps * STEPS_PER_REVOLUTION * microstep;

    ledcChangeFrequency(pwm_channel, freq, STEP_PWM_RESOLUTION);
}

void Stepper::set_acceleration(double rps_per_s){
    accel = rps_per_s;
}

bool Stepper::accelerate_to(double target_rps){
    if(accel == 0)
        return 0;
    
    if(accel_last_time == 0){
        accel_last_time = millis();
        return 1;
    }

    if(accel_last_time == millis())
        return 1;

    double new_rps = current_rps + accel *  (double) (millis() - accel_last_time) / 1000.0;

    if(abs(new_rps) > abs(target_rps)){
        new_rps = target_rps;
        set_speed(new_rps);

        accel_last_time = 0;
        return 0;
    }

    set_speed(new_rps);

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

void Stepper::step(int8_t dir){
    bool remebered_dir = digitalRead(dir_pin);
    digitalWrite(dir_pin, (dir < 0));

    digitalWrite(step_pin, 1);
    delayMicroseconds(5);
    digitalWrite(step_pin, 0);

    digitalWrite(dir_pin, remebered_dir);
}

double Stepper::current_speed(){
    return current_rps;
}

double Stepper::current_accel(){
    return accel;
}