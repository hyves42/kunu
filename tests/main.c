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
#include "tests_linux_simul.h"
#include "tests_system.h"

int main(void){
	int fail_count=0;
	fail_count+=tests_state_machine_run();
	fail_count+=tests_events_dispatch_run();
	fail_count+=tests_events_fifo_run();
	fail_count+=tests_sched_run();
	fail_count+=tests_timer_run();
	fail_count+=tests_linux_simul_run();
	fail_count+=tests_system_run();
	return fail_count;
}