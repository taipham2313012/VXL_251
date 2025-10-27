/*
 * function.c
 *
 *  Created on: Sep 18, 2025
 *      Author: PHAM TAN TAI
 */

#ifndef SRC_FUNCTION_C_
#define SRC_FUNCTION_C_

#include "timer.h"
#include "button_n_display.h"
 int timer0_counter = 0;
 int timer0_flag = 0;
 int timer0_inni = 0;

 int timer1_counter = 0;
 int timer1_flag = 0;
 int timer1_inni = 0;

 int timer2_counter = 0;
 int timer2_flag = 0;
 int timer2_inni = 0;

int traffic_timer_counter = 0;
int traffic_timer_flag = 0;
int traffic_timer_inni = 0;

int modify_timer_counter = 0;
int modify_timer_flag = 0;
int modify_timer_inni = 0;


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
 void set_Timer2(int duration2){
	 timer2_inni = duration2 /TIMER_CYCLE;
	 timer2_counter = timer2_inni;
 	 timer2_flag = 0;
 	 }
 void set_Traffic_Timer(int duration){
	 traffic_timer_inni = duration /TIMER_CYCLE;
	 traffic_timer_counter = traffic_timer_inni;
	 traffic_timer_flag = 0;
  	 }
 void set_Modify_Timer(int duration){
 	 modify_timer_inni = duration /TIMER_CYCLE;
 	 modify_timer_counter = modify_timer_inni;
 	 modify_timer_flag = 0;
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
	 if(timer2_counter > 0){
		 timer2_counter--;
		 if(timer2_counter == 0) {
			 timer2_flag = 1;
			 timer2_counter = timer2_inni;
		 }
	 }
	 if(traffic_timer_counter > 0){
		 traffic_timer_counter--;
		 if(traffic_timer_counter == 0) {
			 traffic_timer_flag = 1;
			 traffic_timer_counter = traffic_timer_inni;
		 }
	 }
	 if(modify_timer_counter > 0){
	 		 modify_timer_counter--;
	 		 if(modify_timer_counter == 0) {
	 			 modify_timer_flag = 1;
	 			 modify_timer_counter = modify_timer_inni;
	 		 }
	 	 }
 }


#endif /* SRC_FUNCTION_C_ */
