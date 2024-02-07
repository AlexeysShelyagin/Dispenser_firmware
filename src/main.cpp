#include <Arduino.h>

#include "config.h"
#include "encoder.h"
#include "display.h"
#include "ui.h"
#include "menu_data.h"
#include "stepper.h"

#include "HX711.h"

Encoder encoder(SA_PIN, SB_PIN, SW_PIN);
Display_SH1106 display(DISPLAY_WIDTH, DISPLAY_HEIGHT);
UI ui;

Stepper stepper(STEP_PIN, DIR_PIN, ENABLE_PIN);
HX711 weight;

uint64_t update_timer = 0;

void IRAM_ATTR encoder_interrupt(){
	encoder.tick(); 
}

void vibro(double rot_speed, int work_time, int jerk_time){
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

void setup() {
	Serial.begin(115200);
	Serial.println("\nstarted... ");

#ifdef I2C_HIGH_PRFORMANCE
	while (!Wire.begin()) {
		Serial.println("Failed to configure I2C bus");
		delay(10);
	}
	Wire.setClock(I2C_CLKRATE);
#endif

	display.init(DISPLAY_ADDRESS);
	display.init_font(FONT_SIZE);
    display.clear();
    display.show();

	pinMode(SA_PIN, INPUT_PULLUP);
	pinMode(SB_PIN, INPUT_PULLUP);
	pinMode(SW_PIN, INPUT_PULLUP);

	attachInterrupt(digitalPinToInterrupt(SA_PIN), encoder_interrupt, CHANGE);
    attachInterrupt(digitalPinToInterrupt(SB_PIN), encoder_interrupt, CHANGE);
    attachInterrupt(digitalPinToInterrupt(SW_PIN), encoder_interrupt, CHANGE);
    
	ui.init(
        load_menu(menu::menu_list, menu::menu_sizes, menu::menu_linking, menu::menu_types, menu::menu_n),
        &display, FONT_HEIGHT, ROW_HEIGHT
    );
    ui.tree();

	ui.current_menu = ui.default_menu;

	//weight.begin(WEIGHT_DOUT_PIN, WEIGHT_SCK_PIN);

	stepper.init(0, 32);
	stepper.enable();

	/*
	for(int i = 0; i < 1000; i++){
		stepper.set_speed(1);
		delayMicroseconds(4000);

		stepper.set_speed(-5);
		delayMicroseconds(1000);
	}
	*/
	stepper.set_speed(1);
	stepper.run();
	delay(5000);
}


void loop() {
	vibro(1, 16, 4);
	/*
	Encoder_data enc_data = encoder.get_updates();
	
    if(enc_data.turns != 0)
        ui.move(enc_data.turns);
    if(enc_data.clicks != 0)
        ui.select();
	
	
	if(ui.event_done || update_timer + DISP_UPDATE_INTERVAL >= millis()){
		ui.render();
		update_timer = millis();
	}
	*/
}