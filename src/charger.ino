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

enum {Root, Main, Help, About, M1, M2, M3, MM1, MM2, MM3, MVOLTAGE, MCURRENT};
MenuItem items[] = {
	MenuItem("V", Root, MVOLTAGE),
	MenuItem("C", Root, MCURRENT),
	MenuItem("Main", Root, Main, true),
		MenuItem("..", Main, Root, true),
		MenuItem("M1", Main, MM1),
		MenuItem("M2", Main, MM2),
		MenuItem("M3", Main, MM3),
	MenuItem("Help", Root, Help, true),
		MenuItem("..", Help, Root, true),
		MenuItem("About", Help, About),
	MenuItem("1", Root, M1),
	MenuItem("2", Root, M2),
	MenuItem("3", Root, M3),
};

Menu<array_len(items)> menu(items);
Display display;
Debug debug("Main");
MenuDisplay<array_len(items)> menuDisplay(display, menu);
Encoder encoder(ENCODER_CLK, ENCODER_DT);
SupplyButton enter_button(ENTER_BUTTON_PIN);

State STATE_IDLE;
State STATE_CONFIG_VOLTAGE;
State STATE_CONFIG_CURRENT;

StateMachine mstateMachine(&STATE_IDLE);

DisplayParam<float> voltage(String("Voltage"), &display, &encoder, 0, 0, 25, 0.1, 1);
DisplayParam<float> current(String("Current"), &display, &encoder, 0, 0, 10, 0.1, 1);

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

void setup(){
	debug.info("Initializing (" + String(menu.getSize()) + ")");
	display.init();
	menu.init();
	enter_button.poll();
	mstateMachine.init();
	STATE_IDLE.transitions.add(&STATE_CONFIG_VOLTAGE);
	STATE_IDLE.transitions.add(&STATE_CONFIG_CURRENT);
	STATE_CONFIG_VOLTAGE.transitions.add(&STATE_IDLE);
	STATE_CONFIG_CURRENT.transitions.add(&STATE_IDLE);

	updateVoltageAndCurrent();
	debug.info("Init complete");
}

void processMenuItemEnter(MenuItem * item){
	if (item -> id == MVOLTAGE){
		mstateMachine.changeState(&STATE_CONFIG_VOLTAGE);
	} else if (item->id == MCURRENT){
		debug.info("current menu");
		mstateMachine.changeState(&STATE_CONFIG_CURRENT);
	}
}

void processMainMenu(){
	if (encoder.is_right()){
		menu.next();
	}
	if (encoder.is_left()){
		menu.prev();
	}
	if (enter_button.pressed()){
		debug.info("Entering menu item: " + menu.findCurrent()->caption);
		if (!menu.enter()){
			processMenuItemEnter(menu.findCurrent());
		};
	}
	menuDisplay.poll();
}
void checkExitToMain(){
	if (enter_button.pressed()){
		debug.info("Exiting");
		menuDisplay.needRefresh = true;
		mstateMachine.changeState(&STATE_IDLE);
	}
}

void processStateMachine(){
	if (mstateMachine.current == &STATE_IDLE){
		processMainMenu();
	} else if (mstateMachine.current == &STATE_CONFIG_VOLTAGE){
		voltage.poll();
		checkExitToMain();
	} else if (mstateMachine.current == &STATE_CONFIG_CURRENT){
		current.poll();
		checkExitToMain();
	}
}

void loop(){
	enter_button.poll();
	encoder.poll();
	processStateMachine();
	if (mstateMachine.isChanged()){
		debug.info("State was changed");
		voltage.needRefresh = true;
		current.needRefresh = true;
		menuDisplay.needRefresh = true;
		mstateMachine.unHandledStateChange();
	}
	updateVoltageAndCurrent();
}