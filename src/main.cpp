#include <Arduino.h>

#include "config.h"				//must be always first include
#include "encoder.h"
#include "display.h"
#include "ui.h"
#include "menu_data.h"
#include "values.h"

#include "dispenser.h"

Encoder encoder(SA_PIN, SB_PIN, SW_PIN);
Display_SH1106 display(DISPLAY_WIDTH, DISPLAY_HEIGHT);
UI ui;
Values *values = new Values();

Dispenser dispenser;

uint64_t update_timer = 0;

void IRAM_ATTR encoder_interrupt(){
	encoder.tick(); 
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

	values -> load();

	pinMode(MIXER_PWM, OUTPUT);
	pinMode(MIXER_IN1, OUTPUT);
	pinMode(MIXER_IN2, OUTPUT);
	pinMode(MIXER_STANDBY, OUTPUT);

	dispenser.init_stepper(STEP_PIN, DIR_PIN, ENABLE_PIN, MICROSTEPPING);
	dispenser.init_mixer(MIXER_PWM, MIXER_IN1, MIXER_IN2, MIXER_STANDBY);
	dispenser.init_weight(WEIGHT_DOUT_PIN, WEIGHT_SCK_PIN);


	
	dispenser.stop_mixer();
	Serial.println("end");
}

void loop() {
	Encoder_data enc_data = encoder.get_updates();
	
    if(enc_data.turns != 0)
        ui.move(enc_data.turns);
    if(enc_data.clicks != 0){
        ui.select();
	}
	
	
	if(ui.event_done || update_timer + DISP_UPDATE_INTERVAL >= millis()){
		ui.render();
		update_timer = millis();
	}
	
}