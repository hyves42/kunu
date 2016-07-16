/* 
 * This file is part of Kunu, a bare-metal task scheduler for microcontrollers.
 * 
 * Copyright (c) 2016 hyves42
 * 
 * This file is licensed according to the terms of the MIT license. 
 * Full text of license is in /LICENSE file
 */
 
#include "tests_state_machine.h"
#include "tests_events_dispatch.h"
#include "tests_events_fifo.h"
#include "tests_sched.h"

int main(void){
	tests_state_machine_run();
	tests_events_dispatch_run();
	tests_events_fifo_run();
	tests_sched_run();
	tests_timer_run();
	return 0;
}