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

// Call this function once in your main()
void button_init(void);

// Call this function in your 10ms timer interrupt
void button_reading(void);

// Returns 1 if the button is *currently* held down
unsigned char is_button_pressed(unsigned char index);

// Returns 1 *only on the first tick* the button is pressed (rising edge)
unsigned char is_button_pressed_edge(unsigned char index);

// Returns 1 if the button has been held for 1 seconds
unsigned char is_button_pressed_1s(unsigned char index);

#endif /* INC_BUTTON_N_DISPLAY_H_ */
