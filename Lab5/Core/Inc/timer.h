/*
 * function.h
 *
 *  Created on: Sep 18, 2025
 *      Author: PHAM TAN TAI
 */
#ifndef INC_FUNCTION_H_
#define INC_FUNCTION_H_

#define TIMER_CYCLE 10
extern int timer0_flag;
extern int timer1_flag;

void set_Timer0(int duration0);
void set_Timer1(int duration1);

void timer_run();

#endif /* INC_FUNCTION_H_ */
