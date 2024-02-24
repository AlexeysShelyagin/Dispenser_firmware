#include "dispenser.h"

void Dispenser::init_stepper(int step_pin, int dir_pin, int en_pin, int microstepping){
    stepper = Stepper(step_pin, dir_pin, en_pin);
    stepper.init(0, microstepping);
	stepper.disable();
}

void Dispenser::init_mixer(int pwm_pin, int in1_pin, int in2_pin, int standby_pin){
    mixer_pwm = pwm_pin;
    mixer_in1 = in1_pin;
    mixer_in2 = in2_pin;
    mixer_stby = standby_pin;
}

void Dispenser::init_weight(int dout_pin, int sck_pin){
    weight.begin(dout_pin, sck_pin);
}

void Dispenser::vibro(double rot_speed, int work_time, int jerk_time){
	double new_speed;
	double t1 = work_time, t2 = jerk_time;
	double fwd_speed = (t1 + t2) / (t1 + VIBRO_SPEED_RATIO * t2) * rot_speed;

	if(millis() % (work_time + jerk_time) < work_time)
		new_speed = fwd_speed;
	else
		new_speed = -fwd_speed * VIBRO_SPEED_RATIO;
	
	if(stepper.current_speed() == new_speed)
		return;
	stepper.set_speed(new_speed);
}