/* 
 * This file is part of Kunu, a bare-metal task scheduler for microcontrollers.
 * 
 * Copyright (c) 2016 hyves42
 * 
 * This file is licensed according to the terms of the MIT license. 
 * Full text of license is in /LICENSE file
 */
 
#include "kunu.h"

 #include <stdio.h>

//typedef struct main_component_t main_component_t;

typedef struct main_component_t main_component_t;

struct main_component_t{
	kn_component_t super;
	kn_event_t fifo_buf[FIFO_SIZE_8];
	kn_event_reg_t output_dispatcher_map[10];
	kn_event_reg_t input_dispatcher_map[1];
	void* user_data;
};

main_component_t main_component={
	.super=KN_COMPONENT_INIT(main_component.super, 
		main_component.fifo_buf, 
		main_component.input_dispatcher_map, 
		main_component.output_dispatcher_map,
		1),
	.input_dispatcher_map={
		KN_EVENT_REGISTER(1, NULL, NULL)
	},
};




kn_sched_t scheduler=KN_SCHED_INIT_VALUE;
kn_tick_controller_t tic = {};



void timer_cb(kn_timer_t *timer, void *user_data){
	printf("caca\n");
}


int main(void){
	kn_timer_t timer={};

	kn_platform_init();
	// j'aime pas trop devoir appeler ,ca chaque fois
	kn_sched_set_default(&scheduler);
	kn_tick_controller_set_default(&tic);
	// power_sched n'a pas une API coh'erente avec tick controller et sched
	kn_power_sched_init(&scheduler, &tic);


	kn_sched_add_schedulable(&scheduler, &main_component.super.schedulable_if);

	kn_timer_init(&timer, timer_cb, NULL, 1);

	kn_timer_start(&timer, 100);

	kn_platform_linux_start_tick_thread();

	while(1){
		kn_power_sched_run();
	}
	return 0;
}