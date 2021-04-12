#pragma once

#define CONFIG_SERIAL_SPEED 115200
#define CONFIG_DEBUG_ENABLED 1
#define CONFIG_DEBUG_LEVEL INFO


//          Display configuration

#define CONF_DISPLAY_ADDRESS 63
//#define CONF_DISPLAY_ADDRESS 0x3F
#define CONF_DISPLAY_X_SIZE 16
#define CONF_DISPLAY_Y_SIZE 2

//  Buttons

#define CONF_BUTTON_ON_OFF 2
#define CONF_BUTTON_1 3
#define CONF_BUTTON_2 4

//          Voltage control configuration
#define CONF_DAC_ADDRESS 0x60
#define CONF_VOLTAGE_CHECK_PIN A0
#define CONF_CURRENT_CHECK_PIN A6
#define CONF_MIN_VOLTAGE 1.22
//#define CONF_MAX_VOLTAGE 12
#define CONF_MAX_CURRENT 8
#define CONF_VOLTAGE_THRESHOLD 0.1
#define CONF_MAX_ADC 1024
#define CONF_CURRENT_DIVIDER_K 1
#define CONF_CURRENT_ACS_OFFSET 2.39

#define CONF_CURRENT_DIFF_VOLTAGE_INCREMENT_K 1

//          voltage resistors
#define CONF_VOLTAGE_RESISTORS 0

#if CONF_VOLTAGE_RESISTORS == 1
    #define CONF_VOLTAGE_RESISTOR_1 81850
    #define CONF_VOLTAGE_RESISTOR_2 15050
    #define CONF_VOLTAGE_DIVIDER_K ((float)CONF_VOLTAGE_RESISTOR_1 / CONF_VOLTAGE_RESISTOR_2)
#else
    //#define CONF_VOLTAGE_DIVIDER_K (13.66 / 2.12)
    #define CONF_VOLTAGE_DIVIDER_K (8.73 / 1.36)
#endif

#define CONF_VREF_VOLTAGE   4.81


#define CONF_POWER_RELAY_PIN 14


#define ENCODER_CLK 6
#define ENCODER_DT 5

#define ENTER_BUTTON_PIN 7

#define MAX_VOLTAGE 20

