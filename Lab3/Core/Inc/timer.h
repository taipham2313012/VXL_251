/*
 * function.h
 *
 *  Created on: Sep 18, 2025
 *      Author: PHAM TAN TAI
 */
#include "main.h"
#ifndef INC_FUNCTION_H_
#define INC_FUNCTION_H_

extern int timer0_flag;
extern int timer1_flag;
extern int timer2_flag;
extern int traffic_timer_flag;
extern int modify_timer_flag;

void set_Timer0(int duration0);
void set_Timer1(int duration1);
void set_Timer2(int duration2);
void set_Traffic_Timer(int duration);
void set_Modify_Timer(int duration);

void timer_run();

#endif /* INC_FUNCTION_H_ */
