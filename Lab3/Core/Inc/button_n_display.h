/*
 * button.h
 *
 *  Created on: 15 Oct 2025
 *      Author: PHAM TAN TAI
 */
#include "main.h"
#ifndef INC_BUTTON_N_DISPLAY_H_
#define INC_BUTTON_N_DISPLAY_H_

void set_road1_leds(int red, int yellow, int green);
void set_road2_leds(int red, int yellow, int green);
void display7SEG(int num);
void update_buffer(int first, int last);

extern const int MAX_LED;
extern int index_led;
extern int led_buffer[4];

void update7SEG(int index);

#define NUM_OF_BUTTONS 3
#define DEBOUNCE_TIME_MS 50 // Button must be stable for 50ms

// States for our button state machine
typedef enum {
    BUTTON_STATE_IDLE,
    BUTTON_STATE_DEBOUNCING,
    BUTTON_STATE_PRESSED,
    BUTTON_STATE_RELEASED
} ButtonState_t;

// A struct to hold all info for one button
typedef struct {
    ButtonState_t state;
    GPIO_TypeDef* port;
    uint16_t pin;
    uint32_t last_tick; // For timing the debounce
    int is_pressed_flag; // A flag for the main FSM to read
} Button_t;
extern Button_t buttons[NUM_OF_BUTTONS];

void button_init();
void button_scan(Button_t *button);

#endif /* INC_BUTTON_N_DISPLAY_H_ */
