/* 
 * This file is part of Kunu, a bare-metal task scheduler for microcontrollers.
 * 
 * Copyright (c) 2016 hyves42
 * 
 * This file is licensed according to the terms of the MIT license. 
 * Full text of license is in /LICENSE file
 */
 
#include "kn_power_sched.h"
#include "kn_power.h"
#include <stdio.h>

static kn_sched_t* sched=NULL;



int kn_power_sched_init(kn_sched_t* s){
	if (!s) return -1;
	sched=s;
	return 0;
}

int kn_power_sched_run(void){
	// perform actions scheduled for now
	while (kn_sched_step(sched) == 0);

	//Sleep until next date where some work is scheduled
	int ticks = sched_get_remaining_ticks(sched);

	// Here notify everyone interested that we are going to sleep
	if (ticks < 0){
		kn_platform_sleep_forever();
	}
	else{
		kn_platform_sleep_for(ticks);
	}
	// Here notify everyone interested that we woke up

}