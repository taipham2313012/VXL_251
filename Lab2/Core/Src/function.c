/*
 * function.c
 *
 *  Created on: Sep 18, 2025
 *      Author: PHAM TAN TAI
 */

#ifndef SRC_FUNCTION_C_
#define SRC_FUNCTION_C_

#include "function.h"


 int timer0_counter = 0;
 int timer0_flag = 0;

 int timer1_counter = 0;
 int timer1_flag = 0;

 int timer2_counter = 0;
 int timer2_flag = 0;

 int TIMER_CYCLE = 10;
 void set_Timer0(int duration0){
	 timer0_counter = duration0 /TIMER_CYCLE;
	 timer0_flag = 0;
	 }
 void set_Timer1(int duration1){
 	 timer1_counter = duration1 /TIMER_CYCLE;
 	 timer1_flag = 0;
 	 }
 void set_Timer2(int duration2){
 	 timer2_counter = duration2 /TIMER_CYCLE;
 	 timer2_flag = 0;
 	 }
 void timer_run(){
	 if(timer0_counter > 0){
		 timer0_counter--;
		 if(timer0_counter == 0) timer0_flag = 1;
	 }
	 if(timer1_counter > 0){
		 timer1_counter--;
		 if(timer1_counter == 0) timer1_flag = 1;
	 }
	 if(timer2_counter > 0){
		 timer2_counter--;
		 if(timer2_counter == 0) timer2_flag = 1;
	 }
 }

#endif /* SRC_FUNCTION_C_ */
