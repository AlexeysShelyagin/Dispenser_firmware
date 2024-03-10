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

    ledcSetup(1, 10000, 8);
    ledcAttachPin(pwm_pin, 1);
    ledcWrite(1, 0);
}

void Dispenser::init_weight(int dout_pin, int sck_pin, double factor){
    weight.begin(dout_pin, sck_pin);
    weight.set_scale(factor);
}

void Dispenser::init_display(Display_SH1106* display_){
    display = display_;
}

void Dispenser::init_encoder(Encoder* encoder_){
    encoder = encoder_;
}

void Dispenser::start_auger(double speed){
    speed *= (INVERT_AUGER) ? 1 : -1;

    stepper.set_speed(speed);
    stepper.enable();
    stepper.run();
}

void Dispenser::set_auger_speed(double speed){
    if(speed == stepper.current_speed())
        return;
    
    speed *= (INVERT_AUGER) ? 1 : -1;
    
    stepper.set_speed(speed);
}

double Dispenser::get_auger_speed(){
    return stepper.current_speed();
}

void Dispenser::stop_auger(){
    stepper.stop();
    stepper.disable();
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

void Dispenser::tare(){
    display -> clear();
    display -> print("Tare...");
    display -> show();

    weight.tare();
}

double Dispenser::calibrate_weight(float reference){
    display -> clear();
    display -> print("Calibrating...");
    display -> show();

    float calibrate_val = weight.get_value(20);
    double weight_factor = calibrate_val / reference;
    weight.set_scale(weight_factor);

    return weight_factor;
}

void Dispenser::vibro(void *pvParameters){
    Vibro_data *args = (Vibro_data*) pvParameters;
    
    Dispenser *dispenser = args -> dispenser;
    uint16_t work_t = args -> work_t, jerk_t = args -> jerk_t;

    double new_speed;
    double t1 = work_t, t2 = jerk_t;
    
    dispenser -> start_auger(0);
    double fwd_speed;
    while(1){
        fwd_speed = values -> feed_speed * (t1 + t2) / (t1 - VIBRO_SPEED_RATIO * t2);

        if(millis() % (work_t + jerk_t) < work_t)
            new_speed = fwd_speed;
        else
            new_speed = -fwd_speed * VIBRO_SPEED_RATIO;
        
        dispenser -> set_auger_speed(new_speed);

        delay(2);
    }
}

bool Dispenser::dispense(float ammount){
    if(ammount == 0)
        return true;
    if(weight.get_scale() == 0){
        display -> clear();
        display -> print("Not calibrated");
        display -> show();
        delay(1500);
        return false;
    }
    tare();

    start_mixer();

    TaskHandle_t func;
    Vibro_data args = {this, VIBRO_IN_T, VIBRO_OUT_T};
    xTaskCreatePinnedToCore(vibro, "", 4096, &args, 1, &func, !xPortGetCoreID());

    double init_spd = values -> feed_speed;
    uint64_t changing_spd = 0;
    float current_weight = 0;
    float spd_factor = 1;
    bool aborted = false;
    while(true){
        current_weight = max(weight.get_units(), (float) 0);
        float ready = current_weight / ammount;

        display -> clear();

        display -> print("Dispensing:");
        display -> print(String(current_weight, 1), 0, FONT_HEIGHT + 2);
        display -> draw_rect(1, FONT_HEIGHT * 2 + 4, display -> width - 2, 6);
        display -> draw_rect(3, FONT_HEIGHT *2  + 6, floor((display -> width - 5) * ready), 2);
        if(changing_spd == 0)
            display -> print("Click to abort", 0, display -> height - FONT_HEIGHT);
        else
            display -> print("Speed: " + String(values -> feed_speed), 0, display -> height - FONT_HEIGHT);

        Encoder_data enc_data = encoder -> get_updates();
        if(enc_data.clicks != 0){
            Serial.println("Dispensing aborted");
            aborted = true;
            break;
        }
        if(enc_data.turns != 0){
            changing_spd = millis();
            values -> feed_speed = max(min(
                values -> feed_speed + 0.1 * enc_data.turns, 
                (double) MAX_FEED_SPEED),
                (double) MIN_FEED_SPEED
            );
            Serial.println(values -> feed_speed);
        }

        if(millis() - changing_spd > 500)
            changing_spd = 0;

        display -> show();

        if(ammount - current_weight < DISPENSING_STOP_DIFFERENCE)
            break;
    }

    if(init_spd != values -> feed_speed)
        values -> save();
    
    Serial.println("done");
    vTaskDelete(func);
    stop_mixer();
    stop_auger();

    current_weight = weight.get_units();
    display -> clear();
    if(aborted)
        display -> print("Aborted at:\n" + String(current_weight));
    else{
        display -> print("Done: " + String(current_weight));
        if(abs(ammount - current_weight) > DISPENSING_PRECISION)
            display -> print("Error: " + String(abs(ammount - current_weight)), 0, FONT_HEIGHT + 2);
    }
    display -> show();

    delay(2000);

    return aborted;
}

void Dispenser::clean(){
    start_mixer();
    start_auger(- values -> feed_speed * 0.5);

    while(true){
        display -> clear();
        display -> print("Cleaning...");
        display -> print("Click to stop", 0, display -> height - FONT_HEIGHT);
        display -> show();

        Encoder_data enc_data = encoder -> get_updates();
        if(enc_data.clicks != 0)
            break;
    }

    stop_auger();
    stop_mixer();
}

void Dispenser::mix(){
    display -> clear();
    display -> print("Press to stop");
    display -> show();

    start_mixer();

    while (true){
        Encoder_data enc_data = encoder -> get_updates();
        if(enc_data.clicks != 0)
            break;
        
        delay(1);
    }

    stop_mixer();
}

void Dispenser::restore(){
    weight.set_scale(0);
}