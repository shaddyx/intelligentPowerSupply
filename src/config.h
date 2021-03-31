#pragma once

#define CONFIG_SERIAL_SPEED 115200
#define CONFIG_DEBUG_LEVEL INFO



//          Display configuration

#define CONF_DISPLAY_ADDRESS 63
//#define CONF_DISPLAY_ADDRESS 0x3F
#define CONF_DISPLAY_X_SIZE 16
#define CONF_DISPLAY_Y_SIZE 2

//          Voltage control configuration
#define CONF_DAC_ADDRESS 0x60
#define CONF_VOLTAGE_CHECK_PIN A0
#define CONF_MIN_VOLTAGE 1.22
#define CONF_MAX_VOLTAGE 12
#define CONF_MAX_CURRENT 8
#define CONF_VOLTAGE_THRESHOLD 0.1
#define CONF_MAX_ADC 1024


#define ENCODER_CLK 6
#define ENCODER_DT 5

#define ENTER_BUTTON_PIN 7

#define MAX_VOLTAGE 20

