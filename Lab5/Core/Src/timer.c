/*
 * function.c
 *
 *  Created on: Sep 18, 2025
 *      Author: PHAM TAN TAI
 */

#ifndef SRC_FUNCTION_C_
#define SRC_FUNCTION_C_

#include "timer.h"
 int timer0_counter = 0;
 int timer0_flag = 0;
 int timer0_inni = 0;

 int timer1_counter = 0;
 int timer1_flag = 0;
 int timer1_inni = 0;



 void set_Timer0(int duration0){
	 timer0_inni = duration0 /TIMER_CYCLE;
	 timer0_counter = timer0_inni;
	 timer0_flag = 0;
	 }
 void set_Timer1(int duration1){
	 timer1_inni = duration1 /TIMER_CYCLE;
	 timer1_counter = timer1_inni;
 	 timer1_flag = 0;
 	 }
 void timer_run(){
	 if(timer0_counter > 0){
		 timer0_counter--;
		 if(timer0_counter == 0) {
			 timer0_flag = 1;
			 timer0_counter = timer0_inni;
		 }
	 }
	 if(timer1_counter > 0){
		 timer1_counter--;
		 if(timer1_counter == 0) {
			 timer1_flag = 1;
			 timer1_counter = timer1_inni;
		 }
	 }
 }


#endif /* SRC_FUNCTION_C_ */
