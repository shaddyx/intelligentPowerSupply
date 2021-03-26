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
#include "state/main_state_machine.h"

enum {Root, Main, Help, About, M1, M2, M3, MM1, MM2, MM3};
MenuItem items[] = {
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
MainStateMachine mstateMachine;

void setup(){
	debug.info("Initializing (" + String(menu.getSize()) + ")");
	display.init();
	menu.init();
	enter_button.poll();
	mstateMachine.init();
	debug.info("Init complete");
}

void loop(){
	enter_button.poll();
	encoder.poll();
	menuDisplay.poll();
	menu.poll();

	if (encoder.is_right()){
		debug.info("Clockwise");
		menu.next();
	}
	if (encoder.is_left()){
		debug.info("Counterclockwise:");
		menu.prev();
	}
	if (enter_button.pressed()){
		debug.info("Entering menu item: " + menu.findCurrent()->caption);
		menu.enter();
	}

}