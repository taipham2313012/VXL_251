/*
 * fsm.h
 *
 *  Created on: 15 Oct 2025
 *      Author: PHAM TAN TAI
 */

#ifndef INC_FSM_H_
#define INC_FSM_H_

// Main operating modes
typedef enum {
	INIT,
    MODE_NORMAL,
    MODE_MODIFY_RED,
	MODE_MODIFY_YELLOW,
	MODE_MODIFY_GREEN
} AppMode_t;

// Sub-states for the normal traffic light sequence
typedef enum {
	INIT_LIGHT,
    STATE_R1_GREEN_R2_RED,
    STATE_R1_AMBER_R2_RED,
    STATE_R1_RED_R2_GREEN,
    STATE_R1_RED_R2_AMBER
} TrafficState_t;

int current_timer;
// --- Function Prototypes ---

// Initializes the FSM to its default state
void fsm_init(void);

// Runs one iteration of the FSM
void fsm_run(void);


#endif /* INC_FSM_H_ */
