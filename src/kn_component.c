/* 
 * This file is part of Kunu, a bare-metal task scheduler for microcontrollers.
 * 
 * Copyright (c) 2016 hyves42
 * 
 * This file is licensed according to the terms of the MIT license. 
 * Full text of license is in /LICENSE file
 */
 
#include "kn_component.h"
#include <stdio.h>

int kn_component_on_event(kn_event_worker_t *worker, kn_event_t *event){
	kn_component_t *self=(kn_component_t*)worker->user_data;

	int ret=kn_fifo_post(&(self->events_fifo), event);
	self->schedulable_if.state=NEEDS_SCHEDULE;

	printf("event! %d\n", ret);
	return 0;
}

kn_schedulable_return_t kn_component_schedule(kn_schedulable_t *s, void* user_data){
	return DID_NOTHING;
}



int kn_component_broadcast_event(kn_component_t *self, kn_event_t *evt){
	if (!self || !evt){
		return -1;
	}

	evt->sender=&(self->worker_if);
	// TODO time stamp the event here


	return kn_eventdisp_publish(&(self->output_disp), evt);
}