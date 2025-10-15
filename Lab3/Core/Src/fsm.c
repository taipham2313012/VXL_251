/*
 * fsm.c
 *
 *  Created on: 15 Oct 2025
 *      Author: PHAM TAN TAI
 */

#include "fsm.h"
#include "timer.h"
#include "button_n_display.h"

// --- State and Duration Variables ---
static AppMode_t current_mode;
static TrafficState_t traffic_state;

// Default durations (in seconds)
int red_duration = 5;
int yellow_duration = 2;
int green_duration = 3;

// Temporary variable for editing in Mode 2
static int temp_duration = 0;


// --- FSM Helper Functions ---
int current_time = 0;
// Handles the logic for the normal traffic light sequence
static void run_normal_mode() {
    // The nested FSM for traffic light control

    switch (traffic_state) {
    	case INIT_LIGHT:
    		set_Traffic_Timer(1000);
    		current_time = green_duration;
    		traffic_state = STATE_R1_GREEN_R2_RED;
    		break;

        case STATE_R1_GREEN_R2_RED:
            // ACTION: Control LEDs for this state.
        	set_road1_leds(0, 0, 1);
        	set_road2_leds(1, 0, 0);
            // TRANSITION: Check if the green duration has passed.
            if (traffic_timer_flag == 1 ) {
            	traffic_timer_flag = 0;
            	set_Traffic_Timer(1000);
            	current_time --;
            }
            if (current_time <= 0){
                current_time = yellow_duration;
                traffic_state = STATE_R1_AMBER_R2_RED;
                //set_traffic_timer(amber_duration); // Set timer for next state
            }
            update_buffer(current_time + yellow_duration, current_time);
            break;

        case STATE_R1_AMBER_R2_RED:
            // ACTION: Control LEDs for this state.
        	set_road1_leds(0, 1, 0);
			set_road2_leds(1, 0, 0);
            // TRANSITION: Check if the amber duration has passed.
			if (traffic_timer_flag == 1 ) {
				traffic_timer_flag = 0;
				set_Traffic_Timer(1000);
            	current_time --;
			}
			if (current_time <= 0){
				current_time = green_duration;
				traffic_state = STATE_R1_RED_R2_GREEN;
				//set_traffic_timer(amber_duration); // Set timer for next state
			}
			update_buffer(current_time, current_time);
            break;

        case STATE_R1_RED_R2_GREEN:
            // ACTION: Control LEDs for this state.
        	set_road1_leds(1, 0, 0);
			set_road2_leds(0, 0, 1);
			// TRANSITION: Check if the amber duration has passed.
			if (traffic_timer_flag == 1 ) {
				traffic_timer_flag = 0;
				set_Traffic_Timer(1000);
            	current_time --;
			}
			if (current_time <= 0){
				current_time = yellow_duration;
				traffic_state = STATE_R1_RED_R2_AMBER;
				//set_traffic_timer(amber_duration); // Set timer for next state
			}
			update_buffer(current_time, current_time + yellow_duration);
            break;

        case STATE_R1_RED_R2_AMBER:
        	set_road1_leds(1, 0, 0);
			set_road2_leds(0, 1, 0);
            // ACTION: Control LEDs for this state.
            // TRANSITION: Check if the amber duration has passed.
			if (traffic_timer_flag == 1 ) {
				traffic_timer_flag = 0;
				set_Traffic_Timer(1000);
            	current_time --;
			}
			if (current_time <= 0){
				current_time = green_duration;
				traffic_state = STATE_R1_GREEN_R2_RED;
				//set_traffic_timer(amber_duration); // Set timer for next state
			}
			update_buffer(current_time, current_time);
            break;
        default:
        	break;

    }
}

// Handles the logic for modifying the red light duration
static void run_modify_red_mode() {
    // ACTION: Blink all Red LEDs (handled by a separate blinking timer).
    // ACTION: Display "02" and temp_duration on 7-segment LEDs.
	if (modify_timer_flag == 1 ) {
		modify_timer_flag = 0;
		set_Modify_Timer(500);
		update_buffer(2, temp_duration);
		HAL_GPIO_TogglePin(RED1_GPIO_Port, RED1_Pin);
		HAL_GPIO_TogglePin(RED2_GPIO_Port, RED2_Pin);
	}
    // Check for MODIFY button press to increase value
    if (buttons[1].is_pressed_flag) { // Assuming button 1 is MODIFY
    	buttons[1].is_pressed_flag = 0;
        temp_duration++;
        if (temp_duration > 99) {
            temp_duration = 1;
        }
    }

    // Check for SET button press to save and return to normal
    if (buttons[2].is_pressed_flag) { // Assuming button 2 is SET
    	buttons[2].is_pressed_flag = 0;
        red_duration = temp_duration; // Save the new value
    }
}

static void run_modify_yellow_mode() {
    // ACTION: Blink all Red LEDs (handled by a separate blinking timer).
    // ACTION: Display "02" and temp_duration on 7-segment LEDs.
	if (modify_timer_flag == 1 ) {
		modify_timer_flag = 0;
		set_Modify_Timer(500);
		update_buffer(3, temp_duration);
		HAL_GPIO_TogglePin(YEL1_GPIO_Port, YEL1_Pin);
		HAL_GPIO_TogglePin(YEL2_GPIO_Port, YEL2_Pin);
	}
    // Check for MODIFY button press to increase value
    if (buttons[1].is_pressed_flag) { // Assuming button 1 is MODIFY
    	buttons[1].is_pressed_flag = 0;
        temp_duration++;
        if (temp_duration > 99) {
            temp_duration = 1;
        }
    }

    // Check for SET button press to save and return to normal
    if (buttons[2].is_pressed_flag) { // Assuming button 2 is SET
    	buttons[2].is_pressed_flag = 0;
        yellow_duration = temp_duration; // Save the new value
    }
}

static void run_modify_green_mode() {
    // ACTION: Blink all Red LEDs (handled by a separate blinking timer).
    // ACTION: Display "02" and temp_duration on 7-segment LEDs.
	if (modify_timer_flag == 1 ) {
		modify_timer_flag = 0;
		set_Modify_Timer(500);
		update_buffer(4, temp_duration);
		HAL_GPIO_TogglePin(GRN1_GPIO_Port, GRN1_Pin);
		HAL_GPIO_TogglePin(GRN2_GPIO_Port, GRN2_Pin);
	}
    // Check for MODIFY button press to increase value
    if (buttons[1].is_pressed_flag) { // Assuming button 1 is MODIFY
    	buttons[1].is_pressed_flag = 0;
        temp_duration++;
        if (temp_duration > 99) {
            temp_duration = 1;
        }
    }

    // Check for SET button press to save and return to normal
    if (buttons[2].is_pressed_flag) { // Assuming button 2 is SET
    	buttons[2].is_pressed_flag = 0;
        green_duration = temp_duration; // Save the new value
    }
}


// --- Main FSM Functions ---

void fsm_init(void) {
    current_mode = INIT;
    traffic_state = INIT_LIGHT;
}

void fsm_run(void) {
    // --- Global State Transitions (Mode Switching) ---
    // Check for MODE button press (Button 0)
	if (buttons[0].is_pressed_flag) {
		buttons[0].is_pressed_flag = 0; // Clear the flag
		if (current_mode == MODE_NORMAL) {
			current_mode = MODE_MODIFY_RED;
			// On entry to modify mode, copy the current value to the temp variable
			set_Modify_Timer(500);
			set_road1_leds(0, 0, 0);
			set_road2_leds(0, 0, 0);
			temp_duration = red_duration;
		} else if (current_mode == MODE_MODIFY_RED) {
			current_mode = MODE_MODIFY_YELLOW;
			// On entry to modify mode, copy the current value to the temp variable
			set_Modify_Timer(500);
			set_road1_leds(0, 0, 0);
			set_road2_leds(0, 0, 0);
			temp_duration = yellow_duration;
		}
		else if (current_mode == MODE_MODIFY_YELLOW) {
			current_mode = MODE_MODIFY_GREEN;
			// On entry to modify mode, copy the current value to the temp variable
			set_Modify_Timer(500);
			set_road1_leds(0, 0, 0);
			set_road2_leds(0, 0, 0);
			temp_duration = green_duration;
		}
		else if (current_mode == MODE_MODIFY_GREEN) {
			current_mode = MODE_NORMAL; // Cycle back to normal mode
			// Re-initialize the normal mode to a safe state
			traffic_state = INIT_LIGHT;
		}
    }

    // --- State-Specific Logic ---
    switch (current_mode) {
		case INIT:
			current_mode = MODE_NORMAL;
			break;

        case MODE_NORMAL:
            run_normal_mode();
            break;

        case MODE_MODIFY_RED:
            run_modify_red_mode();
            break;
        case MODE_MODIFY_YELLOW:
			run_modify_yellow_mode();
			break;
        case MODE_MODIFY_GREEN:
			run_modify_green_mode();
			break;
    }
}
