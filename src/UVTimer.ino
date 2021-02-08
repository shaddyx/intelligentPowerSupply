#include <Arduino.h>
#include <JC_Button.h>
#include <Ticker.h>
// #define PIN_5M 11 
// #define PIN_2M 9

// #define PIN_5M 11 
#define PIN_2M 11
#define PIN_1M 9


#define PIN_ON_OFF 4
#define PIN_OUT 2
#define PIN_LED 6
#define LONG_PRESS 2000
#define MINUTES (long)1000 * 60

bool on = false;
bool led_state = false;
long wasPressedForLongTime = 0;

void stopTimer();
void ledTimer(){
	led_state = !led_state;
}

Button onButton(PIN_ON_OFF);
Ticker timer(stopTimer, 0, 0);
Ticker led_timer(ledTimer, 500, 1000);

void setup() {
  Serial.begin(115200);
  Serial.println("Initializing");
  pinMode(PIN_OUT, OUTPUT);
  pinMode(PIN_LED, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PIN_1M, INPUT);
  pinMode(PIN_2M, INPUT);
  digitalWrite(PIN_1M, 1);
  digitalWrite(PIN_2M, 1);
  digitalWrite(PIN_OUT, 1);
  digitalWrite(PIN_LED, 0);
  onButton.begin();
  
  Serial.println("init complete");
}

bool is2(){
	return digitalRead(PIN_2M) == 0;
}
bool is1(){
	return digitalRead(PIN_1M) == 0;
}

bool is_infinite(){
	return wasPressedForLongTime;
}

bool is5(){
	return !is1() && !is2() && !is_infinite();
}


void updateOutput(){
	digitalWrite(PIN_OUT, !on);
	digitalWrite(LED_BUILTIN, on);
	digitalWrite(PIN_LED, led_state);
}
void clearTimer(){
	if (timer.state() == RUNNING){
		Serial.println("Stopping timer");
		timer.stop();
	}
	if (led_timer.state() == RUNNING){
		Serial.println("Stopping led timer");
		led_timer.stop();
	}
	Ticker newTimer(stopTimer, 0, 0);
	Ticker newLedTimer(ledTimer, 0, 0);
	led_timer = newLedTimer;
	timer = newTimer;
}

void stopTimer(){
	on = false;
	led_state = false;
	clearTimer();
}

void startTimer(long minutes, uint32_t ledTimerInterval){
	Serial.println("Starting " + String(minutes) + " minutes timer");
	Ticker newTimer(stopTimer, minutes * MINUTES, 1);
	Ticker newLedTimer(ledTimer, ledTimerInterval, 0);
	led_timer = newLedTimer;
	timer = newTimer;
	timer.start();
	led_timer.start();
}
void toggleState(){
	on = !on;
	Serial.println("State changed: " + String(on));
	clearTimer();
	if (on){
		if (is1()){
			startTimer(1, 100);
		} else if (is5()){
			startTimer(5, 1000);
		} else if (is2()){
			startTimer(2, 300);
		} else if (is_infinite()) {
			Serial.println("Starting infinite timer");
			led_state = true;
		} else {

		}
	} else {
		led_state = false;
	}
}

void loop() {
	onButton.read();
	led_timer.update();
	timer.update();
	if (onButton.wasReleased()){
		Serial.println("Button was pressed for:" + String(wasPressedForLongTime));
		toggleState();
		delay(300);
	}
	wasPressedForLongTime = onButton.pressedFor(LONG_PRESS);
	updateOutput();
}