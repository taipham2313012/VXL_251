/*
 * fsm.c
 *
 * Created on: Nov 21, 2025
 * Author: PHAM TAN TAI
 */
#include "fsm.h"
#include "timer.h"
#include <stdio.h> // Required for sprintf

// FSM State Variables
ParserState parser_state = CMD_INIT;
uint8_t command_flag = 0;
CommandType command_data = NO_CMD;

UartState uart_state = UART_IDLE;

// Data Variables
uint32_t ADC_value = 0;
char str_buffer[30];
void read_adc(){
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, 100);
	ADC_value = HAL_ADC_GetValue(&hadc1); // Update the variable here!
	HAL_ADC_Stop(&hadc1);
}
/* USER CODE BEGIN 0 */
void command_parser_fsm() {
    // Get the last received character.
    uint8_t current_char;
    if (index_buffer == 0) {
        current_char = buffer[MAX_BUFFER_SIZE - 1];
    } else {
        current_char = buffer[index_buffer - 1];
    }

    switch (parser_state) {
        case CMD_INIT:
            if (current_char == '!') parser_state = CMD_WAIT_R_OR_O;
            break;

        case CMD_WAIT_R_OR_O:
            if (current_char == 'R') parser_state = CMD_WAIT_S;
            else if (current_char == 'O') parser_state = CMD_WAIT_K;
            else if (current_char == '!') parser_state = CMD_WAIT_R_OR_O;
            else parser_state = CMD_INIT;
            break;

        case CMD_WAIT_S:
            if (current_char == 'S') parser_state = CMD_WAIT_T;
            else parser_state = (current_char == '!') ? CMD_WAIT_R_OR_O : CMD_INIT;
            break;

        case CMD_WAIT_T:
            if (current_char == 'T') {
                parser_state = CMD_WAIT_HASH;
                command_data = RST_CMD;
            } else {
                parser_state = (current_char == '!') ? CMD_WAIT_R_OR_O : CMD_INIT;
            }
            break;

        case CMD_WAIT_K:
            if (current_char == 'K') {
                parser_state = CMD_WAIT_HASH;
                command_data = OK_CMD;
            } else {
                parser_state = (current_char == '!') ? CMD_WAIT_R_OR_O : CMD_INIT;
            }
            break;

        case CMD_WAIT_HASH:
            if (current_char == '#') {
                command_flag = 1;
                parser_state = CMD_INIT;
            } else {
                parser_state = (current_char == '!') ? CMD_WAIT_R_OR_O : CMD_INIT;
            }
            break;

        default:
            parser_state = CMD_INIT;
            break;
    }
}

void uart_communiation_fsm() {
    switch (uart_state) {
        case UART_IDLE:
            // Wait for !RST# command
            if (command_flag == 1 && command_data == RST_CMD) {
                command_flag = 0;

                // Read ADC
                read_adc();

                uart_state = UART_SEND_DATA;
            }
            break;

        case UART_SEND_DATA: {
            // 1. Send the Data Packet
            int len = sprintf(str_buffer, "!ADC=%lu#\r\n", (unsigned long)ADC_value);
            HAL_UART_Transmit(&huart2, (uint8_t*)str_buffer, len, 1000);

            // 2. Start software timer (3000ms)
            set_Timer0(3000);

            uart_state = UART_WAIT_ACK;
            break;
        }

        case UART_WAIT_ACK:
            // Priority 1: Got !OK#
            if (command_flag == 1 && command_data == OK_CMD) {
                command_flag = 0;
                set_Timer0(0); // Stop timer
                uart_state = UART_IDLE;
            }
            // Priority 2: New !RST# request
            else if (command_flag == 1 && command_data == RST_CMD) {
                command_flag = 0;
                set_Timer0(0);

                // Re-read ADC immediately
                read_adc();

                uart_state = UART_SEND_DATA;
            }
            // Priority 3: Timeout
            else if (timer0_flag == 1) {
                uart_state = UART_SEND_DATA; // Re-transmit
            }
            break;

        default:
            uart_state = UART_IDLE;
            break;
    }
}
/* USER CODE END 0 */
