#include "encoder.h"

Encoder_data::Encoder_data(int turns_, int clicks_){
    turns = turns_;
    clicks = clicks_;
}

Encoder::Encoder(int sa_pin_, int sb_pin_, int sw_pin_){
    sa_pin = sa_pin_;
    sb_pin = sb_pin_;
    sw_pin = sw_pin_;

    buff_size = 0;
}

int Encoder::get_rotation(bool v1, bool v2){
    if(v1 == 1 && v2 == 1) 
        ready_to_read = true;

    else if(ready_to_read){
        ready_to_read = false;
        return (v1 == 1) ? 1 : -1;
    }
    
    return 0;
}

void Encoder::tick(){

    if(tick_time_filter + 1 >= millis())
        return;
    tick_time_filter = millis();

    if(buff_size >= ENCODER_BUFFER_SIZE) return;
    
    buffer[buff_size][0] = digitalRead(sa_pin);
    buffer[buff_size][1] = digitalRead(sb_pin);
    buff_size++;
};

void Encoder::tick_button(){
    if(!digitalRead(sw_pin)){
        if(click_start == 0)
            click_start = millis();
        return;
    }

    if(millis() - click_time_filter <= 100)
        return;
    click_time_filter = millis();

    if(click_start == 0 || millis() - click_start < 20)
        return;

    click = true;
    click_start = 0;
}

Encoder_data Encoder::get_updates(){
    Encoder_data response;
    for(int i = 0; i < buff_size; i++)
        response.turns += get_rotation(buffer[i][0], buffer[i][1]);
    response.clicks += click;

    response.turns += forced_turns;
    forced_turns = 0;

    buff_size = 0;
    click = false;

    return response;
}

void Encoder::force_button(bool state){
    click = state;
}

void Encoder::force_tick(int8_t turns){
    forced_turns = turns;
}

Encoder_data Encoder::get_updates_normalized(){
    Encoder_data response = get_updates();

    if(response.turns != 0)
        response.turns = response.turns / abs(response.turns);

    return response;
}
