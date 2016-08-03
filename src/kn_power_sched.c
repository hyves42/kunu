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
#include "kn_tick.h"

#include <stdio.h>

static kn_sched_t* sched=NULL;
static kn_tick_controller_t *tick=NULL;



int kn_power_sched_init(kn_sched_t* s, kn_tick_controller_t *t){
	if (!s || !t) return -1;
	sched=s;
	tick=t;
	return 0;
}

int kn_power_sched_run(void){
	// perform actions scheduled for now
	while (kn_sched_step(sched) == 0);


	//Sleep until next date where some work is scheduled
	int ticks = sched_get_remaining_ticks(sched);
	int slept_ticks=0;

	//printf("need to sleep for %d ticks\n", ticks);

	// Here notify everyone interested that we are going to sleep
	if (ticks < 0){
		slept_ticks=kn_platform_sleep_forever();
	}
	else{
		slept_ticks=kn_platform_sleep_for(ticks);
	}
	kn_tick_broadcast_ticks(tick, slept_ticks);
	// Here notify everyone interested that we woke up

	return slept_ticks;
}