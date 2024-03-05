#include <Arduino.h>

#include "config.h"				//must be always first include
#include "encoder.h"
#include "display.h"
#include "ui.h"
#include "menu_data.h"
#include "values.h"
#include "updater.h"

#include "dispenser.h"

Encoder encoder(SA_PIN, SB_PIN, SW_PIN);
Display_SH1106 display(DISPLAY_WIDTH, DISPLAY_HEIGHT);
UI ui;
Values *values = new Values();

Client_updater updater;
Dispenser dispenser;

uint64_t update_timer = 0;

void IRAM_ATTR encoder_tick(){
	encoder.tick(); 
}

void IRAM_ATTR encoder_click(){
	encoder.tick_button(); 
}

void enable_encoder_interrupt(){
	attachInterrupt(digitalPinToInterrupt(SA_PIN), encoder_tick, CHANGE);
    attachInterrupt(digitalPinToInterrupt(SB_PIN), encoder_tick, CHANGE);
    attachInterrupt(digitalPinToInterrupt(SW_PIN), encoder_click, RISING);
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

	Serial.println("display initialized");

	pinMode(SA_PIN, INPUT_PULLUP);
	pinMode(SB_PIN, INPUT_PULLUP);
	pinMode(SW_PIN, INPUT_PULLUP);
	pinMode(TOUCH_PANEL_PIN, INPUT);

	enable_encoder_interrupt();
	attachInterrupt(digitalPinToInterrupt(TOUCH_PANEL_PIN), encoder_click, RISING);
    
	ui.init(
        load_menu(menu::menu_list, menu::menu_sizes, menu::menu_linking, menu::menu_types, menu::menu_n),
        &display, FONT_HEIGHT, ROW_HEIGHT
    );
    //ui.tree();

	ui.current_menu = ui.default_menu;

	values -> init_EEPROM();
	values -> load();
	values -> dump();

	values -> mac_addr = WiFi.macAddress();

	updater.init_display(&display);
	updater.check_updates();


	pinMode(MIXER_PWM, OUTPUT);
	pinMode(MIXER_IN1, OUTPUT);
	pinMode(MIXER_IN2, OUTPUT);
	pinMode(MIXER_STANDBY, OUTPUT);

	dispenser.init_stepper(STEP_PIN, DIR_PIN, ENABLE_PIN, MICROSTEPPING);
	dispenser.init_mixer(MIXER_PWM, MIXER_IN1, MIXER_IN2, MIXER_STANDBY);
	dispenser.init_weight(WEIGHT_DOUT_PIN, WEIGHT_SCK_PIN, values -> weight_factor);
	dispenser.init_display(&display);
	dispenser.init_encoder(&encoder);

	Serial.println("Test updater");
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

	if(values -> dispenser_mode != Dispenser_modes::NONE){
		if(values -> dispenser_mode == Dispenser_modes::DISPENSE){
			dispenser.dispense(values -> ammount);

			values -> ammount = 0;
		}
		if(values -> dispenser_mode == Dispenser_modes::TARE)
			dispenser.tare();
		if(values -> dispenser_mode == Dispenser_modes::CALIBRATE){
			values -> weight_factor = dispenser.calibrate_weight(values -> reference_mass);
			values -> save();
		}
		if(values -> dispenser_mode == Dispenser_modes::RESTORE)
			dispenser.restore();
		if(values -> dispenser_mode == Dispenser_modes::CLEAN)
			dispenser.clean();
		if(values -> dispenser_mode == Dispenser_modes::UPDATE){
			updater.start_upload();
		}

		values -> dispenser_mode = Dispenser_modes::NONE;
		ui.event_done = true;
	}
}