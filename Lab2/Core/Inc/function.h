/*
 * function.h
 *
 *  Created on: Sep 18, 2025
 *      Author: PHAM TAN TAI
 */

#ifndef INC_FUNCTION_H_
#define INC_FUNCTION_H_

void display7SEG(int num);
extern int timer0_flag;
extern int timer1_flag;
extern int timer2_flag;
extern int timer3_flag;
extern int timer4_flag;

void set_Timer0(int duration0);
void set_Timer1(int duration1);
void set_Timer2(int duration2);
void set_Timer3(int duration3);
void set_Timer4(int duration4);

void timer_run();

#endif /* INC_FUNCTION_H_ */
