#include <Arduino.h>
#include "config.h"
#include "display/liquid_crystal_display2x2.h"
#include "debug.h"
#include "util/util.h"
#include "display/menu_item.h"
#include "display/menu.h"
#include "display/MenuDisplay.h"
#include "encoder/encoder.h"


enum {Root, Main, Help, About, M1, M2, M3};
MenuItem items[] = {
	MenuItem("Main", Root, Main),
	MenuItem("Help", Root, Help),
	MenuItem("1", Root, M1),
	MenuItem("2", Root, M2),
	MenuItem("3", Root, M3),
	MenuItem("About", Help, About)
};
Menu<array_len(items)> menu(items);
Display display;
Debug debug("Main");
MenuDisplay<array_len(items)> menuDisplay(display, menu);
Encoder encoder(ENCODER_CLK, ENCODER_DT);

void setup(){
	debug.info("Initializing (" + String(menu.getSize()) + ")");
	display.init();
	menu.init();
	debug.info("Init complete");
}

void loop(){
	encoder.poll();
	menuDisplay.poll();

	if (encoder.is_right()){
		debug.info("Clockwise");
		menu.next();
	}
	if (encoder.is_left()){
		debug.info("Counterclockwise:");
		menu.prev();
	}
}