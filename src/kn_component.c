/* 
 * This file is part of Kunu, a bare-metal task scheduler for microcontrollers.
 * 
 * Copyright (c) 2016 hyves42
 * 
 * This file is licensed according to the terms of the MIT license. 
 * Full text of license is in /LICENSE file
 */
 
#include "kn_component.h"
//#include <stdio.h>


int kn_component_on_event(kn_event_worker_t *worker, kn_event_t *event){
	kn_component_t *self=(kn_component_t*)worker->user_data;

	int ret=kn_fifo_post(&(self->events_fifo), event);
	self->schedulable_if.state=NEEDS_SCHEDULE;

	return 0;
}


kn_schedulable_return_t kn_component_schedule(kn_schedulable_t *s, void* user_data){
	kn_component_t *self=(kn_component_t*)user_data;
	kn_event_t event={};


	int ret=kn_fifo_get(&(self->events_fifo), &event);
	if (ret<0){
		// fifo empty, why am I being scheduled ?
		s->state=SLEEPING;
		return DID_NOTHING;
	}

	// Publish and handle this event internally
	kn_eventdisp_publish(&(self->input_disp), &event);

	int count=kn_fifo_count(&(self->events_fifo));
	s->state=(count==0)?SLEEPING:NEEDS_SCHEDULE;
	return DID_SOMETHING;
}


int kn_component_add_listener(kn_component_t *comp, int event_id, kn_event_worker_t *worker){
	if (!comp || !worker){
		return -1;
	}

	return kn_eventdisp_subscribe_worker(&comp->output_disp, event_id, worker);
}


int kn_component_add_listener_comp(kn_component_t *comp, int event_id, kn_component_t *listener_component){
	if (!comp || !listener_component){
		return -1;
	}

	return kn_eventdisp_subscribe_worker(&comp->output_disp, event_id, &listener_component->worker_if);	
}



int kn_component_publish_evt(kn_component_t *self, kn_event_t *evt){
	if (!self || !evt){
		return -1;
	}

	evt->sender=&(self->worker_if);

	return kn_eventdisp_publish(&(self->output_disp), evt);
}