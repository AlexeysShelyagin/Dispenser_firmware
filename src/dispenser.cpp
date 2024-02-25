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

    digitalWrite(in1_pin, 0);
    digitalWrite(in2_pin, 0);
    digitalWrite(mixer_stby, 1);

    ledcSetup(1, 1e4, 8);
    ledcAttachPin(pwm_pin, 1);
    ledcWrite(1, 0);
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

void Dispenser::start_auger(double speed, bool dir){
    if(INVERT_AUGER)
        dir = !dir;
    
    stepper.set_speed(speed);
    stepper.enable();
    stepper.run();
}

void Dispenser::stop_auger(){
    stepper.stop();
}

void Dispenser::start_mixer(uint8_t pwm){
    digitalWrite(mixer_in1, INVERT_MIXER);
    digitalWrite(mixer_in2, !INVERT_MIXER);

    ledcWrite(1, pwm);
}

void Dispenser::stop_mixer(){
    digitalWrite(mixer_in1, 0);
    digitalWrite(mixer_in2, 0);

    ledcWrite(1, 0);
}