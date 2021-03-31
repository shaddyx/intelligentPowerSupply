#include <Arduino.h>
#include "config.h"
#include "display/liquid_crystal_display2x2.h"
#include "debug.h"
#include "util/util.h"
#include "display/menu_item.h"
#include "display/menu.h"
#include "display/MenuDisplay.h"
#include "encoder/encoder.h"
#include "display/button.h"
#include "state/state_machine.h"
#include "display/displayParam.h"
#include "supply/power_control.h"
#include "display/displayInfo.h"
DebugModule(debug_main, "Main");
enum {Root, MVOLTAGE, MCURRENT, CHARGE, CC, CV, START_CHARGE, MCONFIG, MCALIBRATE};
MenuItem items[] = {
	MenuItem("V", Root, MVOLTAGE),
	MenuItem("C", Root, MCURRENT),
	MenuItem("Charge", Root, CHARGE, true),
		MenuItem("..", CHARGE, Root, true),
		MenuItem("CC", CHARGE, CC),
		MenuItem("CV", CHARGE, CV),
	MenuItem("Config", Root, MCONFIG, true),
		MenuItem("..", MCONFIG, Root, true),
		MenuItem("Calibrate", MCONFIG, MCALIBRATE),
	MenuItem("Start charge", Root, START_CHARGE)
};

Menu<array_len(items)> menu(items);
Display display;
MenuDisplay<array_len(items)> menuDisplay(display, menu);
Encoder encoder(ENCODER_CLK, ENCODER_DT);
SupplyButton enter_button(ENTER_BUTTON_PIN);
State STATE_IDLE;
State STATE_DISPLAY_INFO;
State STATE_CONFIG_VOLTAGE;
State STATE_CONFIG_CURRENT;
State STATE_CALIBRATING;

PowerControl power(CONF_DAC_ADDRESS, CONF_VOLTAGE_CHECK_PIN, CONF_MAX_VOLTAGE);
StateMachine mstateMachine(&STATE_IDLE);
DisplayParam<float> voltage(String("Voltage"), &display, &encoder, 0, 0, CONF_MAX_VOLTAGE, 0.1, 1);
DisplayParam<float> current(String("Current"), &display, &encoder, 0, 0, CONF_MAX_CURRENT, 0.1, 1);
DisplayInfo displayInfo(&display);
TimeDelay idle_timer(5000);

MenuItem * findItem(int id){
	for (unsigned int i=0; i< array_len(items); i++){
		if (items[i].id == id){
			return &items[i];
		}
	}
	return 0;
}

void updateVoltageAndCurrent(){
	findItem(MVOLTAGE) -> caption = "V: " + String (voltage.current);
	findItem(MCURRENT) -> caption = "C: " + String (current.current);
}

void initComponents(){
	display.add_component(&voltage);
	display.add_component(&current);
	display.add_component(&menuDisplay);
	display.add_component(&displayInfo);
}

void setup(){
	log_info(debug_main, "Initializing (" + String(menu.getSize()) + ")");
	initComponents();
	display.init();
	menu.init();
	enter_button.poll();
	mstateMachine.init();
	STATE_IDLE.transitions.add(&STATE_CONFIG_VOLTAGE);
	STATE_IDLE.transitions.add(&STATE_CONFIG_CURRENT);
	STATE_IDLE.transitions.add(&STATE_CALIBRATING);
	STATE_IDLE.transitions.add(&STATE_DISPLAY_INFO);
	STATE_DISPLAY_INFO.transitions.add(&STATE_IDLE);
	STATE_CONFIG_VOLTAGE.transitions.add(&STATE_IDLE);
	STATE_CONFIG_CURRENT.transitions.add(&STATE_IDLE);
	STATE_CALIBRATING.transitions.add(&STATE_IDLE);

	updateVoltageAndCurrent();
	log_info(debug_main, "Calibrating");
	power.calibrate();
	log_info(debug_main, "Init complete");
}

void processMenuItemEnter(MenuItem * item){
	if (item -> id == MVOLTAGE){
		mstateMachine.changeState(&STATE_CONFIG_VOLTAGE);
	} else if (item->id == MCURRENT){
		log_info(debug_main, "current menu");
		mstateMachine.changeState(&STATE_CONFIG_CURRENT);
	} else if (item->id == MCALIBRATE){
		power.calibrate();
	}
}

void processMainMenu(){
	if (encoder.is_right()){
		mstateMachine.changeState(&STATE_IDLE);
		menu.next();
		idle_timer.start();
	}
	if (encoder.is_left()){
		mstateMachine.changeState(&STATE_IDLE);
		menu.prev();
		idle_timer.start();
	}
	if (enter_button.pressed()){
		idle_timer.start();
		mstateMachine.changeState(&STATE_IDLE);
		log_info(debug_main, "Entering menu item: " + menu.findCurrent()->caption);
		if (!menu.enter()){
			processMenuItemEnter(menu.findCurrent());
		};
	}
	if (mstateMachine.current == &STATE_IDLE){
		menuDisplay.poll();
	}
	
}
void checkExitToMainMenu(){
	if (enter_button.pressed()){
		log_info(debug_main, "Exiting");
		menuDisplay.needRefresh = true;
		mstateMachine.changeState(&STATE_IDLE);
	}
}


void processStateMachine(){
	if (mstateMachine.current == &STATE_IDLE || mstateMachine.current == &STATE_DISPLAY_INFO){
		processMainMenu();
	} else if (mstateMachine.current == &STATE_CONFIG_VOLTAGE){
		voltage.poll();
		checkExitToMainMenu();
	} else if (mstateMachine.current == &STATE_CONFIG_CURRENT){
		current.poll();
		checkExitToMainMenu();
	}
}

void loop(){
	enter_button.poll();
	encoder.poll();
	displayInfo.c = 0;
	displayInfo.v = power.get_current_voltage();
	power.target_voltage = voltage.current;
	power.poll();
	if (idle_timer.poll()){
		log_info(debug_main, "showing display");
		mstateMachine.changeState(&STATE_DISPLAY_INFO);
	}
	if (mstateMachine.current == &STATE_DISPLAY_INFO){	
		displayInfo.poll();
	}
	processStateMachine();
	if (mstateMachine.current == &STATE_IDLE){
		idle_timer.start_if_not();
	} else {
		idle_timer.stop();
	}
	if (mstateMachine.isChanged()){
		log_info(debug_main, "State was changed");
		display.refresh_all();
		mstateMachine.unHandledStateChange();
	}
	updateVoltageAndCurrent();
}