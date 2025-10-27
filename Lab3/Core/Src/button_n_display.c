/*
 * button_n_display.c
 *
 *  Created on: 15 Oct 2025
 *      Author: PHAM TAN TAI
 */
#include "button_n_display.h"

#ifndef SRC_BUTTON_N_DISPLAY_C_
#define SRC_BUTTON_N_DISPLAY_C_

void set_road1_leds(int red, int yellow, int green) {
	HAL_GPIO_WritePin(RED1_GPIO_Port, RED1_Pin, red ? GPIO_PIN_RESET : GPIO_PIN_SET);
	HAL_GPIO_WritePin(YEL1_GPIO_Port, YEL1_Pin, yellow ? GPIO_PIN_RESET : GPIO_PIN_SET);
	HAL_GPIO_WritePin(GRN1_GPIO_Port, GRN1_Pin, green ? GPIO_PIN_RESET : GPIO_PIN_SET);
	}

// Control Road 2 LEDs (Red, Yellow, Green)
void set_road2_leds(int red, int yellow, int green) {
	HAL_GPIO_WritePin(RED2_GPIO_Port, RED2_Pin, red ? GPIO_PIN_RESET : GPIO_PIN_SET);
	HAL_GPIO_WritePin(YEL2_GPIO_Port, YEL2_Pin, yellow ? GPIO_PIN_RESET : GPIO_PIN_SET);
	HAL_GPIO_WritePin(GRN2_GPIO_Port, GRN2_Pin, green ? GPIO_PIN_RESET : GPIO_PIN_SET);
	}

void display7SEG(int num) {
	const uint8_t segmentMap[10][7] = {
		{1,1,1,1,1,1,0}, // 0
		{0,1,1,0,0,0,0}, // 1
		{1,1,0,1,1,0,1}, // 2
		{1,1,1,1,0,0,1}, // 3
		{0,1,1,0,0,1,1}, // 4
		{1,0,1,1,0,1,1}, // 5
		{1,0,1,1,1,1,1}, // 6
		{1,1,1,0,0,0,0}, // 7
		{1,1,1,1,1,1,1}, // 8
		{1,1,1,1,0,1,1}  // 9
	};

	if (num < 0 || num > 9) return; // invalid input

	HAL_GPIO_WritePin(SEG0_GPIO_Port, SEG0_Pin, segmentMap[num][0] ? GPIO_PIN_RESET : GPIO_PIN_SET);
	HAL_GPIO_WritePin(SEG1_GPIO_Port, SEG1_Pin, segmentMap[num][1] ? GPIO_PIN_RESET : GPIO_PIN_SET);
	HAL_GPIO_WritePin(SEG2_GPIO_Port, SEG2_Pin, segmentMap[num][2] ? GPIO_PIN_RESET : GPIO_PIN_SET);
	HAL_GPIO_WritePin(SEG3_GPIO_Port, SEG3_Pin, segmentMap[num][3] ? GPIO_PIN_RESET : GPIO_PIN_SET);
	HAL_GPIO_WritePin(SEG4_GPIO_Port, SEG4_Pin, segmentMap[num][4] ? GPIO_PIN_RESET : GPIO_PIN_SET);
	HAL_GPIO_WritePin(SEG5_GPIO_Port, SEG5_Pin, segmentMap[num][5] ? GPIO_PIN_RESET : GPIO_PIN_SET);
	HAL_GPIO_WritePin(SEG6_GPIO_Port, SEG6_Pin, segmentMap[num][6] ? GPIO_PIN_RESET : GPIO_PIN_SET);

}

const int MAX_LED = 4;
int index_led = 0;
int led_buffer[4] = {1, 5, 3, 1};

void update_buffer(int first, int last){
	led_buffer[0] = first / 10;
	led_buffer[1] = first % 10;
	led_buffer[2] = last / 10;
	led_buffer[3] = last % 10;
}
void update7SEG(int index){
	HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, SET);
	HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, SET);
	HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, SET);
	HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, SET);
    switch (index){
        case 0:
			display7SEG(led_buffer[0]);
        	HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, RESET);
            break;
        case 1:
			display7SEG(led_buffer[1]);
			HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, RESET);
            break;
        case 2:
			display7SEG(led_buffer[2]);
			HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, RESET);
            break;
        case 3:
			display7SEG(led_buffer[3]);
			HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, RESET);
            break;
        default:
            break;
    }
}

// Change this to match your number of buttons
#define N0_OF_BUTTONS 3

// We want a 3-second hold time
#define LONG_PRESS 1000 //ms

#define BUTTON_IS_PRESSED  GPIO_PIN_RESET
#define BUTTON_IS_RELEASED GPIO_PIN_SET

// --- Buffers for debouncing ---
static GPIO_PinState buttonBuffer[N0_OF_BUTTONS];
static GPIO_PinState debounceButtonBuffer1[N0_OF_BUTTONS];
static GPIO_PinState debounceButtonBuffer2[N0_OF_BUTTONS];

// --- Buffers for edge detection ---
static GPIO_PinState buttonBuffer_last[N0_OF_BUTTONS];

// --- Buffers for long press ---
static uint8_t flagForButtonPress1s[N0_OF_BUTTONS];
static uint16_t counterForButtonPress1s[N0_OF_BUTTONS];

// --- Map your button pins here ---
static GPIO_TypeDef* button_ports[N0_OF_BUTTONS] = {
    BTN0_GPIO_Port,
    BTN1_GPIO_Port,
    BTN2_GPIO_Port
};

static uint16_t button_pins[N0_OF_BUTTONS] = {
    BTN0_Pin,
    BTN1_Pin,
    BTN2_Pin
};

// Initialization function
void button_init(void) {
    for (int i = 0; i < N0_OF_BUTTONS; i++) {
        buttonBuffer[i] = BUTTON_IS_RELEASED;
        debounceButtonBuffer1[i] = BUTTON_IS_RELEASED;
        debounceButtonBuffer2[i] = BUTTON_IS_RELEASED;
        buttonBuffer_last[i] = BUTTON_IS_RELEASED;
    }
}

// Main reading function (call from 10ms interrupt)
void button_reading(void) {
    for (char i = 0; i < N0_OF_BUTTONS; i++) {
        // 1. Debounce logic
        debounceButtonBuffer2[i] = debounceButtonBuffer1[i];
        debounceButtonBuffer1[i] = HAL_GPIO_ReadPin(button_ports[i], button_pins[i]);

        if (debounceButtonBuffer1[i] == debounceButtonBuffer2[i]) {
            buttonBuffer[i] = debounceButtonBuffer1[i];
        }

        // 2. Long press (1-second) logic
        if (buttonBuffer[i] == BUTTON_IS_PRESSED) {
            // Start counting
            if (counterForButtonPress1s[i] < LONG_PRESS/TIMER_CYCLE) {
                counterForButtonPress1s[i]++;
            } else {
                // Flag is set after 3 seconds
                flagForButtonPress1s[i] = 1;
            }
        } else {
            // Button is released, reset counters
            counterForButtonPress1s[i] = 0;
            flagForButtonPress1s[i] = 0;
        }
    }
}

// Check if button is *currently* pressed (level check)
unsigned char is_button_pressed(unsigned char index) {
    if (index >= N0_OF_BUTTONS) return 0;
    return (buttonBuffer[index] == BUTTON_IS_PRESSED);
}

// Check for 3-second hold
unsigned char is_button_pressed_1s(unsigned char index) {
    if (index >= N0_OF_BUTTONS) return 0;
    return (flagForButtonPress1s[index] == 1);
}

// Check for *one-shot* press (edge check)
unsigned char is_button_pressed_edge(unsigned char index) {
    if (index >= N0_OF_BUTTONS) return 0;

    // Check if the button is pressed *now* but was *not* pressed last time
    if (buttonBuffer[index] == BUTTON_IS_PRESSED && buttonBuffer_last[index] == BUTTON_IS_RELEASED) {
        // Update the last state to "consume" the edge
        buttonBuffer_last[index] = BUTTON_IS_PRESSED;
        return 1;
    }

    // Reset the last state when the button is released
    if (buttonBuffer[index] == BUTTON_IS_RELEASED) {
        buttonBuffer_last[index] = BUTTON_IS_RELEASED;
    }

    return 0;
}

#endif /* SRC_BUTTON_N_DISPLAY_C_ */
