/*
 * fsm.h
 *
 * Created on: Nov 21, 2025
 * Author: PHAM TAN TAI
 */

#ifndef INC_FSM_H_
#define INC_FSM_H_

#include "main.h"
#include <stdint.h>

/* USER CODE BEGIN PV */
// Defined in the PDF Lab Manual
#define MAX_BUFFER_SIZE 30

// Command Parser FSM States
typedef enum {
    CMD_INIT,
    CMD_WAIT_R_OR_O,     // Received '!'
    CMD_WAIT_S,          // Received '!R'
    CMD_WAIT_T,          // Received '!RS'
    CMD_WAIT_K,          // Received '!O'
    CMD_WAIT_HASH        // Received '!RST' or '!OK'
} ParserState;

// Command Types
typedef enum {
    NO_CMD,
    RST_CMD,
    OK_CMD
} CommandType;

// UART FSM States
typedef enum {
    UART_IDLE,
    UART_SEND_DATA,
    UART_WAIT_ACK
} UartState;

// External Variables (Defined in main.c or fsm.c)
extern uint8_t temp;
extern uint8_t buffer[MAX_BUFFER_SIZE];
extern uint8_t index_buffer;
extern uint8_t buffer_flag;

extern ADC_HandleTypeDef hadc1;
extern UART_HandleTypeDef huart2;
extern TIM_HandleTypeDef htim2;

// Function Prototypes (Crucial for main.c to see them)
void command_parser_fsm(void);
void uart_communiation_fsm(void);

/* USER CODE END PV */

#endif /* INC_FSM_H_ */
