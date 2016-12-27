/* 
 * This file is part of Kunu, a bare-metal task scheduler for microcontrollers.
 * 
 * Copyright (c) 2016 hyves42
 * 
 * This file is licensed according to the terms of the MIT license. 
 * Full text of license is in /LICENSE file
 */
 
#include "kn_component.h"


int kn_component_on_event(kn_event_worker_t *worker, kn_event_t *event){
	kn_component_t *this=(kn_component_t*)worker->user_data;

	kn_fifo_post(&(this->events_fifo), event);
	this->schedulable_if.state=NEEDS_SCHEDULE;

	return 0;
}

kn_schedulable_return_t kn_component_schedule(kn_schedulable_t *s, void* user_data){
	return DID_NOTHING;
}



int kn_component_broadcast_event(kn_component_t *this, kn_event_t *evt){
	evt->sender=&(this->worker_if);

}