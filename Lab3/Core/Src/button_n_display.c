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

Button_t buttons[NUM_OF_BUTTONS];

 // Call this function once in your main() before the while(1) loop
 void button_init() {
     // Button 1 (MODE)
     buttons[0].port = BTN0_GPIO_Port;
     buttons[0].pin = BTN0_Pin;
     buttons[0].state = BUTTON_STATE_IDLE;
     buttons[0].is_pressed_flag = 0;

     // Button 2 (MODIFY)
     buttons[1].port = BTN1_GPIO_Port;
     buttons[1].pin = BTN1_Pin;
     buttons[1].state = BUTTON_STATE_IDLE;
     buttons[1].is_pressed_flag = 0;

     // Button 3 (SET)
     buttons[2].port = BTN2_GPIO_Port;
     buttons[2].pin = BTN2_Pin;
     buttons[2].state = BUTTON_STATE_IDLE;
     buttons[2].is_pressed_flag = 0;
 }

 // Pass a pointer to the button you want to process
 void button_scan(Button_t *button) {
     // We read the button as "pressed" when the pin is LOW (0)
     int is_pin_low = (HAL_GPIO_ReadPin(button->port, button->pin) == GPIO_PIN_RESET);

     switch (button->state) {
         case BUTTON_STATE_IDLE:
             if (is_pin_low) {
                 // Pin is low, start debouncing
                 button->state = BUTTON_STATE_DEBOUNCING;
                 button->last_tick = HAL_GetTick(); // Start the timer
             }
             break;

         case BUTTON_STATE_DEBOUNCING:
             if (is_pin_low) {
                 // Check if the debounce time has passed
                 if (HAL_GetTick() - button->last_tick >= DEBOUNCE_TIME_MS) {
                     // Stable press confirmed!
                     button->state = BUTTON_STATE_PRESSED;
                     // **This is the key part: set a flag for the main FSM**
                     button->is_pressed_flag = 1;
                 }
             } else {
                 // If it bounces back high, reset to idle
                 button->state = BUTTON_STATE_IDLE;
             }
             break;

         case BUTTON_STATE_PRESSED:
             // Wait here until the button is released
             if (!is_pin_low) {
                 button->state = BUTTON_STATE_RELEASED;
             }
             break;

         case BUTTON_STATE_RELEASED:
             // This state confirms the release and prevents multiple reads
             // for a single long press. We just go back to idle.
             button->state = BUTTON_STATE_IDLE;
             break;
     }
 }

#endif /* SRC_BUTTON_N_DISPLAY_C_ */
